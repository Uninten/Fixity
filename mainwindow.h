#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ElaWindow.h"

class T_Home;
class T_Setting;
class T_Graphics;
class T_Navigation;
class T_Serial;
class T_Note;

class MainWindow : public ElaWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initWindow();
    void initEdgeLayout();
    void initContent();

private:
    T_Home* _homePage{nullptr};
    T_Graphics* _graphicsPage{nullptr};
    T_Serial* _serialPage{nullptr};
    T_Note* _notePage{nullptr};
    T_Setting* _settingPage{nullptr};
    QString _settingKey{""};
};
#endif // MAINWINDOW_H
