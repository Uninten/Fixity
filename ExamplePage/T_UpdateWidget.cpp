#include "T_UpdateWidget.h"

#include <QVBoxLayout>

#include "ElaText.h"
#include "ElaGraphicsItem.h"
#include "ElaGraphicsScene.h"
#include "ElaGraphicsView.h"
#include "ElaToolTip.h"
T_UpdateWidget::T_UpdateWidget(QWidget* parent)
    : QWidget{parent}
{
    setMinimumSize(200, 260);
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
    mainLayout->setContentsMargins(5, 10, 5, 400);
    mainLayout->setSpacing(4);

    ElaGraphicsItem* item1 = new ElaGraphicsItem();
    item1->setWidth(100);
    item1->setHeight(100);
    item1->setMaxLinkPortCount(100);
    item1->setMaxLinkPortCount(1);
    item1->setItemImage(QImage(":/Resource/Image/github.png"));
    item1->setItemSelectedImage(QImage(":/Resource/Image/github.png"));
    ElaGraphicsScene* scene = new ElaGraphicsScene(this);
    scene->addItem(item1);
    ElaGraphicsView* view = new ElaGraphicsView(scene);
    view->setScene(scene);

    ElaToolTip* itemToolTip1 = new ElaToolTip(view);
    itemToolTip1->setToolTip("Github");

    mainLayout->addStretch();
    mainLayout->addWidget(view);
    mainLayout->addStretch();

}

T_UpdateWidget::~T_UpdateWidget()
{
}

