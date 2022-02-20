#include <iostream>
#include <vector>
#include <string>
#include "Database.h"
#include "server.h"

int main(int argc, char * argv[])
{
    if(argc != 2)
    {
        std::cout<<"join_server <port>"<<std::endl;
        return -1;
    }

	Database db;
	Server server(db, atoi(argv[1]));
	server.start();
	return 0;
}