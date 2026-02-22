#include "T_Graphics.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "ElaGraphicsItem.h"
#include "ElaGraphicsScene.h"
#include "ElaGraphicsView.h"
#include "ElaText.h"
#include "ElaPushButton.h"
#include "ElaScrollPageArea.h"
#include "ElaTheme.h"
T_Graphics::T_Graphics(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    //setWindowTitle("绘图");
    // 顶部元素
    createCustomWidget("");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("绘图");

    // GraphicsView
    ElaGraphicsScene* scene = new ElaGraphicsScene(this);
    scene->setSceneRect(centralWidget->rect());
    ElaGraphicsItem* item1 = new ElaGraphicsItem();
    item1->setWidth(100);
    item1->setHeight(100);
    item1->setMaxLinkPortCount(100);
    item1->setMaxLinkPortCount(1);
    item1->setItemImage(QImage(":/Resource/Image/github.png"));
    item1->setItemSelectedImage(QImage(":/Resource/Image/github.png"));
    ElaGraphicsItem* item2 = new ElaGraphicsItem();
    item2->setWidth(100);
    item2->setHeight(100);
    ElaGraphicsItem* item3 = new ElaGraphicsItem();
    item3->setWidth(100);
    item3->setHeight(100);
    scene->addItem(item1);
    scene->addItem(item2);
    scene->addItem(item3);
    ElaGraphicsView* view = new ElaGraphicsView(scene);
    view->setScene(scene);
    view->setMinimumHeight(750);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    view->setAttribute(Qt::WA_TranslucentBackground); // 关键：启用透明通道
    view->setStyleSheet("background: transparent; border: none;"); // 移除边框和背景
    view->setFrameShape(QFrame::NoFrame); // 取消默认边框
    QHBoxLayout* viewLayout = new QHBoxLayout();
    viewLayout->setContentsMargins(0, 0, 12, 0);
    viewLayout->addWidget(view);
    // 放在中心位置
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addLayout(viewLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget, true, false, 0);
}

T_Graphics::~T_Graphics()
{
}
