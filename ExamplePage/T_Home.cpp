#include "T_Home.h"

#include <QDebug>
#include <QDesktopServices>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QVBoxLayout>

#include "ElaAcrylicUrlCard.h"
#include "ElaFlowLayout.h"
#include "ElaImageCard.h"
#include "ElaMenu.h"
#include "ElaMessageBar.h"
#include "ElaNavigationRouter.h"
#include "ElaPopularCard.h"
#include "ElaScrollArea.h"
#include "ElaText.h"
#include "ElaToolTip.h"
T_Home::T_Home(QWidget* parent)
    : T_BasePage(parent)
{
    // 预览窗口标题
    setWindowTitle("Home");

    setTitleVisible(false);
    setContentsMargins(2, 2, 0, 0);
    // 标题卡片区域
    ElaText* titleText = new ElaText("FIXITY", this);
    titleText->setTextPixelSize(35);

    QVBoxLayout* titleLayout = new QVBoxLayout();
    titleLayout->setContentsMargins(30, 60, 0, 0);
    titleLayout->addWidget(titleText);

    ElaImageCard* backgroundCard = new ElaImageCard(this);
    backgroundCard->setBorderRadius(10);
    backgroundCard->setFixedHeight(400);
    backgroundCard->setMaximumAspectRatio(1.7);
    backgroundCard->setCardImage(QImage(":/Resource/Image/Home_Background.png"));

    ElaAcrylicUrlCard* urlCard1 = new ElaAcrylicUrlCard(this);
    urlCard1->setCardPixmapSize(QSize(62, 62));
    urlCard1->setFixedSize(195, 225);
    urlCard1->setTitlePixelSize(17);
    urlCard1->setTitleSpacing(25);
    urlCard1->setSubTitleSpacing(13);
    urlCard1->setUrl("https://github.com");
    urlCard1->setCardPixmap(QPixmap(":/Resource/Image/github.png"));
    urlCard1->setTitle("Github");
    urlCard1->setSubTitle("GitHub 是一个基于云的软件开发平台，为开发者提供代码托管和版本控制服务。它基于Git版本控制系统，允许开发者存储、管理代码，并跟踪代码的更改历史。GitHub 不仅是一个代码托管工具，更是一个协作平台，支持团队合作、代码审查、项目管理等功能。");
//    ElaToolTip* urlCard1ToolTip = new ElaToolTip(urlCard1);
//    urlCard1ToolTip->setToolTip("https://github.com");
    ElaAcrylicUrlCard* urlCard2 = new ElaAcrylicUrlCard(this);
    urlCard2->setCardPixmapSize(QSize(62, 62));
    urlCard2->setFixedSize(195, 225);
    urlCard2->setTitlePixelSize(17);
    urlCard2->setTitleSpacing(25);
    urlCard2->setSubTitleSpacing(13);
    urlCard2->setUrl("https://www.bilibili.com/");
    urlCard2->setCardPixmap(QPixmap(":/Resource/Image/Bil.png"));
    urlCard2->setTitle("哔哩哔哩");
    urlCard2->setSubTitle("哔哩哔哩（NASDAQ:BILI；HKEX:9626 [154]），英文名称：bilibili。");
    ElaAcrylicUrlCard* urlCard3 = new ElaAcrylicUrlCard(this);
    urlCard3->setCardPixmapSize(QSize(62, 62));
    urlCard3->setFixedSize(195, 225);
    urlCard3->setTitlePixelSize(17);
    urlCard3->setTitleSpacing(25);
    urlCard3->setSubTitleSpacing(13);
    urlCard3->setUrl("https://element.eleme.cn/#/zh-CN/component/layout");
    urlCard3->setCardPixmap(QPixmap(":/Resource/Image/Element.png"));
    urlCard3->setTitle("Element");
    urlCard3->setSubTitle("Element 是一套基于Vue实现的UI组件库，提供了丰富的 PC 端组件，降低了开发的难易程度。Element 是为开发者、设计师和产品经理准备的，可以美化界面，简化代码。");

    ElaScrollArea* cardScrollArea = new ElaScrollArea(this);
    cardScrollArea->setWidgetResizable(true);
    cardScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardScrollArea->setIsGrabGesture(true, 0);
    cardScrollArea->setIsOverShoot(Qt::Horizontal, true);
    QWidget* cardScrollAreaWidget = new QWidget(this);
    cardScrollAreaWidget->setStyleSheet("background-color:transparent;");
    cardScrollArea->setWidget(cardScrollAreaWidget);
    QHBoxLayout* urlCardLayout = new QHBoxLayout();
    urlCardLayout->setSpacing(15);
    urlCardLayout->setContentsMargins(30, 0, 0, 6);
    urlCardLayout->addWidget(urlCard1);
    urlCardLayout->addWidget(urlCard2);
    urlCardLayout->addWidget(urlCard3);
    urlCardLayout->addStretch();
    QVBoxLayout* cardScrollAreaWidgetLayout = new QVBoxLayout(cardScrollAreaWidget);
    cardScrollAreaWidgetLayout->setContentsMargins(0, 0, 0, 0);
    cardScrollAreaWidgetLayout->addStretch();
    cardScrollAreaWidgetLayout->addLayout(urlCardLayout);

    QVBoxLayout* backgroundLayout = new QVBoxLayout(backgroundCard);
    backgroundLayout->setContentsMargins(0, 0, 0, 0);
    backgroundLayout->addLayout(titleLayout);
    backgroundLayout->addWidget(cardScrollArea);

    // 推荐卡片
    ElaText* flowText = new ElaText("热门免费应用", this);
    flowText->setTextPixelSize(20);
    QHBoxLayout* flowTextLayout = new QHBoxLayout();
    flowTextLayout->setContentsMargins(33, 0, 0, 0);
    flowTextLayout->addWidget(flowText);
    // ElaFlowLayout
    ElaPopularCard* homeCard = new ElaPopularCard(this);
    connect(homeCard, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        QDesktopServices::openUrl(QUrl("https://yuanbao.tencent.com/chat/naQivTmsDa/d7793c82-2f3d-4c5d-8c00-5f9fe0ea9c36"));
    });
    homeCard->setCardPixmap(QPixmap(":/Resource/Image/AI.png"));
    homeCard->setTitle("腾讯元宝");
    homeCard->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard->setInteractiveTips("免费使用");
    homeCard->setDetailedText("腾讯元宝是深圳市腾讯计算机系统有限公司基于自研混元大模型开发的C端AI助手App，于2024年5月30日上线。 腾讯元宝支持语音和文字搜索，涵盖微信公众号和视频号等信源。 搜索结果以图片、视频、音乐和地图等形式展现；支持PDF、Word、PPT、Excel、TXT等格式文档识别，支持本地和微信上传；腾讯元宝可用于撰写报告、方案、文案和代码。");
    homeCard->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard1 = new ElaPopularCard(this);
    connect(homeCard1, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaScreenNavigation();
    });
    homeCard1->setTitle("ElaScreen");
    homeCard1->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard1->setCardPixmap(QPixmap(":/Resource/Image/control/AutomationProperties.png"));
    homeCard1->setInteractiveTips("免费使用");
    homeCard1->setDetailedText("使用ElaDxgiManager获取屏幕的实时数据，以QImage的形式处理数据，支持切换采集设备和输出设备。");
    homeCard1->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard2 = new ElaPopularCard(this);
    connect(homeCard2, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaSceneNavigation();
    });
    homeCard2->setTitle("ElaScene");
    homeCard2->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard2->setCardPixmap(QPixmap(":/Resource/Image/control/Canvas.png"));
    homeCard2->setInteractiveTips("免费使用");
    homeCard2->setDetailedText("使用ElaScene封装的高集成度API进行快速拓扑绘图开发，对基于连接的网络拓扑特化处理。");
    homeCard2->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard3 = new ElaPopularCard(this);
    connect(homeCard3, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaBaseComponentNavigation();
    });
    homeCard3->setTitle("ElaBaseComponent");
    homeCard3->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard3->setCardPixmap(QPixmap(":/Resource/Image/control/StandardUICommand.png"));
    homeCard3->setInteractiveTips("免费使用");
    homeCard3->setDetailedText("添加ElaBaseComponent页面中的基础组件到你的项目中以进行快捷开发，使用方便，结构整洁，API规范");
    homeCard3->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard4 = new ElaPopularCard(this);
    connect(homeCard4, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaCardNavigation();
    });
    homeCard4->setTitle("ElaCard");
    homeCard4->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard4->setCardPixmap(QPixmap(":/Resource/Image/control/FlipView.png"));
    homeCard4->setInteractiveTips("免费使用");
    homeCard4->setDetailedText("使用ElaCard系列组件，包括促销卡片和促销卡片视窗来快速建立循环动画。");
    homeCard4->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaPopularCard* homeCard5 = new ElaPopularCard(this);
    connect(homeCard5, &ElaPopularCard::popularCardButtonClicked, this, [=]() {
        Q_EMIT elaIconNavigation();
    });
    homeCard5->setTitle("ElaIcon");
    homeCard5->setSubTitle("5.0⭐ 实用程序与工具");
    homeCard5->setCardPixmap(QPixmap(":/Resource/Image/control/CommandBarFlyout.png"));
    homeCard5->setInteractiveTips("免费使用");
    homeCard5->setDetailedText("在该界面快速挑选你喜欢的图标应用到项目中，以枚举的形式使用它");
    homeCard5->setCardFloatPixmap(QPixmap(":/Resource/Image/IARC/IARC_7+.svg.png"));

    ElaFlowLayout* flowLayout = new ElaFlowLayout(0, 5, 5);
    flowLayout->setContentsMargins(30, 0, 0, 0);
    flowLayout->setIsAnimation(true);
    flowLayout->addWidget(homeCard);
    flowLayout->addWidget(homeCard1);
    flowLayout->addWidget(homeCard2);
    flowLayout->addWidget(homeCard3);
    flowLayout->addWidget(homeCard4);
    flowLayout->addWidget(homeCard5);

    // 菜单
    _homeMenu = new ElaMenu(this);
    ElaMenu* checkMenu = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu->addAction("查看1");
    checkMenu->addAction("查看2");
    checkMenu->addAction("查看3");
    checkMenu->addAction("查看4");

    ElaMenu* checkMenu1 = _homeMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu1->addAction("查看1");
    checkMenu1->addAction("查看2");
    checkMenu1->addAction("查看3");
    checkMenu1->addAction("查看4");

    ElaMenu* checkMenu2 = checkMenu->addMenu(ElaIconType::Cubes, "查看");
    checkMenu2->addAction("查看1");
    checkMenu2->addAction("查看2");
    checkMenu2->addAction("查看3");
    checkMenu2->addAction("查看4");

    // QKeySequence key = QKeySequence(Qt::CTRL | Qt::Key_S);

    _homeMenu->addSeparator();
    _homeMenu->addElaIconAction(ElaIconType::BoxCheck, "排序方式", QKeySequence::Save);
    _homeMenu->addElaIconAction(ElaIconType::ArrowRotateRight, "刷新");
    QAction* action = _homeMenu->addElaIconAction(ElaIconType::ArrowRotateLeft, "撤销");
    connect(action, &QAction::triggered, this, [=]() {
        ElaNavigationRouter::getInstance()->navigationRouteBack();
    });

    _homeMenu->addElaIconAction(ElaIconType::Copy, "复制");
    _homeMenu->addElaIconAction(ElaIconType::MagnifyingGlassPlus, "显示设置");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("Home");
    QVBoxLayout* centerVLayout = new QVBoxLayout(centralWidget);
    centerVLayout->setSpacing(0);
    centerVLayout->setContentsMargins(0, 0, 0, 0);
    centerVLayout->addWidget(backgroundCard);
    centerVLayout->addSpacing(20);
    centerVLayout->addLayout(flowTextLayout);
    centerVLayout->addSpacing(10);
    centerVLayout->addLayout(flowLayout);
    centerVLayout->addStretch();
    addCentralWidget(centralWidget);

    // 初始化提示
    ElaMessageBar::success(ElaMessageBarType::BottomRight, "Success", "初始化成功!", 2000);
    qDebug() << "初始化成功";
}

T_Home::~T_Home()
{
}

void T_Home::mouseReleaseEvent(QMouseEvent* event)
{
    switch (event->button())
    {
    case Qt::RightButton:
    {
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        _homeMenu->popup(event->globalPosition().toPoint());
#else
        _homeMenu->popup(event->globalPos());
#endif
        break;
    }
    default:
    {
        break;
    }
    }
    ElaScrollPage::mouseReleaseEvent(event);
}
