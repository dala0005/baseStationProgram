#include "Database.h"
#include <iostream>
#include <sqlite3.h>
#include <cstdio>
#include <string>
#include <debug/string>

Database::Database(const std::string &path)
{
    this->path = path;
}

void Database::openDatabase()
{

    if(sqlite3_open(path.c_str(), &database))
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
    }
    else
    {
        /*fprintf(stderr, "Opened database successfully\n");*/
    }
}


int Database::sqlSelect(const char *sqlString, std::string *sqlResult)
{
    int rc = 0;
    int bytes = 0;
    std::string result = "";
    sqlite3_stmt *stmt;


    openDatabase();

    rc = sqlite3_prepare_v2(database, sqlString, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cout << sqlite3_errmsg(database);
        return 0;
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        bytes = sqlite3_column_bytes(stmt, 0);
        result.assign( reinterpret_cast<char const*>(sqlite3_column_text(stmt, 0)), bytes);
        *sqlResult = result;
    }

    if (rc != SQLITE_DONE) {
        std::cout << "error: " << sqlite3_errmsg(database);
    }
    sqlite3_finalize(stmt);

    closeDatabase();
    return 0;
}


int Database::sqlUpdate(const char * sqlString)
{
    sqlite3_stmt *stmt;

    openDatabase();

    sqlite3_prepare(database, sqlString, -1, &stmt, NULL);
    sqlite3_step(stmt);

    closeDatabase();

    return 0;
}

int Database::sqlInsert(const char * sqlString)
{
    sqlUpdate(sqlString);

    return 0;
}


void Database::printCalls(const char* sqlString)
{
    int rc = 0;
    sqlite3_stmt *stmt;


    openDatabase();

    rc = sqlite3_prepare_v2(database, sqlString, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        std::cout << sqlite3_errmsg(database);
    }

    std::cout << "\n\n** Call history **\n";
    std::cout << "Source phone number\tStart time\t\tEnd time\t\tCall type\t Phone number reciever\n";
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        printf("%s\t\t%s\t%s\t%s\t\t %s\n", sqlite3_column_text(stmt, 0), sqlite3_column_text(stmt, 1), sqlite3_column_text(stmt, 2), sqlite3_column_text(stmt, 3), sqlite3_column_text(stmt, 4));
    }

    if (rc != SQLITE_DONE) {
        std::cout << "error: " << sqlite3_errmsg(database);
    }
    sqlite3_finalize(stmt);

    closeDatabase();
}

void Database::closeDatabase()
{
    sqlite3_close(database);
}

Database::~Database()
{

}
