#include "wirelessdeviceinfo.h"

DataStructures::WirelessDeviceInfo::WirelessDeviceInfo() {

}
QString DataStructures::WirelessDeviceInfo::get_ip_address() {
    return this->ip_address;
}
bool DataStructures::WirelessDeviceInfo::set_ip_address(QString ip_address) {
    this->ip_address = ip_address;
    return true;
}
QString DataStructures::WirelessDeviceInfo::get_port() {
    return this->port;
}
bool DataStructures::WirelessDeviceInfo::set_port(QString port) {
    this->port = port;
    return true;
}

QString DataStructures::WirelessDeviceInfo::toString() {
    QString device_info = this->ip_address + ":" + this->port;
    return device_info;
}
