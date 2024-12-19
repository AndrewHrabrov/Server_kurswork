/** @file
 * @author Храбров А.А.
 * @version 1.0
 * @date 18.12.2024
 * @copyright ИБСТ ПГУ
 * @warning Тестовый
 * @brief Заголовочный файл для модуля аутентификации auth.h
*/

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

/*
 * @file UserInterface.h
 * @brief Этот файл содержит объявления для функций, связанных с пользовательским интерфейсом
 */

#include "UserInterface.h"

/*
 * @file serv_error.h
 * @brief Этот файл содержит классы возбуждаемых исключений
 */

#include "serv_error.h"

/*
 * @file server.h
 * @brief Этот файл содержит объявления для функций, связанных с сетевым взаимодействием
 */

#include "server.h"

#define BUFLEN 5024 ///< Макрос, который определяет максимальную длину буфера

/** @namespace CPP
  * @brief Пространство имён библиотеки Crypto++
 */
namespace CPP = CryptoPP; 

/** @brief Класс аутентификации
 * @details В конструкторе устанавливается клиентский сокет и название файла журнала для логирования.
 * Класс содержит главную функцию аутентификации, метод чтений данных из буфера, метод поиска пароля из базы данных,
 * метод отправки сообщения клиенту, метод вычисления хэша и метод приема и обработки векторов.
 */

class Auth {
public:
    /**
     * @brief Конструктор класса Auth
     * @details Констурктор устанавливает клиентский сокет, название файла журнала, вызывает конструктор класса Logger
     * и создает новый объект класса SHA1 для метода вычисления хэша
     * @param [in] socket клиентский сокет, file название файла журнала
     */
    Auth(int socket, std::string file);
    
    ~Auth(); ///< Деструктор класса Auth освобождает динамическую память, выделенную после создания объекта класса SHA1
    
    /**
     * @brief Функция аутентификации
     * @details Принимает сообщение от клиента и извлекает из него логин, соль и хэш.
     * Затем по извлеченному логину в базе данных ищется пароль вычисляется хэш и сравнивается 
     * с хэшем, полученным от клиента. Если хэши совпадают, то клиенту отправляется сообщение "OK",
     * в противном случае клиенту отправляется сообщение "ERR" и возбуждается исключение auth_error.
     * @param [in] socket клиентский сокет
     * @param [in] database название файла с базой данных
     * @throw FileError, если файл не открывается или его не существует
     * @throw auth_error, если из сообщения не удалось извлечь логин или если пароли не совпадают
     * @throw std::system_error, при ошибках, связанных с приемом или отправкой данных клиенту
     */
    void authentication(int socket, std::string database);

private:
    int clientSocket; ///< Клиентский сокет
    std::string database; ///< Строка, содержащая название файла с базой клиентов
    std::string logfile; ///< Строка, содержащая название файла журнала
    Logger logger; ///< Объект класса Logger
    CryptoPP::HashTransformation*  hash; 
    
    /**
     * @brief Функция чтения данных из буфера методом удвоения в цикле
     * @return res Данные из буфера
     * @throw std::system_error, если буфер пустой или при ошибках приема данных от клиента
     */
    std::string read_buffer(); 
    
    /**
     * @brief Функция отправки данных клиенту
     * @param [in] socket клиентский сокет
     * @param [in] message сообщение, отправляемое клиенту
     * @throw std::system_error, при ошибках оправки данных клиенту
     */
    void sendMessage(int socket, const std::string& message);
    
    /**
     * @brief Функция вычисления хэша 
     * @param [in] pass пароль
     * @param [in] salt соль
     * @return hash_16 строка, содержащая хэш
     * @throw auth_error, если вычисленный хэш и хэш, полученный от клиента не совпадают
     */
    std::string calculateHash(const std::string& pass, const std::string& salt);
    
    /**
     * @brief Функция обработки векторов
     * @details Функция принимает количество векторов, длину вектора и его значения и вычисляет сумму квадратов
     * для каждого вектора и отправляет результат клиенту. В случае переполнения клиенту отправляется максимальное значения 
     * для типа uint32_t.
     * @param [in] client_socket клиентский сокет
     * @throw std::system_error, при ошибках, связанных с приемом или отправкой данных клиенту
     * @throw vector_error, если длина вектора больше максимального значения для uint32_t 
     * или длина вектора отрицательная
     */
    void processVectors(int client_socket);
    
    /**
     * @brief Функция поиска пароля по логину
     * @param [in] filename файл с базой клиетов
     * @param [in] username логин
     * @return pass строка, содержащая пароль, соответствующий логину
     * @throw FileError, если невозможно открыть файл или его не существует
     * @throw auth_error, если логин не найден или строки базы данных не соответствуют формату
     */
    std::string getPass(const std::string& filename, const std::string& username);
};

// #undef private // Для модульного тестирования приватных методов класса