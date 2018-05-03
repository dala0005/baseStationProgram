#include "BaseStation.h"
#include <iostream>
#include "CallTime.h"
#include "Database.h"
BaseStation::BaseStation()
{
    database = new Database(databasePath);
    calltime = new CallTime();
}


int BaseStation::connect(std::string srcIMEI, std::string destPhoneNumber)
{
    unsigned short int deviceOK = 1;
    std::string srcPhoneNumber = "";

    /*
    *   whitelisted
    *   greylisted
    *   blacklisted
    */
    /* Check if device can make a call */
    sqlString = "select access_type from Accesses, Devices where imei == \"" + srcIMEI + "\" and access = Accesses.id";
    database->sqlSelect(sqlString.c_str(), &sqlResult);
    if(sqlResult.compare("whitelisted") != 0)
    {
        deviceOK = 0;
    }



    /* check if the owner tries to call its own number */
    if(deviceOK)
    {
        sqlString = "select device_imei from Phone_numbers where \"" + destPhoneNumber + "\" == Phone_numbers.phone_number";
        database->sqlSelect(sqlString.c_str(), &sqlResult);
        if(sqlResult.compare(srcIMEI) == 0)
        {
            deviceOK = 0;
        }
        srcPhoneNumber = sqlResult;
    }

    /* check if the device is not already connected */
    if(deviceOK)
    {
        sqlString = "select status_type from Status, Devices where Devices.imei == \"" + srcIMEI + "\" and Devices.device_status == Status.id";
        database->sqlSelect(sqlString.c_str(), &sqlResult);
        if(sqlResult.compare("disconnected") != 0)
        {
            deviceOK = 0;
        }
    }

    /* check if reciever is not already connected */
    if(deviceOK)
    {
        sqlString = "select Status.status_type from Status, Devices, Phone_numbers where \"" + destPhoneNumber + "\" == Phone_numbers.phone_number and Phone_numbers.device_imei == Devices.imei and Devices.device_status == Status.id";
        database->sqlSelect(sqlString.c_str(), &sqlResult);
        if(sqlResult.compare("disconnected") != 0)
        {
            deviceOK = 0;
        }
    }

    /* connect the two devices to each others */
    if(deviceOK)
    {
        /* connect own device */
        sqlString = "update Devices set device_status = 1 where \"" + srcIMEI + "\" == imei";
        database->sqlUpdate(sqlString.c_str());

        /* connect recievers device */
        sqlString = "update Devices set device_status = 1 where imei IN (select device_imei from Phone_numbers where \"" + destPhoneNumber + "\" == phone_number)";
        database->sqlUpdate(sqlString.c_str());


        /* get dailing devices phone number */
        sqlString = "select phone_number from Phone_numbers where \"" + srcIMEI + "\" == device_imei";
        database->sqlSelect(sqlString.c_str(), &sqlResult);
        srcPhoneNumber = sqlResult;

        /*
        * start call on the dailing device
        */

        /* get number of rows in Calls table */
        sqlString = "select count(id) from Calls";
        database->sqlSelect(sqlString.c_str(), &sqlResult);


        /* get current time */
        calltime->getTime(currentTime);


        /* insert call information on dailed device */
        sqlString = "insert into Calls (id, phone_number, start_time, call_type, phone_number_reciever) values (" + sqlResult + "+1, \"" + srcPhoneNumber + "\", \"" + currentTime +  "\",  0, \"" + destPhoneNumber + "\")";
        database->sqlInsert(sqlString.c_str());

        /*
        * start call on the reciver device
        */

        /* get number of rows in Calls table */
        sqlString = "select count(id) from Calls";
        database->sqlSelect(sqlString.c_str(), &sqlResult);


        /* insert call information on reciviers device */
        sqlString = "insert into Calls (id, phone_number, start_time, call_type, phone_number_reciever) values (" + sqlResult + "+1, \"" + destPhoneNumber + "\", \"" + currentTime +  "\",  1, \"" + srcPhoneNumber + "\")";
        database->sqlInsert(sqlString.c_str());
    }

    return 0;
}

int BaseStation::disconnect(std::string srcIMEI)
{
    std::string destIMEI = "";
    std::string destPhoneNumber = "";

    /* check if source device is connected */
    sqlString = "select status_type from Status, Devices where device_status == Status.id and imei == \"" + srcIMEI + "\"";
    database->sqlSelect(sqlString.c_str(), &sqlResult);
    if(sqlResult.compare("connected") == 0)
    {

        /* get destinations phone number */
        sqlString = "select phone_number_reciever from Calls inner join Phone_numbers on Calls.phone_number = Phone_numbers.phone_number and Phone_numbers.device_imei == \"" + srcIMEI + "\" and Calls.end_time is null";
        database->sqlSelect(sqlString.c_str(), &sqlResult);
        destPhoneNumber = sqlResult;
        std::cout << "destPhoneNumber: " << destPhoneNumber << std::endl;


        /* get imei of destination device */
        sqlString = "select device_imei from Phone_numbers where phone_number == \"" + destPhoneNumber + "\"";
        database->sqlSelect(sqlString.c_str(), &sqlResult);
        destIMEI = sqlResult;
        std::cout << "destIMEI: " << destIMEI << std::endl;

         /* get current time */
        calltime->getTime(currentTime);

        /* set end time for source device */
        sqlString = "update Calls set end_time = \"" + currentTime + "\" where phone_number IN (select phone_number from Phone_numbers where \"" + srcIMEI + "\" == device_imei) and end_time is null";
        database->sqlUpdate(sqlString.c_str());

        /* set end time for destination device */
        sqlString = "update Calls set end_time = \"" + currentTime + "\" where phone_number IN (select phone_number from Phone_numbers where \"" + destIMEI + "\" == device_imei) and end_time is null";
        database->sqlUpdate(sqlString.c_str());

        /* disconnect source device */
        sqlString = "update Devices set device_status = 0 where imei == \"" + srcIMEI + "\"";
        database->sqlUpdate(sqlString.c_str());

        /* disconnect destination device */
        sqlString = "update Devices set device_status = 0 where imei == \"" + destIMEI + "\"";
        database->sqlUpdate(sqlString.c_str());
    }

    return 0;
}

void BaseStation::getDeviceStatus(std::string srcIMEI, std::string &srcRes)
{
    sqlString = "select status_type from Status, Devices where device_status == Status.id and imei == \"" + srcIMEI + "\"";
    database->sqlSelect(sqlString.c_str(), &sqlResult);
    srcRes = sqlResult;
}

void BaseStation::printCallHistory(std::string srcIMEI)
{
    std::string srcPhoneNumber = "";

    /* get source devices phone number */
    sqlString = "select phone_number from Phone_numbers where device_imei == \"" + srcIMEI + "\"";
    database->sqlSelect(sqlString.c_str(), &sqlResult);
    srcPhoneNumber = sqlResult;

    /* print out history */
    sqlString = "select Calls.phone_number, Calls.start_time, Calls.end_time, Call_types.type, Calls.phone_number_reciever from Calls, Call_types where Calls.call_type == Call_types.id and phone_number == \"" + srcPhoneNumber + "\" order by Calls.id desc";
    database->printCalls(sqlString.c_str());


}


BaseStation::~BaseStation()
{
    delete database;
    delete calltime;
}
