#ifndef T_SERIAL_H
#define T_SERIAL_H

#include "Base/T_BasePage.h"
#include <QComboBox>
#include <QSerialPort>
#include <QMutex>
#include <QSharedPointer>  // Qt 的智能指针
#include "Base/qcustomplot.h"   // QCustomPlot 的主头文件
#include "T_RingBuffer.h"
#include "robotwidget.h"
class ElaToggleButton;
class ElaComboBox;
class ElaTabWidget;
class ElaPlainTextEdit;
class ElaToolButton;
class ElaToolTip;
class RobotWidget;

class T_Serial : public T_BasePage
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit T_Serial(QWidget* parent = nullptr);
    ~T_Serial();
    void ScanSerialName(QComboBox* device);
    void init_Page();
    void showTracer(bool show);

private:
    bool isSwitchOn{false};
    bool isContent{true};
    QScopedPointer<QSerialPort> m_serial{new QSerialPort};
    QCustomPlot* customPlot{new QCustomPlot()};
    T_RingBuffer m_buffer{1024*1024};
    QMutex m_mutex;
    qint64 m_lastActiveTime = 0;
    ElaToggleButton* _toggleButton{nullptr};  // 开关
    ElaComboBox* _comboBox{nullptr};    // 数据接口
    ElaComboBox* _comboBox1{nullptr};   // 端口号
    ElaComboBox* _comboBox2{nullptr};   // 波特率
    ElaComboBox* _comboBox3{nullptr};   // 数据流控
    ElaComboBox* _comboBox4{nullptr};   // 校验位
    ElaComboBox* _comboBox5{nullptr};   // 数据位数
    ElaComboBox* _comboBox6{nullptr};   // 停止位数
    ElaTabWidget* _tabWidget{nullptr};  // 视图窗口
    ElaPlainTextEdit* receiveedit{nullptr};    //接收窗口
    ElaToolButton* toolButton1{nullptr};
    ElaToolButton* toolButton2{nullptr};
    ElaToolButton* toolButton3{nullptr};
    ElaToolButton* toolButton4{nullptr};
    ElaToolButton* SendButton{nullptr};
    ElaToolTip* toolButton1ToolTip{nullptr};
    ElaToolTip* toolButton2ToolTip{nullptr};
    ElaToolTip* toolButton3ToolTip{nullptr};
    ElaToolTip* toolButton4ToolTip{nullptr};
    ElaToolTip* SendButtonToolTip{nullptr};
    QVector<QColor> InitColor = {QColor(255, 0, 127), QColor(0, 162, 0), QColor(0, 0, 255)};
    bool InitFlag{false};
    float x{0.0};
signals:
    void packetReady(QByteArray packet);
    void errorOccurred(QString err);

public slots:
    void StartSerial();
    void CloseSerial();
    void SetTextForm();
    void SetHexForm();

private slots:
    void SendData(const QByteArray &SendData);
    void ReadData();
};

#endif // T_SERIAL_H
