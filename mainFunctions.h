#ifndef MAINFUNCTIONS_H
#define MAINFUNCTIONS_H
#include <iostream>
#include <vector>
#include <fstream>
#include "MobileDevice.h"

BaseStation * BaseStation::_instance;

void readFile(const char path[], std::vector<std::string> &mobileDevices);

void createMobileDevices(std::vector<MobileDevice *> *mobileDevices, std::vector<std::string> imei);

void printDeviceList(std::vector<MobileDevice *> *mobileDevices);

void printCommandList();

void selectDevice(unsigned short int &device);

void selectCommand(unsigned short int &command);

void cleanUpMobileDevices(std::vector<MobileDevice *> *mobileDevices);

void readFile(const char path[], std::vector<std::string> &mobileDevices)
{
    std::string line = "";
    std::ifstream readFile;
    readFile.open(path);

    if(readFile.is_open())
    {
        while (!readFile.eof())
        {
             getline(readFile, line);
             if(line != "")
                mobileDevices.push_back(line);
        }
    }
    readFile.close();
}

void createMobileDevices(std::vector<MobileDevice *> *mobileDevices, std::vector<std::string> imei)
{
    std::vector<std::string>::iterator imeiIterator;
    imeiIterator = imei.begin();



    while(imeiIterator != imei.end())
    {
        mobileDevices->push_back(new MobileDevice(*imeiIterator));
        imeiIterator++;
    }
}


void printDeviceList(std::vector<MobileDevice *> *mobileDevices)
{
    std::vector<MobileDevice *>::iterator mbIterator;
    mbIterator = mobileDevices->begin();
    unsigned int deviceIndex = 0;
    std::string getMobileInfo = "";

    std::cout << "***************\n";
    std::cout << "* Device List *\n";
    std::cout << "***************\n\n";

     while(mbIterator != mobileDevices->end())
    {
        std::cout << "Device: " << deviceIndex << ")\n";
        (*mbIterator)->getImei(getMobileInfo);
        std::cout << "\tIMEI: " << getMobileInfo << "\n";
        (*mbIterator)->getStatus(getMobileInfo);
        std::cout << "\tStatus: " << getMobileInfo << "\n";

        deviceIndex++;
        mbIterator++;
    }


}

void printCommandList()
{
    std::cout << "Commands:\n";
    std::cout << "1) Call\n";
    std::cout << "2) Hang up\n";
    std::cout << "3) Call history\n";
    std::cout << "\n0) Quit\n";
}

void selectDevice(unsigned short int &device)
{
    std::cout << "Select Device: ";
    std::cin >> device;
}

void selectCommand(unsigned short int &command)
{
    std::cout << "Select Command: ";
    std::cin >> command;
}


void cleanUpMobileDevices(std::vector<MobileDevice *> *mobileDevices)
{
    std::vector<MobileDevice *>::iterator mbIterator;
    mbIterator = mobileDevices->begin();

    while(mbIterator != mobileDevices->end())
    {
        delete *mbIterator;
        mbIterator++;
    }
}

#endif // MAINFUNCTIONS_H
