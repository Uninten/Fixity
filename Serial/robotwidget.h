#ifndef ROBOTWIDGET_H
#define ROBOTWIDGET_H

#include "rrglwidget.h"
#include "urdffileloader.h"
class ElaMenu;

class RobotWidget : public RRGLWidget
{ 
public:
    RobotWidget(QWidget *parent = 0);
    ~RobotWidget();

    void drawGL() override;
    void loadFile();
    void clearFile();
    static RobotWidget* instance() {  // 返回指针而不是引用
        static RobotWidget instance;
        return &instance;
    }

protected:
    void paintGL() override;
//    void mousePressEvent(QMouseEvent *event) override;
//    void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event);

private:
    ElaMenu* _robotMenu{nullptr};

public:
    typedef struct GlobalConfig {
      bool isDrawGrid;        //是否画网格
      bool isDrawWorldCoord;  //世界坐标系
    } GlobalConfig;

    typedef struct _RobotModel {
      // 7个小部件组成
        STLFileLoader* stlModel{nullptr};
        bool isstlModel{false};
        URDFFileLoader* urdfModel{nullptr};
        bool isurdfModel{false};
    } RobotModel;

    RobotModel mRobotModel;
    GlobalConfig mGlobalConfig;
};

#endif // ROBOTWIDGET_H
