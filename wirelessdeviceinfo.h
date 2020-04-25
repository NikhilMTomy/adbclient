#ifndef WIRELESSDEVICEINFO_H
#define WIRELESSDEVICEINFO_H

#include <QString>

namespace DataStructures {
    class WirelessDeviceInfo {
    private:
        QString ip_address;
        QString port;
    public:
        WirelessDeviceInfo();

        QString get_ip_address();
        bool set_ip_address(QString ip_address);

        QString get_port();
        bool set_port(QString ip_address);

        QString toString();
    };
}

#endif // WIRELESSDEVICEINFO_H
