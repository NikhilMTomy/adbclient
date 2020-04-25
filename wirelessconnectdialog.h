#ifndef WIRELESSCONNECTDIALOG_H
#define WIRELESSCONNECTDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDebug>

#include <wirelessdeviceinfo.h>

namespace Ui {
    class WirelessConnectDialog;
}

class WirelessConnectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WirelessConnectDialog(QWidget *parent = nullptr);
    ~WirelessConnectDialog();
    DataStructures::WirelessDeviceInfo get_wireless_device_info();

private slots:
    void on_button_box_accepted();
    void on_button_box_rejected();
    void on_line_edit_ip_textChanged(const QString &ip);
    void on_line_edit_port_textChanged(const QString &port);

private:
    Ui::WirelessConnectDialog *ui;
    bool validate_ip(const QString &ip);
    bool validate_port(const QString &port);
    DataStructures::WirelessDeviceInfo device_address;
};

#endif // WIRELESSCONNECTDIALOG_H
