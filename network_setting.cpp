#include "network_setting.h"
#include "ui_network_setting.h"

#include "QFile"
#include "QTextStream"

#define NETWORK_SETTINGS_FILE "/etc/network/interfaces"

network_setting::network_setting(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::network_setting)
{
    ui->setupUi(this);
    loadSettings();
    show();
}

network_setting::~network_setting()
{
    delete ui;
}

void network_setting::loadSettings(void)
{
    bool settings_parsed = false;

    QFile file(NETWORK_SETTINGS_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        ui->ledit_ip_address->setText("Error.Check settings file !");
        ui->ledit_netmask_ip->setText("Error.Check settings file !");
        ui->ledit_gw_ip->setText("Error.Check settings file !");
        return;
    }

    QTextStream in(&file);

    QString line = in.readLine();

    while (!line.isNull() && !settings_parsed) {
        if (line.compare(QString("iface eth0 inet static")) >= 0)
        {
            do {
                line = in.readLine();
            } while (!line.isNull() && !line.trimmed().startsWith("address"));
            if (line.isNull()) {
                break;
            }
            ui->ledit_ip_address->setText(line.trimmed().remove("address"));
            do {
                line = in.readLine();
            } while (!line.isNull() && !line.trimmed().startsWith("netmask"));
            if (line.isNull()) {
                break;
            }
            ui->ledit_netmask_ip->setText(line.trimmed().remove("netmask"));
            do {
                line = in.readLine();
            } while (!line.isNull() && !line.trimmed().startsWith("gateway"));
            if (line.isNull()) {
                break;
            }
            ui->ledit_gw_ip->setText(line.trimmed().remove("gateway"));

            settings_parsed = true;
            file.close();
            return;
        }
        line = in.readLine();
    }
    if (!settings_parsed) {
        ui->ledit_ip_address->setText("Error.Check settings file !");
        ui->ledit_netmask_ip->setText("Error.Check settings file !");
        ui->ledit_gw_ip->setText("Error.Check settings file !");
    }

    return;
}

void network_setting::saveSettings(void)
{
    QFile file(NETWORK_SETTINGS_FILE);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    QTextStream in(&file);

    QString total_file;
    QString line = in.readLine();

    while (!line.isNull()) {
        if (line.compare(QString("iface eth0 inet static")) >= 0)
        {
            do {
                total_file.append(line+"\n");
                line = in.readLine();
            } while (!line.isNull() && !line.trimmed().startsWith("address"));
            if (line.isNull()) {
                total_file.append( "\taddress "+ui->ledit_ip_address->text());
                total_file.append( "\tnetmask "+ui->ledit_netmask_ip->text());
                total_file.append( "\tgateway "+ui->ledit_gw_ip->text());
                break;

            }
            total_file.append( "\taddress "+ui->ledit_ip_address->text());
            line="";
            do {
                total_file.append(line+"\n");
                line = in.readLine();
            } while (!line.isNull() && !line.trimmed().startsWith("netmask"));
            if (line.isNull()) {
                total_file.append( "\tnetmask "+ui->ledit_netmask_ip->text());
                total_file.append( "\tgateway "+ui->ledit_gw_ip->text());
                break;
            }
            total_file.append( "\tnetmask "+ui->ledit_netmask_ip->text());
            line="";
            do {
                total_file.append(line+"\n");
                line = in.readLine();
            } while (!line.isNull() && !line.trimmed().startsWith("gateway"));
            total_file.append( "\tgateway "+ui->ledit_gw_ip->text());
            if (!line.isNull())
               line="";
            break;
        }
        total_file.append(line+"\n");
        line = in.readLine();
    }
    while (!line.isNull()) {
            total_file.append(line+"\n");
            line = in.readLine();
    }

    file.close();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return;
    }
    QTextStream out(&file);
    out << total_file;
    file.close();
    return;
}

void network_setting::on_toolButton_clicked()
{
    saveSettings();
}
