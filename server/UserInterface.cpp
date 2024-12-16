#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "UserInterface.h"
#include "serv_error.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cmath>
#include <limits>

UserInterface::UserInterface() : desc("Allowed options") {
	desc.add_options()
            ("help,h" , "Выдать справку")
        	("database,d" , po::value<std::string>(&params.database)->required(), "База данных пользователей")
        	("logfile,l" , po::value<std::string>(&params.logfile)->required(), "Файл журнала")
        	("port,p", po::value<unsigned short int>(&params.port)->default_value(33333), "Порт сервера, от 1024 до 65535!");
}

bool UserInterface::fileExists(const std::string& filename) {
    fs::path filePath(filename);
    return fs::exists(filePath) && fs::is_regular_file(filePath);
}

bool UserInterface::Parser(int argc, const char** argv) {
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return false;
        }

        po::notify(vm);
        if (!vm.count("database") || (vm.count("database") && params.database.empty())) {
            throw po::required_option("database");
        }
        
        else if (!vm.count("logfile") || (vm.count("logfile") && params.logfile.empty())) {
            throw po::required_option("logfile");
        }
        
        
        else if (params.port > 0 && params.port < 1023) { // от 0 до 1023 - диапазон зарезервированных системных сетевых портов
            throw std::invalid_argument("Invalid port");
        }
        
        else if (!fileExists(params.database) || !fileExists(params.logfile)) {
            throw FileError("File does not exist!");
        }
        
        return true;
    
    } catch (const FileError& e) {
        std::cerr << e.what() << std::endl;
        return false;
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
        std::cout << desc << std::endl;
        return false;
    } catch (const po::required_option& e) {
        std::cerr << "Error parsing command line " << e.what() << std::endl;
        std::cout << desc << std::endl;
        return false;
    } catch (const po::error& e) {
        std::cerr << "Error parsing command line: " << e.what() << std::endl;
        std::cout << desc << std::endl;
        return false;
    }
}

std::string UserInterface::getDescription()
{
	std::ostringstream ss;
	ss << desc;
	return ss.str();
}