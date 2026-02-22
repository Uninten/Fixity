#include "rrglwidget.h"
#include <QOpenGLFunctions>
#include <QColor>
#include <QFont>
#include <cmath>
#include <QMouseEvent>
#include <QDebug>

RRGLWidget::RRGLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

RRGLWidget::~RRGLWidget(){}

void RRGLWidget::drawGrid()
{
    glPushMatrix();         // 存储当前坐标系位置
    GLfloat color[] = {8.0f / 255, 108.0f / 255, 162.0f / 255};
    //
    // \brief glMaterialfv  指定材质对漫射光的反射率
    // @param face   决定该材质运用于图元的正面还是反面
    // @param pname  表示对何种光进行设置(环境光和漫射光)
    // @param params 四维数组，这个数组描述了反光率的RGBA值，每一项取值都为0-1之间
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);

    int step = 50;
    int num = 15;
    for (int i = -num; i < num + 1; i++) {
        glBegin(GL_LINES);
        glVertex3f(i * step, -num * step, 0);
        glVertex3f(i * step, num * step, 0);
        glVertex3f(-num * step, i * step, 0);
        glVertex3f(num * step, i * step, 0);
        glEnd();
    }
    glPopMatrix();          // 恢复存储的坐标系位置
}

void RRGLWidget::drawCoordinates()
{
    glPushMatrix();
    glLineWidth(2.0f);
    // 绘制X轴（红色）
    setupColor(255, 0, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(900, 0, 0);
    glEnd();

    // 绘制Y轴（绿色）
    setupColor(0, 255, 0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 900, 0);
    glEnd();

    // 绘制Z轴（蓝色）
    setupColor(0, 0, 255);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 700);
    glEnd();

    glLineWidth(1.0f);
    glPopMatrix();
}

void RRGLWidget::drawSTLCoordinates(int r, int g, int b)
{
    glPushMatrix();
    glLineWidth(1.5f);
    setupColor(r, g, b);
    glBegin(GL_LINES);

    glVertex3f(-300, 0, 0);
    glVertex3f(300, 0, 0);
    glVertex3f(0, -300, 0);
    glVertex3f(0, 300, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 500);
    glEnd();

    glLineWidth(1.0f);
    glPopMatrix();
}

void RRGLWidget::drawGL()
{
    //子类重写这个接口，显示各自的模型
    qDebug() << " RRGLWidget drawGL";
}

void RRGLWidget::setupColor(int r, int g, int b)
{
    GLfloat color[] = {static_cast<GLfloat>(r / 255.0), static_cast<GLfloat>(g / 255.0), static_cast<GLfloat>(b / 255.0)};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
}

void RRGLWidget::setXRotation(int angle)
{
    int tangle = angle;         // normalizeAngle(angle);
    if (tangle != xRot) {
        xRot = tangle;
        emit xRotationChanged(angle);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        update();
    }
}

void RRGLWidget::setYRotation(int angle)
{
    int tangle = angle;         // normalizeAngle(angle);
    if (tangle != yRot) {
        yRot = tangle;
        emit yRotationChanged(angle);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void RRGLWidget::setXYTranslate(int dx, int dy)
{
    xTran += 3.0 * dx;
    yTran -= 3.0 * dy;
    update();
}

int RRGLWidget::normalizeAngle(int angle)
{
    int tangle = angle;
    while (tangle < 0) {
        tangle += 360 * 16;
    }
    while (tangle > 360 * 16) {
        tangle -= 360 * 16;
    }
    return tangle;
}

void RRGLWidget::setZoom(int zoom)
{
    z_zoom = zoom;
    update();

}

void RRGLWidget::initializeGL()
{

    QSurfaceFormat format;
    format.setAlphaBufferSize(8);  // 必须启用 Alpha 通道
    setFormat(format);            // 必须在 show() 之前调用

    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_AlwaysStackOnTop);  // 确保窗口在最上层
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    //用来初始化这个OpenGL窗口部件的，可以在里面设定一些有关选项
    GLfloat ambientLight[] = {0.7f, 0.7f, 0.7f, 1.0f};      //光源环境光强度数组
    GLfloat diffuseLight[] = {0.7f, 0.8f, 0.8f, 1.0f};      //光源散射光强度数组
    GLfloat specularLight[] = {0.4f, 0.4f, 0.4f, 1.0f};     //光源镜面反射光强度数组
    GLfloat positionLight[] = {20.0f, 20.0f, 20.0f, 0.0f};  //光源位置数组

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);     //设置0号光源的环境光属性
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);     //设置0号光源的散射光属性
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);   //设置0号光源的镜面反射光属性
    glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 1.0);        //设置照明模型参数
    glLightfv(GL_LIGHT0, GL_POSITION, positionLight);   //设置0号光源的位置属性

    glEnable(GL_LIGHTING);   //启用光照
    glEnable(GL_LIGHT0);     //打开光源
//    glEnable(GL_DEPTH_TEST); //隐藏表面消除，打开深度缓冲区，绘制3D图像时候使用

    glClearDepth(1.0);       // 设置深度缓存
    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glDepthFunc(GL_LEQUAL);  // 设置深度测试的类型
    glEnable(GL_NORMALIZE);

}

void RRGLWidget::paintGL()
{
    //用来绘制OpenGL的窗口了，只要有更新发生，这个函数就会被调用
}

void RRGLWidget::resizeGL(int w, int h)
{
    if (w < 0 || h < 0) {
        return;
    }
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLfloat zNear = 1.0;
    GLfloat zFar = 20000.0;
    GLfloat aspect = (GLfloat) w / (GLfloat) h;
    GLfloat fH = tan(GLfloat(70.0 / 360.0 * 3.14159)) * zNear;
    GLfloat fW = fH * aspect;
    glFrustum(-fW, fW, -fH, fH, zNear, zFar);       //将当前矩阵与一个透视矩阵相乘，把当前矩阵转变成透视矩阵，

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(0.0, 0.0, -40.0);
}

void RRGLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void RRGLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();
    // 这里必须使用buttons()
    if (event->buttons() & Qt::LeftButton) {  //进行的按位与
        setXRotation(xRot + 4 * dy);
        setYRotation(yRot - 4 * dx);
    } else if (event->buttons() & Qt::MidButton) {
        setXYTranslate(dx, dy);
    }

    lastPos = event->pos();
}

void RRGLWidget::wheelEvent(QWheelEvent *event)
{
    double delta = event->angleDelta().y() / 5.0;  // 获取滚轮方向（1或-1）
    setZoom(z_zoom + 5.0 * delta);  // 更新缩放值
    event->accept();  // 阻止事件继续传递
}
