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

signals:
    void requestInput(QWidget *input);

private slots:
    void on_toolButton_clicked();

    void on_pb_ip_address_clicked();

    void on_pb_gw_ip_clicked();

    void on_pb_netmask_ip_clicked();

private:
    Ui::network_setting *ui;
};

#endif // NETWORK_SETTING_H
