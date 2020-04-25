#include "deviceinfo.h"

QString DataStructures::DeviceInfo::toString() {
    return (this->device_serial + "\t" + this->device_name);
}
