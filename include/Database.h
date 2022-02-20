#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <map>
#include <sstream>


struct DBStruct
{
    int id;
    std::string name;
};

class Database
{
    using DBTable = std::map<int, DBStruct>;
    using DBTableList = std::map<std::string, DBTable>;
    DBTableList mDBList = {{"A", DBTable()}, {"B", DBTable()}};

public:
    std::string insert(const std::string & table_name, int id, const std::string & name)
    {
        auto table = mDBList.find(table_name);
        if(table == mDBList.end())
        {
            return std::string("ERR table not found");
        }
        auto & table_cont = table->second;
        if(table_cont.find(id) != table_cont.end())
          return std::string("ERR duplicate ") + std::to_string(id);

        DBStruct val = {id, name};
        table_cont.emplace(id, std::move(val));
        return "OK";
    }

    std::string symmetric_difference()
    {
        std::stringstream result;
        auto table_a = mDBList.find("A");
        auto table_b = mDBList.find("B");
    
        auto it = table_a->second.begin();
        auto it2 = table_b->second.begin();
        while(it!= table_a->second.end() && it2 != table_b->second.end())
        {
                auto key1 = it->first;
                auto key2 = it2->first;
                
                if(key1 == key2)
                    it++, it2++;
                else if(key1 < key2)
                {
                    result<<key1<<", "<<it->second.name<<", "<<std::endl;
                    it++;
                }
                else if(key2 < key1)
                {
                    result<<key2<<", , "<<it2->second.name<<std::endl;
                    it2++;
                }
        }

        for(it; it != table_a->second.end(); it++)
            result<<it->first<<", "<<it->second.name<<", "<<std::endl;
        for(it2; it2 != table_b->second.end(); it2++)
            result<<it2->first<<", , "<<it2->second.name<<std::endl;
        result<<"OK"<<std::endl;
        return result.str();
    }

    std::string clear_table(std::string table_name)
    {
        auto table = mDBList.find(table_name);
        if(table == mDBList.end())
        {
            return std::string("ERR table not found");
        }
        table->second.clear();
        return "OK";
    }

    std::string intersection()
    {
        std::stringstream result;
        auto table_a = mDBList.find("A");
        auto table_b = mDBList.find("B");
        for(const auto& [key, value] : table_a->second)
        {
            auto tb_value = table_b->second.find(key);
            if(tb_value != table_b->second.end())
                result<<key<<", "<<value.name<<", "<<tb_value->second.name<<std::endl;
        }
        
        result<<"OK"<<std::endl;
        return result.str();
    }

};