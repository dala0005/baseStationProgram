#ifndef BASESTATION_H
#define BASESTATION_H
#include <iostream>
#include "CallTime.h"
#include "Database.h"
#include <vector>


class BaseStation
{
    static BaseStation* _instance;

    BaseStation();
    ~BaseStation();
    CallTime *calltime;
    const std::string databasePath = "/home/daniel/jobb/baseStationProgram/database/baseStation.db";
    Database *database = 0;
    std::string currentTime;
    std::string sqlResult = "";
    std::string sqlString = "";


public:
    static BaseStation* getInstance() {
        if(_instance == NULL)
            _instance = new BaseStation();
        return _instance;
    }

    int connect(std::string srcIMEI, std::string destPhoneNumber);
    int disconnect(std::string srcIMEI);
    void getDeviceStatus(std::string srcIMEI, std::string &srcRes);
    void printCallHistory(std::string srcIMEI);
};


#endif // BASESTATION_H
