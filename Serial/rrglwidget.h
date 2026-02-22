#ifndef RRGLWIDGET_H
#define RRGLWIDGET_H

#include <QOpenGLWidget>
#include "stlfileloader.h"

class RRGLWidget : public QOpenGLWidget
{
//显示机械臂gl基类，提供了鼠标旋转、方法缩小、平移、网格、坐标系等基本功能

Q_OBJECT
public:
    RRGLWidget(QWidget *parent = 0);
    ~RRGLWidget();
    void drawGrid();
    void drawCoordinates();
    void drawSTLCoordinates(int r, int g, int b);
    virtual void drawGL();
    void setupColor(int r, int g, int b);
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setXYTranslate(int dx, int dy);
    int normalizeAngle(int angle);
    void setZoom(int zoom);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
public:
    GLdouble xRot{-2584}; // x旋转角度
    GLdouble yRot{1376};  // y旋转角度
    GLdouble zRot{0.0f};   // z旋转角度
    int xTran{0};
    int yTran{0};
    int z_zoom{-2500};
    QPoint lastPos{0, 0};

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
};

#endif // RRGLWIDGET_H
