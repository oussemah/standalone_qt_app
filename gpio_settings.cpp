#include "gpio_settings.h"
#include "ui_gpio_settings.h"

#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"


static const QString switch_icons[] = { ":images/switch_off.png", ":images/switch_on.png"};
static const QString led_icons[]    = { ":images/led_off.png", ":images/led_on.png"};

#define OFF 0
#define ON 1

gpio_settings::gpio_settings(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::gpio_settings)
{
    //Setup Gpio
    m_output_pin = 30;
    m_gpio_handler = new QGPIOHandler;
    m_gpio_handler->reservePin(m_output_pin);
    m_gpio_handler->setPinDirection(m_output_pin, QGPIOHandler::DIRECTION_OUTPUT);
    m_gpio_handler->setPinValue(m_output_pin, QGPIOHandler::VALUE_LOW);

    m_gpio_handler->setPinDirection(31, QGPIOHandler::DIRECTION_INPUT);

    switch_status = OFF;

    setFixedHeight(400);
    ui->setupUi(this);

    connect (m_gpio_handler, SIGNAL(newPinValueReady(int)), this, SLOT(updatePinValue(int)));

    show();
}

gpio_settings::~gpio_settings()
{
    delete ui;
}

void gpio_settings::on_button_output_clicked()
{
    switch_status = 1 - switch_status;
    ui->button_output->setIcon(QIcon(switch_icons[switch_status]));

    //Update GPIO Status
    if (switch_status == OFF) {
        m_gpio_handler->setPinValue(m_output_pin, QGPIOHandler::VALUE_LOW);
    } else {
        m_gpio_handler->setPinValue(m_output_pin, QGPIOHandler::VALUE_HIGH);
    }
}

void gpio_settings::updatePinValue(int pin){
    int new_value = m_gpio_handler->readPinValue(pin);
        ui->label->setPixmap(QPixmap(led_icons[new_value]));
}
