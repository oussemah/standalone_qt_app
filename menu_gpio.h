#ifndef MENU_GPIO_H
#define MENU_GPIO_H

#include <QFrame>

namespace Ui {
class Menu_Gpio;
}

class Menu_Gpio : public QFrame
{
    Q_OBJECT

public:
    explicit Menu_Gpio(QWidget *parent = 0);
    ~Menu_Gpio(void);

signals:
    void request_exit(void);
    void switch_central_widget(int widget_id);

private slots:
    void on_button_Exit_p1_clicked(void);

    void on_button_Settings_clicked();

private:
    Ui::Menu_Gpio *ui;
    int menu_type;
};

#endif // MENU_GPIO_H
