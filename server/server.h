#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>
#include <memory>
#include <stdexcept>
#include <fstream>

class Logger {
public:
    Logger(const std::string& filename);
    void logError(const std::string& message, bool critical);
private:
    std::ofstream logFile;
    std::string logFilename;
    std::string getCurrentDateTime();
};

class Server {
public:
    Server(const std::string& database, unsigned short int port, const std::string& logfile);
    ~Server();
    void run();
    
private:
    std::string database;
    unsigned short int port;
    std::string logfile;
    Logger logger;
    int serverSocket;
    std::unique_ptr<sockaddr_in> self_addr;
    std::unique_ptr<sockaddr_in> foreign_addr;
};