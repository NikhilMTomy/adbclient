#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include <QString>

namespace DataStructures {
    class DeviceInfo {
    public:
        QString device_serial;
        QString device_name;
        QString toString();
    };
}

#endif // DEVICEINFO_H
