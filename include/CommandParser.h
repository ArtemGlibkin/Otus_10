#pragma once
#include "Database.h"
#include <map>
#include <string>

class Command
{
public:
    Command()
    {

    }
    virtual std::string execute(Database & db) = 0;
};

class InsertCommand : public Command
{
    std::string mTableName;
    std::string mValue;
    int mID = 0;
public:
    InsertCommand(const std::string & table, int id, const std::string & value) : mTableName(table), mValue(value), mID(id)
    {
       
    }

    std::string execute(Database & db)
    {
        return db.insert(mTableName, mID, mValue);
    }
};

class TruncateCommand : public Command
{
    std::string mTableName;
public:
    TruncateCommand(const std::string & table) : mTableName(table)
    {

    }

    std::string execute(Database & db)
    {
        return db.clear_table(mTableName);
    }
};

class IntersectionCommand : public Command
{
public:
    IntersectionCommand()
    {

    }

    std::string execute(Database & db)
    {
        return db.intersection();
    }
};

class SymmetricDifferenceCommand : public Command
{
public:
    SymmetricDifferenceCommand()
    {

    }

    std::string execute(Database & db)
    {
        return db.symmetric_difference();
    }
};

class UnknownCommand : public Command
{
    public:
    UnknownCommand()
    {

    }
    std::string execute(Database & db)
    {
        return "ERROR: Unknown Command";
    }
};

class ErrorCommand : public Command
{
    std::string mError;
    public:
    ErrorCommand(const std::string & command) : mError(command)
    {
    }
    std::string execute(Database & db)
    {
        return mError;
    }
};

using CommandPtr = std::unique_ptr<Command>;
class CommandCreator
{
    std::map<std::string, std::function<CommandPtr(std::string &)>> mCommandCreators = {
        {"INSERT", std::bind(&CommandCreator::createInsertCommand, this, std::placeholders::_1)},
        {"TRUNCATE", std::bind(&CommandCreator::createTruncateCommand, this, std::placeholders::_1)},
        {"INTERSECTION", std::bind(&CommandCreator::createIntersectionCommand, this, std::placeholders::_1)},
        {"SYMMETRIC_DIFFERENCE", std::bind(&CommandCreator::createSymmetricDifferenceCommand, this, std::placeholders::_1)}
    };
public:
    CommandPtr create(std::string& command, std::string & parameters)
    {
        auto it = mCommandCreators.find(command);
        if(it == mCommandCreators.end())
        {
            std::cout<<command<<"   "<<parameters<<std::endl;
            return std::make_unique<UnknownCommand>();
        }
        return it->second(parameters);
    }

private:
    CommandPtr createInsertCommand(std::string & string)
    {
        std::string::size_type pos = string.find(" ");
        if(pos == std::string::npos)
            return std::make_unique<ErrorCommand>(std::string("ERROR:INSERT [TableName] [id] [value]"));
        std::string table_name = string.substr(0, pos);
        std::string token  = string.substr(pos+1);
        pos = token.find(" ");
        if(pos == std::string::npos)
            return std::make_unique<ErrorCommand>(std::string("ERROR:INSERT [TableName] [id] [value]"));
        std::string id = token.substr(0, pos);
        std::string value  = token.substr(pos+1);    
        return std::make_unique<InsertCommand>(table_name, std::atoi(id.c_str()), value);
    }

    CommandPtr createTruncateCommand(std::string & string)
    {
        return std::make_unique<TruncateCommand>(string);
    }

    CommandPtr createIntersectionCommand(std::string & str)
    {
        return std::make_unique<IntersectionCommand>();
    }

    CommandPtr createSymmetricDifferenceCommand(std::string & str)
    {
        return std::make_unique<SymmetricDifferenceCommand>();
    }
};

class CommandParser
{
    CommandCreator mCreator;
public:
    CommandPtr parse(std::string & string)
    {
        std::string::size_type pos = string.find(" ");
        std::string command = string.substr(0, pos);
        std::string parameters;
        if(pos != std::string::npos)
            parameters = string.substr(pos + 1);
        return mCreator.create(command, parameters);
    }
};