/*
 *
 * adbfunctions.cpp
 *
 * Contains all the functions that deals with `adb.exe` program
 * and all the support functions required for the above operations
 *
 */

#include "adbfunctions.h"

ADBFunctions::ADBFunctions() {

}
ADBFunctions::~ADBFunctions() {
    emit finished();
}
void ADBFunctions::set_device_serial(QString serial) {
    this->adb_device_serial = serial;
}

QString ADBFunctions::get_device_serial() {
    return this->adb_device_serial;
}

/**
 * @brief  Split input_string to QStringList based on delimiters
 *
 * @param  input_string QString which is to be split
 * @param  delimiters   QStringList of delimiters used to split `input_string`
 * @return QStringList which contains split strings of `input_string` based on `delimiters`
 */
QStringList ADBFunctions::split_to_list(QString input_string, QStringList delimiters) {
    QStringList list;
    QString current_line = "";
    for (int i=0; i<input_string.length(); ++i) {
        if (delimiters.contains(QString(input_string[i]))) {
            if (current_line.isNull() || current_line.isEmpty()) {
                current_line = "";
                continue;
            } else {
                list.append(current_line);
                current_line = "";
            }
        } else {
            current_line.append(input_string[i]);
        }
    }
    if (!(current_line.isNull() || current_line.isEmpty())) {
        list.append(current_line);
    }
    return list;
}

DataStructures::DeviceInfo ADBFunctions::get_device_info() {
    return this->device_info;
}
void ADBFunctions::set_device_info(const DataStructures::DeviceInfo &device_info) {
    this->device_info = device_info;
}
DataStructures::WirelessDeviceInfo ADBFunctions::get_wireless_device_info() {
    return this->wireless_device_info;
}
void ADBFunctions::set_wireless_device_info(const DataStructures::WirelessDeviceInfo &wireless_device_info) {
    this->wireless_device_info = wireless_device_info;
}
/**
 * @brief  connect to specified device
 */
void ADBFunctions::connect_to_device() {
    QProcess adb_process;
    QStringList arguments;

    arguments.append("-s");
    arguments.append(device_info.device_serial);
    arguments.append("shell");
    arguments.append("exec");
    arguments.append("ls");
    //arguments.append("");

    adb_process.setProgram("adb.exe");
    adb_process.setArguments(arguments);
    adb_process.start();
    adb_process.waitForFinished();

    QString error = adb_process.readAllStandardError();
    QString output = adb_process.readAllStandardOutput();

    if (adb_process.exitStatus() == QProcess::NormalExit) {
        this->set_device_serial(device_info.device_serial);
        this->connection_mode = CONNECTION_MODE_WIRED;
        emit finished_connecting_to_device(adb_process.exitCode());
    } else {
        emit finished_connecting_to_device(-1);
    }
}
/**
 * @brief  connect to specified device over WiFi
 */
void ADBFunctions::connect_to_wireless_device() {
    QProcess adb_process;
    QStringList arguments;
    arguments.append(QString("connect"));
    arguments.append(wireless_device_info.toString());

    adb_process.setProgram("adb");
    adb_process.setArguments(arguments);
    adb_process.start();
    adb_process.waitForFinished();

    QString error = adb_process.readAllStandardError();
    QString output = adb_process.readAllStandardOutput();

    if (adb_process.exitStatus() == QProcess::NormalExit) {
        this->set_device_serial(wireless_device_info.toString());
        this->connection_mode = CONNECTION_MODE_WIRELESS;
        emit finished_connecting_to_device(adb_process.exitCode());
    } else {
        emit finished_connecting_to_device(-1);
    }
}
/**
 * @brief  lists all the android devices connected to the system
 *
 * @return QMap of serial and name of all devices connected to the system
 */
void ADBFunctions::get_connected_devices() {
    QProcess adb_process;
    adb_process.setProgram("adb");
    adb_process.setArguments(QStringList() << "devices");
    adb_process.start();
    adb_process.waitForFinished();

    QString error = adb_process.readAllStandardError();
    QString output = adb_process.readAllStandardOutput();

    QStringList output_lines = this->split_to_list(output, QStringList() << "\n" << "\r");

    QList<DataStructures::DeviceInfo> connected_devices;
    // iterating from second output line to ignore the header
    for (int i=1; i<output_lines.length(); ++i) {
        QString device_serial, device_name;
        DataStructures::DeviceInfo device_info;
        int j=0;
        for (j=0; output_lines[i][j] != '\t'; ++j) {
            device_serial.append(output_lines[i][j]);
        }
        ++j;
        for (; j<output_lines[i].length(); ++j) {
            device_name.append(output_lines[i][j]);
        }
        device_info.device_name = device_name;
        device_info.device_serial = device_serial;
        connected_devices.append(device_info);
    }
    emit finished_getting_connected_devices(connected_devices);
}
QStringList ADBFunctions::get_storage_directories() {
    return QStringList() << "/mnt/sdcard";
}
void ADBFunctions::set_root_directory(const QString &root_directory) {
    this->root_directory = root_directory;
}
DataStructures::DirectoryItem ADBFunctions::get_directory_item(const QString &current_directory, const QString &directory_item_string) {
    QStringList directory_item_string_contents = split_to_list(directory_item_string, QStringList() << " ");
    DataStructures::DirectoryItemType type;
    if (directory_item_string[0] == "d") {
        type = DataStructures::DirectoryItemType::Directory;
    } else {
        type = DataStructures::DirectoryItemType::File;
    }
    /**
     * 0 - permissions
     * 1 - links
     * 2 - owner
     * 3 - group
     * 4 - size
     * 5 - date
     * 6 - time
     * 7 - name
     */
    return DataStructures::DirectoryItem(
        current_directory,
        directory_item_string_contents[7],
        type,
        directory_item_string_contents[4].toLong()
    );
}
void ADBFunctions::get_directory_contents(const QString &current_directory) {
    QString full_path = this->root_directory + current_directory;
    QList<DataStructures::DirectoryItem> directory_contents;
    QProcess adb_process;
    adb_process.setProgram("adb");
    QStringList arguments;
    arguments.append("-s");
    if (this->connection_mode == CONNECTION_MODE_WIRED)
        arguments.append(device_info.device_serial);
    else
        arguments.append(wireless_device_info.toString());
    arguments.append("shell");
    arguments.append("exec");
    arguments.append("ls");
    arguments.append("-l");
    arguments.append(full_path);

    adb_process.setArguments(arguments);
    adb_process.start();
    adb_process.waitForFinished();

    QString error = adb_process.readAllStandardError();
    QString output = adb_process.readAllStandardOutput();

    QStringList output_lines = this->split_to_list(output, QStringList() << "\n" << "\r");
    // start from 1 to skip total size row
    for (int i=1; i< output_lines.length(); ++i) {
        DataStructures::DirectoryItem directory_item = get_directory_item(current_directory, output_lines[i]);
        directory_contents.append(directory_item);
    }
    //DataStructures::DirectoryItem directory_item("/mnt/sdcard", "sample.txt", DataStructures::DirectoryItemType::File, 4096);
    emit this->finished_getting_directory_content(directory_contents);
}
