#include "wirelessconnectdialog.h"
#include "ui_wirelessconnectdialog.h"

WirelessConnectDialog::WirelessConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WirelessConnectDialog) {
    ui->setupUi(this);
    this->ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(false);
}
WirelessConnectDialog::~WirelessConnectDialog() {
    delete ui;
}

bool WirelessConnectDialog::validate_ip(const QString &ip) {
    if (ip.count('.') == 3) {
        QStringList ip_octets = ip.split(".");
        for (QString ip_octet: ip_octets) {
            bool flag;
            int ip_octet_int = ip_octet.toInt(&flag);
            if (!flag) {
                return false;
            } else {
                if (!(ip_octet_int > 0 && ip_octet_int < 256))
                    return false;
            }
        }
        return true;
    }
    return false;
}
bool WirelessConnectDialog::validate_port(const QString &port) {
    bool flag;
    int port_int = port.toInt(&flag);
    if (flag) {
        if (port_int > 0 && port_int < 65537) {
            return true;
        }
    }
    return false;
}

void WirelessConnectDialog::on_button_box_accepted() {
    QString ip_address = this->ui->line_edit_ip->text();
    QString port = this->ui->line_edit_port->text();
    this->device_address.set_ip_address(ip_address);
    this->device_address.set_port(port);
}
void WirelessConnectDialog::on_button_box_rejected() {

}
void WirelessConnectDialog::on_line_edit_ip_textChanged(const QString &ip) {
    QString port = this->ui->line_edit_port->text();
//    qDebug() << "Validating";
//    qDebug() << "Address " << ip << ":" << port;
//    qDebug() << "IP :  " << (validate_ip(ip) ? "Valid" : "Invalid");
//    qDebug() << "PORT :  " << (validate_port(port) ? "Valid" : "Invalid");

    if (this->validate_ip(ip) && this->validate_port(port)) {
        this->ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        this->ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
void WirelessConnectDialog::on_line_edit_port_textChanged(const QString &port) {
    QString ip = this->ui->line_edit_ip->text();
//    qDebug() << "Validating";
//    qDebug() << "Address " << ip << ":" << port;
//    qDebug() << "IP :  " << (validate_ip(ip) ? "Valid" : "Invalid");
//    qDebug() << "PORT :  " << (validate_port(port) ? "Valid" : "Invalid");
    if (this->validate_ip(ip) && this->validate_port(port)) {
        this->ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        this->ui->button_box->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}
DataStructures::WirelessDeviceInfo WirelessConnectDialog::get_wireless_device_info() {
    return this->device_address;
}
