#include <iostream>
#include <vector>
#include "include/BaseStation.h"
#include "include/MobileDevice.h"
#include "mainFunctions.h"
using namespace std;


int main()
{
    enum commands {QUIT, CALL, HANG_UP, CALL_HISTORY};

    vector<string> mobileDevicesIMEI;
    vector<MobileDevice *> mobileDevices;
    unsigned short int getDevice = 0;
    unsigned short int getCommand = 0;
    string recieversNumber = "";
    bool quit = false;

    readFile("/home/daniel/jobb/baseStationProgram/mobile_devices_imei.txt", mobileDevicesIMEI);
    createMobileDevices(&mobileDevices, mobileDevicesIMEI);


    while(!quit)
    {
        printDeviceList(&mobileDevices);
        printCommandList();
        selectDevice(getDevice);
        selectCommand(getCommand);

        switch(getCommand)
        {

                /*  QUIT */
            case QUIT:
                quit = true;
            break;


                /* CALL */
            case CALL:
                cout << "Enter recievers telephone number: ";
                cin >> recieversNumber;
                mobileDevices.at(getDevice)->call(recieversNumber);
            break;


                /* HANG UP */
            case HANG_UP:
                mobileDevices.at(getDevice)->hangUp();
            break;

                /* CALL HISTORY */
            case CALL_HISTORY:
                mobileDevices.at(getDevice)->printCallHistory();
            break;
        }
    }

    cleanUpMobileDevices(&mobileDevices);

    cout << "\n** Have a nice day! **\n\n";
    return 0;
}
