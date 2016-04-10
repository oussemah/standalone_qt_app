#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "menu_gpio.h"
#include "gpio_settings.h"
#include "network_setting.h"
#include "VirtualKeyboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void update_page(int);
    void hide_keyboard(void);
    void show_keyboard(QWidget*);

private:
    Ui::MainWindow *ui;

    QWidget *centralWidget;
    Menu_Gpio *menu_frame;
    QFrame *central_frame;
    QFrame *backup_frame;
};

#endif // MAINWINDOW_H
