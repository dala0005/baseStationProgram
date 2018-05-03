#ifndef MOBILEDEVICE_H
#define MOBILEDEVICE_H
#include "BaseStation.h"
#include <string>

class MobileDevice
{
    public:
        MobileDevice(std::string imei);
        void call(std::string destPhoneNumber);
        void hangUp();
        void getImei(std::string &getImei);
        void getStatus(std::string &getStatus);
        void printCallHistory();
        ~MobileDevice();
    private:
        BaseStation *bss = BaseStation::getInstance();
        std::string imei = "";
};
#endif // MOBILEDEVICE_H
