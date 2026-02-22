#include "T_BasePage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>

#include "ElaMenu.h"
#include "ElaText.h"
#include "ElaTheme.h"
#include "ElaToolButton.h"
#include "ElaPushButton.h"
#include "ElaToolTip.h"
#include "ElaDockWidget.h"
#include "ExamplePage/T_UpdateWidget.h"
#include "Serial/T_AutoDebugWidget.h"
T_BasePage::T_BasePage(QWidget* parent)
    : ElaScrollPage(parent)
{
    connect(eTheme, &ElaTheme::themeModeChanged, this, [=]() {
        if (!parent)
        {
            update();
        }
    });
}

T_BasePage::~T_BasePage()
{
}

void T_BasePage::createCustomWidget(QString desText)
{
    // 顶部元素
    QWidget* customWidget = new QWidget(this);

    // 界面模式切换
    ElaToolButton* themeButton = new ElaToolButton(this);
    themeButton->setFixedSize(35, 35);
    themeButton->setIsTransparent(false);
    themeButton->setElaIcon(ElaIconType::MoonStars);
    connect(themeButton, &ElaToolButton::clicked, this, [=]() {
        eTheme->setThemeMode(eTheme->getThemeMode() == ElaThemeType::Light ? ElaThemeType::Dark : ElaThemeType::Light);
    });
    ElaToolTip* themeButtonToolTip = new ElaToolTip(themeButton);
    themeButtonToolTip->setToolTip("主题");

    // 组件库
    ElaToolButton* elementButton = new ElaToolButton(this);
    elementButton->setFixedSize(35, 35);
    elementButton->setIsTransparent(false);
    elementButton->setElaIcon(ElaIconType::DiagramCells);
    connect(elementButton, &ElaToolButton::clicked, this, [=]() {
        createElaTabWidget();
    });
    ElaToolTip* elementButtonToolTip = new ElaToolTip(elementButton);
    elementButtonToolTip->setToolTip("组件");

    // 颜料盘
    _colorDialog = new ElaColorDialog(this);
    ElaPushButton* colorDialogButton = new ElaPushButton(this);
    colorDialogButton->setFixedSize(35, 35);
    colorDialogButton->setLightDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setLightPressColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkDefaultColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkHoverColor(_colorDialog->getCurrentColor());
    colorDialogButton->setDarkPressColor(_colorDialog->getCurrentColor());
    connect(colorDialogButton, &ElaPushButton::clicked, this, [=]() {
        _colorDialog->exec();
    });
    connect(_colorDialog, &ElaColorDialog::colorSelected, this, [=](const QColor& color) {
        colorDialogButton->setLightDefaultColor(color);
        colorDialogButton->setLightHoverColor(color);
        colorDialogButton->setLightPressColor(color);
        colorDialogButton->setDarkDefaultColor(color);
        colorDialogButton->setDarkHoverColor(color);
        colorDialogButton->setDarkPressColor(color);
    });
    ElaToolTip* elementButtonToolTip1 = new ElaToolTip(colorDialogButton);
    elementButtonToolTip1->setToolTip("颜料盘");

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addSpacing(5);
    buttonLayout->addStretch();
    buttonLayout->addWidget(elementButton);
    buttonLayout->addWidget(colorDialogButton);
    buttonLayout->addWidget(themeButton);
    buttonLayout->addSpacing(15);

    QVBoxLayout* topLayout = new QVBoxLayout(customWidget);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->addSpacing(5);
    topLayout->addLayout(buttonLayout);
    topLayout->addSpacing(5);
    setCustomWidget(customWidget);
}

// 组件库
void T_BasePage::createElaTabWidget()
{
    m_mainWindow = new QMainWindow();
    m_mainWindow->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    if(!m_mainWindow) return;
    ElaDockWidget* DockWidget = new ElaDockWidget("组件", this);
    DockWidget->setWidget(new T_UpdateWidget(this));
//    updateDockWidget->setVisible(!updateDockWidget->isVisible());
    m_mainWindow->addDockWidget(Qt::RightDockWidgetArea, DockWidget);
    m_mainWindow->resizeDocks({DockWidget}, {200}, Qt::Horizontal);
    DockWidget->setFloating(!DockWidget->isFloating());
}

// 调试窗口
void T_BasePage::createAutoDebugTabWidget()
{
    m_mainWindow = new QMainWindow();
    m_mainWindow->setWindowFlag(Qt::WindowStaysOnTopHint, true);
    if(!m_mainWindow) return;
    ElaDockWidget* AutoDebugDockWidget = new ElaDockWidget("", this);
    AutoDebugDockWidget->setWidget(new T_AutoDebugWidget(this));
    m_mainWindow->addDockWidget(Qt::RightDockWidgetArea, AutoDebugDockWidget);
    m_mainWindow->resizeDocks({AutoDebugDockWidget}, {200}, Qt::Horizontal);
    AutoDebugDockWidget->setFloating(!AutoDebugDockWidget->isFloating());

}
