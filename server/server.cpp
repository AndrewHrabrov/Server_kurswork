#include <iostream>
#include <cstring> 
#include <cerrno> 
#include <ctime>   
#include <fstream>  
#include <iomanip> 
#include "server.h"
#include "auth.h"
#include <system_error>
#include <chrono>
#include <ctime>

Logger::Logger(const std::string& filename) : logFilename(filename){
    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        logError("Unable to open logfile!", false);
        throw FileError("Unable to open logfile!");
    }
}

void Logger::logError(const std::string& message, bool critical) {
    if (logFile.is_open()) {
        logFile << getCurrentDateTime() << " - " ;
        if (critical) {
            logFile << "CRITICAL: " ;
        } else {
            logFile << "WARNING: ";
        }
        logFile << message << std::endl;
    }
}

std::string Logger::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}


Server::Server(const std::string& database, unsigned short int port, const std::string& logfile) : database(database), logfile(logfile), logger(logfile) {
    serverSocket = (socket(AF_INET, SOCK_STREAM, 0));

    if (serverSocket == -1) {
        logger.logError("Socket error", true);
    	throw server_error(errno, "Socket error");
        exit(1);
    }
    
    std::unique_ptr <sockaddr_in> self_addr(new sockaddr_in);
    self_addr->sin_family = AF_INET;
    self_addr->sin_port = htons(port); 
    self_addr->sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(serverSocket, reinterpret_cast<const sockaddr*>(self_addr.get()), sizeof (sockaddr_in)) == -1) {
        logger.logError("Bind error", true);
        throw server_error(errno, "Bind error");
    }
}

Server::~Server() {close(serverSocket);}

void Server::run() {
    
    if (listen(serverSocket, 100) == -1) {
        logger.logError("Listen error", true);
        throw server_error(errno, "Listen error");
    }
    socklen_t socklen = sizeof (sockaddr_in);

    std::clog << "Server started..." << std::endl;
	
    while (true) {
        std::unique_ptr <sockaddr_in> foreign_addr(new sockaddr_in);
        int WorkSocket = accept(serverSocket,
                               reinterpret_cast<sockaddr*>(foreign_addr.get()),
                               &socklen);

        if (WorkSocket == -1) {
            logger.logError("Accept error", false);
            throw server_error(errno, "Accept error");
            continue; 
        }
        std::string ip_addr(inet_ntoa(foreign_addr->sin_addr));
        std::cout << "New client connected. \n";
        struct timeval timeout {180, 0};
        setsockopt(WorkSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        
        try {
            Auth auth(WorkSocket, logfile);
            auth.authentication(WorkSocket, database);
        } catch(const auth_error& e) {
            std::cerr << "error: " << e.what() << "\n";
        } catch (server_error &e) {
            std::cerr << e.what() << std::endl;
        } catch (vector_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }
}
