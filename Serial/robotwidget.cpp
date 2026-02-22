#include "robotwidget.h"
#include <QDebug>
#include <QOpenGLFunctions>
#include <QFileDialog>
#include <fstream>
#include <QTimer>
#include "ElaMenu.h"
#include "ElaNavigationRouter.h"

RobotWidget::RobotWidget(QWidget *parent) : RRGLWidget(parent) {
    // 菜单
    _robotMenu = new ElaMenu(this);
//     QKeySequence key = QKeySequence(Qt::CTRL | Qt::Key_S);
    QAction* loadaction = _robotMenu->addElaIconAction(ElaIconType::FileArrowDown, "导入模型");
    _robotMenu->addSeparator();
    QAction* delaction = _robotMenu->addElaIconAction(ElaIconType::DeleteRight, "清空模型");
//    _robotMenu->addElaIconAction(ElaIconType::BoxCheck, "保存", QKeySequence::Save);
//    _robotMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
//    _robotMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
//    connect(action, &QAction::triggered, this, [=]() {
//        ElaNavigationRouter::getInstance()->navigationRouteBack();
//    });
    connect(loadaction, &QAction::triggered, this, [=]() {
        loadFile();
    });
    connect(delaction, &QAction::triggered, this, [=]() {
        clearFile();
        update();
    });
    mGlobalConfig = {false, false};
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() { update(); });
        timer->start(16); // 约60FPS
}

RobotWidget::~RobotWidget() {
    //    delete objRobot;
    //    objRobot = nullptr;
}

void RobotWidget::drawGL() {
    glPushMatrix();
    if (mGlobalConfig.isDrawGrid) {
        drawGrid();
    }
    if (mGlobalConfig.isDrawWorldCoord) {
        drawCoordinates();
    }
    // stlmodel
    if(mRobotModel.isstlModel){
        setupColor(255, 69, 0);
        mRobotModel.stlModel->draw();
    }
    // urdfmodel
    if(mRobotModel.isurdfModel){
        mRobotModel.urdfModel->draw();
    }
    glPopMatrix();
}

void RobotWidget::loadFile()
{
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "选择文件",
        "",
        "所有文件 (*.*);;STL文件 (*.stl);;URDF文件 (*.urdf)"
    );
    if (!filePath.isEmpty()) {
        QFileInfo fileInfo(filePath);
        QString suffix = fileInfo.suffix().toLower();  // 获取小写的文件后缀
        if (suffix == "stl") {
            mRobotModel.stlModel = new STLFileLoader(filePath, 1000);
            mRobotModel.isstlModel = true;
        }else if (suffix == "urdf") {
            qDebug() << "导入urdf模型";
            mRobotModel.urdfModel = new URDFFileLoader(filePath, 1000);
            mRobotModel.isurdfModel = true;
        }else {
            qDebug() << "导入其他模型";
        }
    }
}

void RobotWidget::clearFile()
{
//    mRobotModel.stlModel.reset();
    if (mRobotModel.stlModel) {
       delete mRobotModel.stlModel;
       mRobotModel.stlModel = nullptr;
       mRobotModel.isstlModel = false;
    }
    if (mRobotModel.urdfModel) {
       delete mRobotModel.urdfModel;
       mRobotModel.urdfModel = nullptr;
       mRobotModel.isurdfModel = false;
    }
}

void RobotWidget::paintGL() {
    //用来绘制OpenGL的窗口了，只要有更新发生，这个函数就会被调用
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除屏幕和深度缓存
    glPushMatrix();
    glTranslated(0, 0, z_zoom);
    glTranslated(xTran, yTran, 0);
    glRotated(xRot / 16.0, 1.0, 0.0, 0.0); //绕x轴旋转
    glRotated(yRot / 16.0, 0.0, 1.0, 0.0); //绕y轴旋转
    glRotated(zRot / 16.0, 0.0, 0.0, 1.0); //绕z轴旋转
    glRotated(+90.0, 1.0, 0.0, 0.0);
    drawGL();
    glPopMatrix();
}

void RobotWidget::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::RightButton:
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        _robotMenu->popup(event->globalPosition().toPoint());
#else
        _robotMenu->popup(event->globalPos());
#endif
        break;
    }
    default:
    {
        break;
    }
    }
}
