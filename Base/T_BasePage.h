#ifndef T_BASEPAGE_H
#define T_BASEPAGE_H

#include <ElaScrollPage.h>
#include <QMainWindow>
#include "ElaColorDialog.h"
#include "mainwindow.h"

class QVBoxLayout;
class T_BasePage : public ElaScrollPage
{
    Q_OBJECT
public:
    T_BasePage(QMainWindow* mainWindow, QWidget* parent = nullptr)
            : ElaScrollPage(parent), m_mainWindow(mainWindow) {}

    explicit T_BasePage(QWidget* parent = nullptr);
    ~T_BasePage() override;

private:
    ElaColorDialog* _colorDialog{nullptr};
    QMainWindow* m_mainWindow;

protected:
    void createCustomWidget(QString desText);
    void createElaTabWidget();
    void createElaDebugTabWidget();
    void createAutoDebugTabWidget();
};

#endif // T_BASEPAGE_H
