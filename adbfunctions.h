#ifndef ADBFUNCTIONS_H
#define ADBFUNCTIONS_H

#include <QProcess>
#include <QDebug>

#include <deviceinfo.h>
#include <wirelessdeviceinfo.h>
#include <directoryitem.h>

#define CONNECTION_MODE_WIRED    0
#define CONNECTION_MODE_WIRELESS 1

class ADBFunctions: public QObject {
    Q_OBJECT

private:
    int connection_mode;
    QStringList storage_locations; // $EXTERNAL_STORAGE and $SECONDARY_STORAGE
    QString adb_device_serial;
    QStringList split_to_list(QString input_string, QStringList delimiters);
    DataStructures::DeviceInfo device_info;
    DataStructures::WirelessDeviceInfo wireless_device_info;
    QString root_directory;

public:
    ADBFunctions();
    ~ADBFunctions();

    void set_device_serial(QString serial);
    QString get_device_serial();

    DataStructures::DeviceInfo get_device_info();
    void set_device_info(const DataStructures::DeviceInfo &device_info);

    DataStructures::WirelessDeviceInfo get_wireless_device_info();
    void set_wireless_device_info(const DataStructures::WirelessDeviceInfo &device_info);

    void set_root_directory(const QString &root_directory);

    QStringList get_storage_directories();

    DataStructures::DirectoryItem get_directory_item(const QString &current_directory, const QString &directory_item_string);

public slots:
    void get_connected_devices();
    void connect_to_device();
    void connect_to_wireless_device();
    void get_directory_contents(const QString &current_path);

signals:
    void finished_getting_connected_devices(const QList<DataStructures::DeviceInfo> &connected_devices);
    void finished_connecting_to_device(const int &status);
    void finished_connecting_to_wireless_device(const int &status);
    void finished_getting_directory_content(const QList<DataStructures::DirectoryItem> &directory_items);
    void finished();
};

#endif // ADBFUNCTIONS_H
