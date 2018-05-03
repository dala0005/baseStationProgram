#ifndef CALLTIME_H
#define CALLTIME_H
#include <ctime>
#include <iostream>

class CallTime
{
    public:
        CallTime();
        void getTime(std::string &str);
        ~CallTime();
    private:
        time_t callTime;
        struct tm *timeStruct;
        char timeBuffer[50];
};

#endif // CALLTIME_H
