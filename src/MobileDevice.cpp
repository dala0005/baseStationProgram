#include "MobileDevice.h"
#include <iostream>
#include <string>

using namespace std;

MobileDevice::MobileDevice(std::string imei)
{
    this->imei = imei;
}

void MobileDevice::call(std::string destPhoneNumber)
{
    bss->connect(imei, destPhoneNumber);
}

void MobileDevice::hangUp()
{
    bss->disconnect(imei);
}

void MobileDevice::getImei(std::string &getImei)
{
    getImei = imei;
}

void MobileDevice::getStatus(std::string &getStatus)
{
    bss->getDeviceStatus(imei, getStatus);
}

void MobileDevice::printCallHistory()
{
    bss->printCallHistory(imei);
}

MobileDevice::~MobileDevice()
{

}

