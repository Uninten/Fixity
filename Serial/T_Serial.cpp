#include "T_Serial.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort/QSerialPort>
#include <QDateTime>

#include "ElaLCDNumber.h"
#include "ElaPromotionCard.h"
#include "ElaPromotionView.h"
#include "ElaToggleButton.h"
#include "ElaScrollPageArea.h"
#include "ElaText.h"
#include "ElaComboBox.h"
#include "ElaToolTip.h"
#include "ElaPlainTextEdit.h"
#include "ElaToolBar.h"
#include "ElaToolButton.h"
#include "ElaTabWidget.h"
#include "Base/qcustomplot.h"

T_Serial::T_Serial(QWidget* parent)
    : T_BasePage(parent)
{
    init_Page();

}

T_Serial::~T_Serial()
{
}

void T_Serial::ScanSerialName(QComboBox* device)
{
    device->clear();  // 清空串口
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &portInfo : serialPortInfos)
    {
        QSerialPort serial;
        serial.setPort(portInfo);
        device->addItem(portInfo.portName());
        serial.close();
    }
    if(device->count() == 0)
    {
        QString WrrCom;
        WrrCom.append("ERROR");
        device->addItem(WrrCom);
    }
}

void T_Serial::init_Page()
{
    // 连接开关
    _toggleButton = new ElaToggleButton("", this);
    _toggleButton->setFixedWidth(40);
    _toggleButton->setFixedHeight(40);
    connect(_toggleButton, &ElaToggleButton::toggled, this, [=]() {
        isSwitchOn = !isSwitchOn; // 切换状态
        qDebug() << "Switch is now:" << (isSwitchOn ? "ON" : "OFF");
        isSwitchOn ? StartSerial() : CloseSerial();
    });
    ElaToolTip* _toggleButtonToolTip = new ElaToolTip(_toggleButton);
    _toggleButtonToolTip->setToolTip("开关");

    // 数据接口
    _comboBox = new ElaComboBox(this);
    QStringList comboList{
        "串口",
        "UDP",
        "TCP客户端",
        "TCP服务端",
        "Demo数据"};
    _comboBox->addItems(comboList);
    ElaText* comboBoxText = new ElaText("数据接口", this);
    comboBoxText->setTextPixelSize(15);

    // 端口号
    _comboBox1 = new ElaComboBox(this);
    QStringList comboList1{
        ""};
    _comboBox1->addItems(comboList1);
    ElaText* comboBoxText1 = new ElaText("端口号", this);
    comboBoxText1->setTextPixelSize(15);
    ScanSerialName(_comboBox1);

    // 波特率
    _comboBox2 = new ElaComboBox(this);
    QStringList comboList2{
        "921600",
        "576000",
        "500000",
        "460800",
        "230400",
        "115200",
        "74800",
        "57600",
        "38400",
        "19200",
        "9600",
        "4800",
        "2400"};
    _comboBox2->addItems(comboList2);
    ElaText* comboBoxText2 = new ElaText("波特率", this);
    comboBoxText2->setTextPixelSize(15);
    _comboBox2->setCurrentIndex(5);

    // 数据流控
    _comboBox3 = new ElaComboBox(this);
    QStringList comboList3{
        "None",
        "Hard",
        "Soft"};
    _comboBox3->addItems(comboList3);
    ElaText* comboBoxText3 = new ElaText("数据流控", this);
    comboBoxText3->setTextPixelSize(15);

    // 校验位
    _comboBox4 = new ElaComboBox(this);
    QStringList comboList4{
        "None",
        "Even",
        "Odd",
        "Space",
        "Mark"};
    _comboBox4->addItems(comboList4);
    ElaText* comboBoxText4 = new ElaText("校验位", this);
    comboBoxText4->setTextPixelSize(15);

    // 数据位数
    _comboBox5 = new ElaComboBox(this);
    QStringList comboList5{
        "8",
        "7",
        "6",
        "5"};
    _comboBox5->addItems(comboList5);
    ElaText* comboBoxText5 = new ElaText("数据位数", this);
    comboBoxText5->setTextPixelSize(15);

    // 停止位数
    _comboBox6 = new ElaComboBox(this);
    QStringList comboList6{
        "1",
        "1.5",
        "2"};
    _comboBox6->addItems(comboList6);
    ElaText* comboBoxText6 = new ElaText("停止位", this);
    comboBoxText6->setTextPixelSize(15);

    ElaScrollPageArea* SerialToolBarArea = new ElaScrollPageArea(this);
    QHBoxLayout* SerialToolBar = new QHBoxLayout(SerialToolBarArea);
    SerialToolBar->addWidget(_toggleButton);
    SerialToolBar->addWidget(comboBoxText);
    SerialToolBar->addWidget(_comboBox);
    SerialToolBar->addWidget(comboBoxText1);
    SerialToolBar->addWidget(_comboBox1);
    SerialToolBar->addWidget(comboBoxText2);
    SerialToolBar->addWidget(_comboBox2);
    SerialToolBar->addWidget(comboBoxText3);
    SerialToolBar->addWidget(_comboBox3);
    SerialToolBar->addWidget(comboBoxText4);
    SerialToolBar->addWidget(_comboBox4);
    SerialToolBar->addWidget(comboBoxText5);
    SerialToolBar->addWidget(_comboBox5);
    SerialToolBar->addWidget(comboBoxText6);
    SerialToolBar->addWidget(_comboBox6);

    // ElaTabWidget
    _tabWidget = new ElaTabWidget(this);
    _tabWidget->setMinimumSize(1250, 400);
    _tabWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _tabWidget->setAttribute(Qt::WA_TranslucentBackground); // 关键：启用透明通道
    _tabWidget->setStyleSheet("QTabWidget::pane { background: transparent; border: none; }"); // 移除边框和背景

    // 页面1
    ElaScrollPageArea* Page1Area = new ElaScrollPageArea();
    QVBoxLayout* Page1Layout = new QVBoxLayout(Page1Area);
//    Page1Area->setMinimumSize(1250, 380);
    Page1Area->setMinimumHeight(380);
    Page1Area->setMaximumHeight(QWIDGETSIZE_MAX);
    Page1Area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    Page1Layout->addWidget(customPlot);

    //绘图部分
    /* 绘图初始化 */
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    customPlot->setBackground(Qt::transparent);
    customPlot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(10);
    customPlot->legend->setFont(legendFont);
    customPlot->legend->setSelectedFont(legendFont);
    customPlot->legend->setSelectableParts(QCPLegend::spItems);
    customPlot->yAxis->setLabel("Magnitude");
    customPlot->xAxis->setLabel("Sample");
    customPlot->clearGraphs();

    // 页面2
    ElaScrollPageArea* Page2Area = new ElaScrollPageArea();
    QVBoxLayout* Page2Layout = new QVBoxLayout(Page2Area);
    Page2Area->setMinimumHeight(380);
    Page2Area->setMaximumHeight(QWIDGETSIZE_MAX);
    Page2Area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    Page2Layout->addWidget(RobotWidget::instance());

    // 页面3
    ElaScrollPageArea* Page3Area = new ElaScrollPageArea();
    QVBoxLayout* Page3Layout = new QVBoxLayout(Page3Area);
    Page3Area->setMinimumHeight(380);
    Page3Area->setMaximumHeight(QWIDGETSIZE_MAX);
    Page3Area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    Page3Layout->addWidget(Robot);

    // 页面4
    ElaScrollPageArea* Page4Area = new ElaScrollPageArea();
    QVBoxLayout* Page4Layout = new QVBoxLayout(Page3Area);
    Page4Area->setMinimumHeight(380);
    Page4Area->setMaximumHeight(QWIDGETSIZE_MAX);
    Page4Area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//    Page4Layout->addWidget(lable);
    _tabWidget->addTab(Page1Area, "新标签页1");
    _tabWidget->addTab(Page2Area, "新标签页2");
    _tabWidget->addTab(Page3Area, "新标签页3");
    _tabWidget->addTab(Page4Area, "新标签页4");

    // 接收窗口
    ElaToolBar* toolBar = new ElaToolBar("工具栏", this);
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    toolBar->setToolBarSpacing(3);
    toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    toolBar->setIconSize(QSize(25, 25));
    toolButton1 = new ElaToolButton(this);
    toolButton1->setElaIcon(ElaIconType::InputText);
    connect(toolButton1, &ElaToolButton::clicked, this, [=]() {
        isContent = !isContent;
        isContent ? SetTextForm() : SetHexForm();
    });
    toolButton1ToolTip = new ElaToolTip(toolButton1);
    toolButton1ToolTip->setToolTip("显示方式:字符串");
    toolButton2 = new ElaToolButton(this);
    toolButton2->setElaIcon(ElaIconType::ArrowsToEye);
    connect(toolButton2, &ElaToolButton::clicked, this, [=]() {
        customPlot->rescaleAxes(false);
    });
    toolButton2ToolTip = new ElaToolTip(toolButton2);
    toolButton2ToolTip->setToolTip("自动");
    toolButton3 = new ElaToolButton(this);
    toolButton3->setElaIcon(ElaIconType::ArrowRotateRight);
    connect(toolButton3, &ElaToolButton::clicked, this, [=]() {
        ScanSerialName(_comboBox1);
    });
    toolButton3ToolTip = new ElaToolTip(toolButton3);
    toolButton3ToolTip->setToolTip("刷新");
    toolButton4 = new ElaToolButton(this);
    toolButton4->setElaIcon(ElaIconType::PaperPlane);
    connect(toolButton4, &ElaToolButton::clicked, this, [=]() {
        createAutoDebugTabWidget();
    });
    toolButton4ToolTip = new ElaToolTip(toolButton4);
    toolButton4ToolTip->setToolTip("调试");
    toolBar->addWidget(toolButton1);
    toolBar->addWidget(toolButton2);
    toolBar->addWidget(toolButton3);
    toolBar->addWidget(toolButton4);
    receiveedit = new ElaPlainTextEdit(this);
    receiveedit->setReadOnly(true);
    receiveedit->setMinimumHeight(180);
    receiveedit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // 发送窗口
    ElaToolBar* SendWidget = new ElaToolBar(this);
    ElaPlainTextEdit* sendedit = new ElaPlainTextEdit(this);
    sendedit->setFixedHeight(40);
    SendButton = new ElaToolButton(this);
    SendButton->setElaIcon(ElaIconType::ArrowTurnDownLeft);
    connect(SendButton, &ElaToolButton::clicked, this, [=]() {
        SendData(sendedit->toPlainText().toUtf8());
    });
    SendButtonToolTip = new ElaToolTip(SendButton);
    SendButtonToolTip->setToolTip("发送");
    SendWidget->addWidget(sendedit);
    SendWidget->addWidget(SendButton);

    ElaScrollPageArea* EditToolBarArea = new ElaScrollPageArea(this);
    EditToolBarArea->setFixedHeight(300);
    QVBoxLayout* EditToolBar = new QVBoxLayout(EditToolBarArea);
    EditToolBar->addWidget(toolBar);
    EditToolBar->addWidget(receiveedit);
    EditToolBar->addWidget(SendWidget);

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setWindowTitle("协议与连接");
    centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* centerLayout = new QVBoxLayout(centralWidget);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->addWidget(SerialToolBarArea);
    centerLayout->addWidget(_tabWidget);
    centerLayout->addWidget(EditToolBarArea);
    addCentralWidget(centralWidget, true, true, 0);
}

void T_Serial::StartSerial()
{
    _comboBox->setDisabled(true);
    _comboBox1->setDisabled(true);
    _comboBox2->setDisabled(true);
    _comboBox3->setDisabled(true);
    _comboBox4->setDisabled(true);
    _comboBox5->setDisabled(true);
    _comboBox6->setDisabled(true);
    QMutexLocker lock(&m_mutex);
    m_serial->setPortName(_comboBox1->currentText());
    m_serial->setBaudRate(_comboBox2->currentText().toInt());
    switch (_comboBox3->currentIndex()) {
    case 1:
        m_serial->setFlowControl(QSerialPort::HardwareControl);
        break;
    case 2:
        m_serial->setFlowControl(QSerialPort::SoftwareControl);
        break;
    default:
        m_serial->setFlowControl(QSerialPort::NoFlowControl);
        break;
    }
    switch (_comboBox4->currentIndex()) {
    case 1:
        m_serial->setParity(QSerialPort::EvenParity);
        break;
    case 2:
        m_serial->setParity(QSerialPort::OddParity);
        break;
    case 3:
        m_serial->setParity(QSerialPort::SpaceParity);
        break;
    case 4:
        m_serial->setParity(QSerialPort::MarkParity);
        break;
    default:
        m_serial->setParity(QSerialPort::NoParity);
        break;
    }
    switch (_comboBox5->currentIndex()) {
    case 0:
        m_serial->setDataBits(QSerialPort::Data8);
        break;
    case 1:
        m_serial->setDataBits(QSerialPort::Data7);
        break;
    case 2:
        m_serial->setDataBits(QSerialPort::Data6);
        break;
    case 3:
        m_serial->setDataBits(QSerialPort::Data5);
        break;
    default:
        m_serial->setDataBits(QSerialPort::UnknownDataBits);
        break;
    }
    switch (_comboBox6->currentIndex()) {
    case 0:
        m_serial->setStopBits(QSerialPort::OneStop);
        break;
    case 1:
        m_serial->setStopBits(QSerialPort::OneAndHalfStop);
        break;
    case 2:
        m_serial->setStopBits(QSerialPort::TwoStop);
        break;
    default:
        m_serial->setStopBits(QSerialPort::UnknownStopBits);
        break;
    }
    m_serial->open(QIODevice::ReadWrite);
    if(m_serial->isOpen())
    {
        connect(m_serial.data(), &QSerialPort::readyRead, this, &T_Serial::ReadData);
    }
}

void T_Serial::CloseSerial()
{
    _comboBox->setDisabled(false);
    _comboBox1->setDisabled(false);
    _comboBox2->setDisabled(false);
    _comboBox3->setDisabled(false);
    _comboBox4->setDisabled(false);
    _comboBox5->setDisabled(false);
    _comboBox6->setDisabled(false);
    if(m_serial->isOpen())
    {
        m_serial->clear();
        m_serial->close();
    }
}

void T_Serial::SetTextForm()
{
    toolButton1->setElaIcon(ElaIconType::InputText);
    toolButton1ToolTip->setToolTip("显示方式:字符串");
}

void T_Serial::SetHexForm()
{
    toolButton1->setElaIcon(ElaIconType::Binary);
    toolButton1ToolTip->setToolTip("显示方式:十六进制");
}

void T_Serial::SendData(const QByteArray &SendData)
{
    m_serial->write(SendData);
}

void T_Serial::ReadData()
{
    // 写入缓冲区的数据读写（生产者-消费者问题）
    const QByteArray data = m_serial->readAll();
    m_buffer.write(data.constData(), data.size());

    // 不断检查环形缓冲区是否有完整的数据包（以 '\n' 分隔）
    while (true) {
       // 1. 检查是否有数据
       int availableBytes = m_buffer.availableReadData();
       if (availableBytes <= 0)
           break;  // 没有数据，退出循环

       // 2. 临时读取数据到缓冲区（不移动读指针）
       char tempBuffer[1024];
       int bytesRead = m_buffer.peek(tempBuffer, sizeof(tempBuffer));

       // 3. 查找分隔符 '\n'
       QByteArray dataReceived(tempBuffer, bytesRead);
       int delimiterPos = dataReceived.indexOf('\n');

       if (delimiterPos != -1) {
           // 4. 提取完整数据包（不包括 '\n'）
           QByteArray completePacket = dataReceived.left(delimiterPos);
           m_buffer.skip(delimiterPos + 1);  // 移动读指针（跳过分隔符）

           // 5. 处理数据包
           QString strPacket = QString::fromUtf8(completePacket);
           receiveedit->insertPlainText(strPacket+'\n');

           QStringList parts = strPacket.split(",");
           if(parts.size() != customPlot->graphCount()){
               customPlot->clearGraphs();
               receiveedit->clear();
               InitFlag = false;
           }
           if(!InitFlag){
               InitFlag = true;
               for(int i=0; i<parts.size(); i++){
                   customPlot->addGraph();
                   if(i >= InitColor.size()){
                       InitColor.append(QColor(QRandomGenerator::global()->bounded(256),
                                               QRandomGenerator::global()->bounded(256),
                                               QRandomGenerator::global()->bounded(256)));
                   }
                   customPlot->graph(i)->setPen(QPen(InitColor[i]));
               }
           }
           x += 0.1;
           for(int i=0; i<parts.size(); i++){
               double num = parts.at(i).toDouble();
               customPlot->graph(i)->addData(x, num);
           }
           customPlot->replot();//重绘
       } else {
           // 7. 没有找到分隔符，等待下次读取
           break;
       }
    }
}
