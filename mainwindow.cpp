#include "mainwindow.h"

#include "ElaMenuBar.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"
#include "ElaProgressBar.h"
#include "ElaDockWidget.h"
#include "ElaStatusBar.h"
#include "ElaText.h"

#include "ExamplePage/T_Home.h"
#include "ExamplePage/T_LogWidget.h"
#include "ExamplePage/T_UpdateWidget.h"
#include "ExamplePage/T_Setting.h"
#include "ExamplePage/T_Graphics.h"
#include "Serial/T_Serial.h"
#include "Note/T_Note.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : ElaWindow(parent)
{
    initWindow();

    initEdgeLayout();

    initContent();
}

MainWindow::~MainWindow()
{
}

void MainWindow::initWindow()
{
//     setIsCentralStackedWidgetTransparent(true);
     resize(1560, 920);
     //setIsStayTop(true);
     //setWindowTitle("Fixity");
     setUserInfoCardVisible(false);
}

void MainWindow::initEdgeLayout()
{
    //工具栏
    ElaToolBar* toolBar = new ElaToolBar("工具栏", this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolBar->setToolBarSpacing(3);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setIconSize(QSize(25, 25));
    // toolBar->setFloatable(false);
    // toolBar->setMovable(false);
    ElaToolButton* toolButton1 = new ElaToolButton(this);
    toolButton1->setElaIcon(ElaIconType::BadgeCheck);
    toolBar->addWidget(toolButton1);
    ElaToolButton* toolButton2 = new ElaToolButton(this);
    toolButton2->setElaIcon(ElaIconType::ChartUser);
    toolBar->addWidget(toolButton2);
    toolBar->addSeparator();
    ElaToolButton* toolButton3 = new ElaToolButton(this);
    toolButton3->setElaIcon(ElaIconType::Bluetooth);
    toolButton3->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    toolButton3->setText("蓝牙");
    toolBar->addWidget(toolButton3);
    ElaToolButton* toolButton4 = new ElaToolButton(this);
    toolButton4->setElaIcon(ElaIconType::BringFront);
    toolBar->addWidget(toolButton4);
    toolBar->addSeparator();
    ElaToolButton* toolButton5 = new ElaToolButton(this);
    toolButton5->setElaIcon(ElaIconType::ChartSimple);
    toolBar->addWidget(toolButton5);
    ElaToolButton* toolButton6 = new ElaToolButton(this);
    toolButton6->setElaIcon(ElaIconType::FaceClouds);
    toolBar->addWidget(toolButton6);
    ElaToolButton* toolButton8 = new ElaToolButton(this);
    toolButton8->setElaIcon(ElaIconType::Aperture);
    toolBar->addWidget(toolButton8);
    ElaToolButton* toolButton9 = new ElaToolButton(this);
    toolButton9->setElaIcon(ElaIconType::ChartMixed);
    toolBar->addWidget(toolButton9);
    ElaToolButton* toolButton10 = new ElaToolButton(this);
    toolButton10->setElaIcon(ElaIconType::Coins);
    toolBar->addWidget(toolButton10);
//    ElaToolButton* toolButton11 = new ElaToolButton(this);
//    toolButton11->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    toolButton11->setElaIcon(ElaIconType::AlarmPlus);
//    toolButton11->setText("AlarmPlus");
//    toolBar->addWidget(toolButton11);
    ElaToolButton* toolButton12 = new ElaToolButton(this);
    toolButton12->setElaIcon(ElaIconType::Crown);
    toolBar->addWidget(toolButton12);
    QAction* test = new QAction(this);
    test->setMenu(new QMenu(this));

    ElaProgressBar* progressBar = new ElaProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setFixedWidth(350);
    toolBar->addWidget(progressBar);

    this->addToolBar(Qt::TopToolBarArea, toolBar);

//    //停靠窗口
//    ElaDockWidget* logDockWidget = new ElaDockWidget("日志信息", this);
//    logDockWidget->setWidget(new T_LogWidget(this));
//    this->addDockWidget(Qt::RightDockWidgetArea, logDockWidget);
//    resizeDocks({logDockWidget}, {200}, Qt::Horizontal);

//    ElaDockWidget* elementDockWidget = new ElaDockWidget("更新内容", this);
//    elementDockWidget->setWidget(new T_UpdateWidget(this));
//    this->addDockWidget(Qt::RightDockWidgetArea, elementDockWidget);
//    resizeDocks({elementDockWidget}, {200}, Qt::Horizontal);
}

void MainWindow::initContent()
{
     _homePage = new T_Home(this);
     _graphicsPage = new T_Graphics(this);
     _serialPage = new T_Serial(this);
     _notePage = new T_Note(this);
     _settingPage = new T_Setting(this);


     addPageNode("主页", _homePage, ElaIconType::House); // home键
     addPageNode("绘图", _graphicsPage, ElaIconType::Paintbrush); // 画图
     addPageNode("协议与连接", _serialPage, ElaIconType::Bagel);   // 串口调试助手
     addPageNode("笔记", _notePage, ElaIconType::BookOpenCover);   // 笔记

     addFooterNode("设置", _settingPage, _settingKey, 0, ElaIconType::GearComplex); // 设置
     connect(this, &MainWindow::userInfoCardClicked, this, [=]() {
         this->navigation(_homePage->property("ElaPageKey").toString());
     });
}
