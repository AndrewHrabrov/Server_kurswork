#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
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
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << std::endl;
            return false;
        }

        po::notify(vm);

        if (!vm.count("port")) {
            throw std::logic_error("Error: --port option is required.");
        }
        if (!vm.count("database")) {
            throw std::logic_error("Error: --database option is required.");
        }
        if (!vm.count("logfile")) {
            throw std::logic_error("Error: --logfile option is required.");
        }
        
        std::string database = vm["database"].as<std::string>();
        std::string logfile = vm["logfile"].as<std::string>();
        unsigned short int port = vm["port"].as<unsigned short int>();
        if (!fileExists(database)) {
            throw FileError("Database file does not exist!");
            return false;
        }
        
        else if (!fileExists(logfile)) {
            throw FileError("Log file does not exist");
            return false;
        }
        
        if (port < 0 || port > 65535) { //65535 - максимальный возможный порт
        }
        return true;
    } catch (const FileError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    } catch (po::invalid_command_line_syntax& e) {
		std::cerr << "error: " << e.what() << "\n";
		std::cerr << "Use -h for help\n";
		exit(1);
	} catch (po::required_option& e) {
		std::cerr << "error: " << e.what() << "\n";
		exit(1);
	} catch (po::error& e) {
		std::cerr << "error: " << e.what() << "\n";
		std::cerr << "Use -h for help\n";
        exit(1);
    } catch (std::runtime_error& e) {
		std::cerr << "error: " << e.what() << "\n";
        exit(1);
    }
}

std::string UserInterface::getDescription()
{
	std::ostringstream ss;
	ss << desc;
	return ss.str();
}

        
   
