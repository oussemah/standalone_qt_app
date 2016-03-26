#include "menu_gpio.h"
#include "ui_menu_gpio.h"

#define GPIO_SETTINGS 0
#define NETWORK_SETTINGS 1

static const QString icons[] = { ":images/gpio.png", ":images/network.png"};
static const QString toolTips[] = { "GPIO Settings", "Network Settings"};
Menu_Gpio::Menu_Gpio(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::Menu_Gpio)
{
    setFixedHeight(80);
    setStyleSheet("QFrame {border-style: outset;border-width: 10px;border-radius:10px;}");
    ui->setupUi(this);

    menu_type = NETWORK_SETTINGS;

    show();
}

Menu_Gpio::~Menu_Gpio(void)
{
    delete ui;
}

void Menu_Gpio::on_button_Exit_p1_clicked(void)
{
    emit request_exit();
}


void Menu_Gpio::on_button_Settings_clicked()
{
    menu_type = 1 - menu_type;
    ui->button_Settings->setIcon(QIcon(icons[menu_type]));
    ui->button_Settings->setToolTip(toolTips[menu_type]);
    emit(switch_central_widget(1-menu_type));
}
