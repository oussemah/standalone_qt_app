#include <QVBoxLayout>
#include <QTextStream>

#include "mainwindow.h"

#define GPIO_SETTINGS 0
#define NETWORK_SETTINGS 1


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setStyleSheet("QMainWindow {background: url(:/images/background.png) }");
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    setFixedSize(800,480);

    centralWidget = new QWidget(this);
    centralWidget->setBaseSize(800,480);

    menu_frame = new Menu_Gpio(centralWidget);
    central_frame = new gpio_settings(centralWidget);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(central_frame);
    layout->addWidget(menu_frame);
    centralWidget->setLayout(layout);

    this->setCentralWidget(centralWidget);

    connect( menu_frame, SIGNAL(request_exit()), this, SLOT(close()));
    connect( menu_frame, SIGNAL(switch_central_widget(int)), this, SLOT(update_page(int)));

    centralWidget->show();

}

MainWindow::~MainWindow()
{
    delete centralWidget;
}

void MainWindow::update_page(int page_type)
{
    centralWidget->layout()->removeWidget(menu_frame);
    centralWidget->layout()->removeWidget(central_frame);
    delete central_frame;
    if (page_type == GPIO_SETTINGS) {
        central_frame = new gpio_settings(centralWidget);
    } else {
        central_frame = new network_setting(centralWidget);
        connect (central_frame, SIGNAL(requestInput(QWidget*)), this, SLOT(show_keyboard(QWidget*)));
    }
    centralWidget->layout()->addWidget(central_frame);
    centralWidget->layout()->addWidget(menu_frame);
    centralWidget->show();
}

void MainWindow::show_keyboard(QWidget* input)
{
    QTextStream out(stdout);
    centralWidget->layout()->removeWidget(menu_frame);
    centralWidget->layout()->removeWidget(central_frame);
    central_frame->hide();
    backup_frame = central_frame;
    central_frame = new VirtualKeyboard();
    ((VirtualKeyboard*)central_frame)->initialisation(input,"EN", true);
    connect ((VirtualKeyboard*)central_frame, SIGNAL(hideMe()), this, SLOT(hide_keyboard()));
    central_frame->show();

}

void MainWindow::hide_keyboard(void)
{
    delete central_frame;
    central_frame = backup_frame;
    central_frame->show();
    centralWidget->layout()->addWidget(central_frame);
    centralWidget->layout()->addWidget(menu_frame);
    //centralWidget->show();
    this->repaint();

}
