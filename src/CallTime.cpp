#include "CallTime.h"
#include <iostream>
#include <ctime>

CallTime::CallTime()
{

}

void CallTime::getTime(std::string &str)
{
    callTime = time(0);
    ctime(&callTime);
    timeStruct = localtime(&callTime);
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", timeStruct);
    str = timeBuffer;
}

CallTime::~CallTime()
{
    //dtor
}
