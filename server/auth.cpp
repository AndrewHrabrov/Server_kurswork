#include <iostream>
#include <string>
#include <system_error>
#include "UserInterface.h"
#include "auth.h"
#include <fstream>
#include <iomanip>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <memory> 
#include <stdexcept> 
#include <limits>
#include <cmath>

Auth::Auth(int socket, std::string file) : clientSocket(socket), logfile(file), logger(logfile) {hash = new CPP::SHA1;}
Auth::~Auth() {delete hash;}

//Аутентификация
void Auth::authentication(int socket, std::string database) {
    std::string message (read_buffer());
    std::cout << "A message has been received: " << message << std::endl;

    std::string hash_16(std::max(message.cbegin(), message.cend() - hash->DigestSize() * 2),
                        message.cend());
    message.erase(std::max(message.cbegin(), message.cend() - hash->DigestSize() * 2),
                  message.cend());
    if (message.empty()) {
        logger.logError("Auth error: wrong auth message", false);
        throw auth_error("Auth error: wrong auth message");
    }
    std::clog << "log: detect HASH: " << hash_16 << std::endl;

    std::string salt_16 (std::max(message.cbegin(), message.cend() - 16), message.cend());
    message.erase(std::max(message.cbegin(), message.cend() - 16), message.cend());
    if (message.empty()) {
        logger.logError("Auth error: wrong auth message", false);
        throw auth_error("Auth error: wrong auth message");
    }
    std::clog << "log: detect SALT: " << salt_16 << std::endl;
    
    std::clog << "log: detect USERNAME: " << message << std::endl;
    
    try {
        std::string pass = getPass(database, message);
        if (pass.empty()) { 
            logger.logError("Auth error: user not found", false);
            sendMessage(socket, "Auth error: user not found");
        }
        
        std::string hash_calculated = calculateHash(pass, salt_16);

        if (hash_calculated != hash_16) {
            sendMessage(socket, "ERR");
            logger.logError("Auth error: password mismatch", false);
            throw auth_error("Auth error: password mismatch");
        }

        sendMessage(socket, "OK");
        processVectors(socket);
    }
    catch (const FileError& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(1);
    }
    catch (const auth_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    
}

std::string Auth::read_buffer() //Чтение буффера, метод удвоения в цикле
{
    int rc;
    int buflen = BUFLEN;
    std::unique_ptr <char[]> buf(new char[buflen]);
    while (true) {
        rc = recv(clientSocket, buf.get(), buflen, MSG_PEEK);
        if (rc == -1) {
            logger.logError("Recv string error", false);
            throw std::system_error(errno, std::generic_category(), "Recv string error");
        }
        if (rc < buflen)
            break;
        buflen *= 2;
        buf = std::unique_ptr<char[]>(new char[buflen]);
    }
    std::string res(buf.get(), rc);
    rc = recv(clientSocket, nullptr, rc, MSG_TRUNC);
    if (rc == -1) {
        logger.logError("Clear buffer error", false);
        throw std::system_error(errno, std::generic_category(), "Clear buffer error");
    }
    res.resize(res.find_last_not_of("\n\r") + 1);
    return res;
}

// Чтение базы данных и поиск пароля по логину
std::string Auth::getPass(const std::string& filename, const std::string& username) { 
    std::fstream file;
    std::string line;
    file.open(filename);
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string login, pass;
            std::getline(ss, login, ':'); 
            std::getline(ss, pass);    

            if (login == username) {
                file.close();
                return pass;
            }
        }
        file.close();
        return "";
    }
    else {
        logger.logError("Unable to open database file", true);
        file.close();
        throw FileError("Unable to open database file ");
    }
}

void Auth::sendMessage(int socket, const std::string& message) {
    ssize_t bytes_sent = send(socket, message.c_str(), message.length(), 0);
    if (bytes_sent == -1) {
        logger.logError("Send result error", false);
        throw std::system_error(errno, std::generic_category(), "Send result error");
    }
}

std::string Auth::calculateHash(const std::string& pass, const std::string& salt) {
    CPP::SHA1 sha1; 
    std::string hash_16; 
    CPP::StringSource(
        std::string(salt).append(pass),
        true,
        new CPP::HashFilter(
            sha1,
            new CPP::HexEncoder(
                new CPP::StringSink(hash_16))));
    
    return hash_16; 
}

// Обработка вектора. Операция: сумма квадратов
void Auth::processVectors(int client_socket) {
    uint32_t num_vectors;
    if (recv(client_socket, &num_vectors, sizeof(num_vectors), 0) == -1) {
        logger.logError("Recv number of vectors error", false);
        throw std::system_error(errno, std::generic_category(), "Recv number of vectors error");
    }

    for (uint32_t i = 0; i < num_vectors; ++i) {
        uint32_t vector_len;
        if (recv(client_socket, &vector_len, sizeof(vector_len), 0) == -1) {
            logger.logError("Recv vector size error", false);
            throw std::system_error(errno, std::generic_category(), "Recv vector size error");
        }

        std::vector<uint32_t> vector(vector_len);
        if (recv(client_socket, vector.data(), vector_len * sizeof(uint32_t), 0) == -1) {
            logger.logError("Recv vector error", false);
            throw std::system_error(errno, std::generic_category(), "Recv vector error");
        }
        else if (vector_len > std::numeric_limits<uint32_t>::max() || vector_len < 0) {
            throw vector_error("Vector error: mismatch actual and expected size");
        }

        uint32_t sum_of_squares = 0;
        for (const auto& x : vector) {
            if (sum_of_squares + pow(x, 2) > std::numeric_limits<uint32_t>::max()) {
                sum_of_squares = std::numeric_limits<uint32_t>::max();
                break;
            }
            sum_of_squares += pow(x, 2);
        }

        if (send(client_socket, &sum_of_squares, sizeof(sum_of_squares), 0) == -1) {
            logger.logError("Send result error", false);
            throw std::system_error(errno, std::generic_category(), "Send result error");
        }
    }
}