#pragma once
// #define private   public // Для модульного тестирования приватных методов класса
#include <iostream>
#include <string>
#include <string_view>
#include <cryptopp/hex.h>
#include <cryptopp/sha.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include "UserInterface.h"
#include "serv_error.h"
#include "server.h"

#define BUFLEN 5024
namespace CPP = CryptoPP;

class Auth {
public:
    Auth(int socket, std::string file);
    ~Auth();
    void authentication(int socket, std::string database);

private:
    int clientSocket;
    std::string database;
    std::string logfile;
    Logger logger;
    CryptoPP::HashTransformation*  hash;
    std::string read_buffer();
    void sendMessage(int socket, const std::string& message);
    std::string calculateHash(const std::string& pass, const std::string& salt);
    void processVectors(int client_socket);
    std::string getPass(const std::string& filename, const std::string& username);
};

// #undef private // Для модульного тестирования приватных методов класса