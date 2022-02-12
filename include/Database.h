#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <functional>

#include "sqlite3.h"

int readCallback(void* context, int columns, char **data, char **names)
{
        std::string * result = (std::string *)context;
		if (!columns) {
			result->append("Empty result!");
			return 0;
		}

		if (!names) {
            result->append("No columns!");
			return 0;
		}

		if (!data) {
            result->append("No data!");
			return 0;
		}

		for (int i=0; i<columns; ++i) {
			if ((i != 0))
				result->append(", ");
			result->append((data[i] == nullptr ? " " : data[i]));
		}
        result->append("\n");
		return 0;
};


//TODO:: RAII. sqlite3 destruction methods
class Database
{
    std::unique_ptr<sqlite3, std::function<void(sqlite3 *)>> mHandler;

public:

    Database()
    {

    }

    bool open(const std::string & database_name)
    {
        sqlite3* handle = nullptr;
        if (sqlite3_open(database_name.c_str(), &handle)) {
		    std::cout << "sqlite3_open failed! " << sqlite3_errmsg(handle) << std::endl;
		    return false;
	    }
        mHandler = std::unique_ptr<sqlite3, std::function<void(sqlite3 *)>>(handle, [](sqlite3*handle){sqlite3_close(handle);});
        return true;
    }

    std::string insert(const std::string & table_name, int id, const std::string & name)
    {
        char * errMsg = nullptr;
        std::string query = "INSERT INTO " + table_name + " VALUES("+std::to_string(id)+ ", '"+name+"');"; 
        if (sqlite3_exec(mHandler.get(), query.c_str(), /*callback*/nullptr, /*cb 1st argument*/nullptr, &errMsg))
            return std::string(errMsg);
        return "OK";
    }

    std::string symmetric_difference()
    {
        char * errMsg = nullptr;
        std::string result;
        const std::string select2 = "SELECT A.id, A.name, B.name FROM A LEFT JOIN B ON A.id = B.id WHERE B.id is NULL UNION SELECT B.id, A.name, B.name FROM B LEFT JOIN A ON A.id = B.id Where A.id is NULL";
        if (sqlite3_exec(mHandler.get(), select2.c_str(), /*callback*/readCallback, &result, &errMsg)) {
            return std::string(errMsg);
        }
        result.append("OK");
        return result;
    }

    bool create_table(const std::string & table_name)
    {
        char * errMsg = nullptr;
        std::string query = "CREATE TABLE " + table_name +"(id int PRIMARY KEY NOT NULL, name TEXT NOT NULL);";
        if (sqlite3_exec(mHandler.get(), query.c_str(), /*callback*/nullptr, /*cb 1st argument*/nullptr, &errMsg)) {
			std::cout << "sqlite3_exec with createQuery failed! " << errMsg << std::endl;
			sqlite3_free(errMsg);
			return false;
		}
    }

    std::string clear_table(std::string table_name)
    {
        char * errMsg = nullptr;
        std::string query = "DELETE FROM " + table_name + ";"; 
        if (sqlite3_exec(mHandler.get(), query.c_str(), /*callback*/nullptr, /*cb 1st argument*/nullptr, &errMsg))
            return std::string(errMsg);
        return "OK";
    }

    std::string intersection()
    {
        char * errMsg = nullptr;
        const std::string selectAllQuery = "SELECT A.id, A.name, B.name FROM A INNER JOIN B ON A.id = B.id";
        std::string result;
        if (sqlite3_exec(mHandler.get(), selectAllQuery.c_str(), readCallback, &result, &errMsg)) {
            return std::string(errMsg);
        }
        result.append("OK");
        return result;
    }

};