#include "T_AutoDebugWidget.h"
#include <QVBoxLayout>
#include <QDebug>
#include "ElaScrollPageArea.h"
#include "ElaFlowLayout.h"
#include "ElaText.h"
#include "T_Serial.h"
#include <QQuaternion>

#define minValue -360
#define maxValue 360


T_AutoDebugWidget::T_AutoDebugWidget(QWidget* parent)
    : T_BasePage{parent}
{
    setMinimumSize(940, 600);
    robot = RobotWidget::instance();
    ElaScrollPageArea* sliderArea = new ElaScrollPageArea(this);
    QVBoxLayout* sliderLayout = new QVBoxLayout(sliderArea);
    _radioButton1 = new ElaRadioButton("网格", this);
    _radioButton2 = new ElaRadioButton("世界坐标系", this);
    _radioButton1->setAutoExclusive(false);
    _radioButton2->setAutoExclusive(false);
    _radioButton1->setChecked(robot->mGlobalConfig.isDrawGrid);
    _radioButton2->setChecked(robot->mGlobalConfig.isDrawWorldCoord);
    ElaScrollPageArea* radioButtonArea = new ElaScrollPageArea(this);
    QHBoxLayout* radioButtonLayout = new QHBoxLayout(radioButtonArea);
    radioButtonLayout->addWidget(_radioButton1);
    radioButtonLayout->addWidget(_radioButton2);
    radioButtonLayout->addStretch();
    connect(_radioButton1, &QRadioButton::toggled, this, [=](bool checked) {
        robot->mGlobalConfig.isDrawGrid = checked;
        robot->update();
    });
    connect(_radioButton2, &QRadioButton::toggled, this, [=](bool checked) {
        robot->mGlobalConfig.isDrawWorldCoord = checked;
        robot->update();
    });
    if(robot->mRobotModel.isstlModel){
        qDebug() << "stl model";
    }else if(robot->mRobotModel.isurdfModel){
        robotModels = robot->mRobotModel.urdfModel->getModels();
        for (auto* mod : robotModels) {
            ElaText* sliderText = new ElaText();  // 创建 ElaText 对象
            sliderText->setText(mod->Name());
            ElaSlider* _slider = new ElaSlider(this);
            ElaToolTip* _sliderToolTip = new ElaToolTip(_slider);
            _sliderToolTip->setToolTip("0");
            _slider->setRange(minValue, maxValue);
            sliderArea->setMaximumHeight(QWIDGETSIZE_MAX);
            sliderArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            sliderText->setTextPixelSize(15);
            sliderLayout->addWidget(sliderText);
            sliderLayout->addWidget(_slider);
            sliderLayout->addStretch();
            connect(_slider, &QSlider::valueChanged, this, [=](int value) {
                mod->setRotation(QQuaternion::fromAxisAndAngle(mod->axis(), _slider->value()));
                _sliderToolTip->setToolTip(QString::number(_slider->value()));
                robot->update();
            });
        }
    }

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("控制面板");
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->addWidget(radioButtonArea);
    centerLayout->addWidget(sliderArea);
    centerLayout->addStretch();
    centerLayout->setContentsMargins(0, 0, 0, 0);
    addCentralWidget(centralWidget, true, true, 0);

//    _slider1->setValue(robot->mRobotConfig.JVars[1]);
//    _slider2->setValue(robot->mRobotConfig.JVars[2]);
//    _slider3->setValue(robot->mRobotConfig.JVars[3]);
//    _slider4->setValue(robot->mRobotConfig.JVars[4]);
//    _slider5->setValue(robot->mRobotConfig.JVars[5]);
//    _slider6->setValue(robot->mRobotConfig.JVars[6]);

}

T_AutoDebugWidget::~T_AutoDebugWidget()
{
}

