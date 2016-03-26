#ifndef NETWORK_SETTING_H
#define NETWORK_SETTING_H

#include <QFrame>

namespace Ui {
class network_setting;
}

class network_setting : public QFrame
{
    Q_OBJECT

public:
    explicit network_setting(QWidget *parent = 0);
    ~network_setting();

    void loadSettings(void);
    void saveSettings(void);

private slots:
    void on_toolButton_clicked();

private:
    Ui::network_setting *ui;
};

#endif // NETWORK_SETTING_H
