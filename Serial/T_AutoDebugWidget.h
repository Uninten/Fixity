#ifndef T_AUTODEBUGWIDGET_H
#define T_AUTODEBUGWIDGET_H

#include "robotwidget.h"
#include "ElaSlider.h"
#include "ElaRadioButton.h"
#include "ElaToolTip.h"
#include "Base/T_BasePage.h"
#include <QWidget>

class T_AutoDebugWidget : public T_BasePage
{
Q_OBJECT
public:
    explicit T_AutoDebugWidget(QWidget* parent = nullptr);
    ~T_AutoDebugWidget();

private:
    RobotWidget* robot{nullptr};
    QList<STLFileLoader*> robotModels{};
    ElaRadioButton* _radioButton1{nullptr};
    ElaRadioButton* _radioButton2{nullptr};
};

#endif // T_AUTODEBUGWIDGET_H
