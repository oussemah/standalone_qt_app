#ifndef GPIO_SETTINGS_H
#define GPIO_SETTINGS_H

#include <QFrame>

#include "qgpiohandler.h"

namespace Ui {
class gpio_settings;
}

class gpio_settings : public QFrame
{
    Q_OBJECT

public:
    explicit gpio_settings(QWidget *parent = 0);
    ~gpio_settings();

private slots:
    void on_button_output_clicked();
    void updatePinValue(int pin);

private:
    Ui::gpio_settings *ui;
    int switch_status;

    QGPIOHandler *m_gpio_handler;
    int m_output_pin;
};

#endif // GPIO_SETTINGS_H
