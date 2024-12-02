#include <iostream>
#include <string>
#include <sstream>
#include "UserInterface.h"
#include "serv_error.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

UserInterface::UserInterface() : desc("Allowed options") {
	desc.add_options()
            	("help,h" , "Выдать справку")
        	("database,d" , po::value<std::string>(&params.database)->required(), "База данных пользователей")
        	("logfile,l" , po::value<std::string>(&params.logfile)->required(), "Файл журнала")
        	("port,p", po::value<unsigned short int>(&params.port)->required(), "Порт сервера");
}

bool UserInterface::fileExists(const std::string& filename) {
    fs::path filePath(filename);
    return fs::exists(filePath) && fs::is_regular_file(filePath);
}

bool UserInterface::Parser(int argc, const char** argv) {
	po::store(parse_command_line(argc, argv, desc), vm);
	if (vm.count("help")) {
        std::cout << desc << std::endl;
        return false;
    }
    po::notify(vm);
    std::string database = vm["database"].as<std::string>();
    std::string logfile = vm["logfile"].as<std::string>();
    if (!fileExists(database)) {
        throw FileError("Database file does not exist!");
        return false;
    }
    else if (!fileExists(logfile)) {
        throw FileError("Log file does not exist");
        return false;
    }
 	return true;
}

std::string UserInterface::getDescription()
{
	std::ostringstream ss;
	ss << desc;
	return ss.str();
}
