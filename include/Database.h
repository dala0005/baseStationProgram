#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <sqlite3.h>
#include <vector>


class Database
{
    public:
        Database(const std::string &path);
        int sqlSelect(const char* sqlString, std::string *sqlResult);
        int sqlUpdate(const char* sqlString);
        int sqlInsert(const char* sqlString);
        void printCalls(const char* sqlString);
        ~Database();
    private:
        void openDatabase();
        void closeDatabase();
        std::string path;
        sqlite3 *database;
};


#endif // DATABASE_H
