#include <iostream>
#include <vector>
#include <string>

#include "sqlite3.h"

#include "Database.h"
#include "CommandParser.h"
#include "server.h"
int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        std::cout<<"join_server <port>"<<std::endl;
        return -1;
    }

	const char* dbName = "test_database.sqlite";
	Database db;
	if(!db.open(dbName))
	{
		std::cout<<"Database can not open"<<std::endl;
		return -1;
	}
/*
	db.insert("A", 0, "lean");
	db.insert("A", 1, "sweater");
	db.insert("A", 2, "frank");
	db.insert("A", 3, "violation");
	db.insert("A", 4, "quality");
	db.insert("A", 5, "precision");

	db.insert("B", 3, "proposal");
	db.insert("B", 4, "example");
	db.insert("B", 5, "lake");
	db.insert("B", 6, "flour");
	db.insert("B", 7, "wonder");
	db.insert("B", 8, "selection");

	db.symmetric_difference();
	db.intersection();
	*/
/*
	CommandParser cp;
	while(true)
	{
		std::string command;
		std::getline(std::cin, command, '\n');
		std::cout<<cp.parse(command)->execute(db)<<std::endl;
	}
	*/
	Server server(db, atoi(argv[1]));
	server.start();
	return 0;
}