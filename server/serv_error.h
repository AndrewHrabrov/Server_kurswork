/** @file
 * @author Храбров А.А.
 * @version 1.0
 * @date 18.12.2024
 * @copyright ИБСТ ПГУ
 * @warning Тестовый
 * @brief Заголовочный файл для модуля исключений serv_error.h
*/

#pragma once
#include <string>
#include <stdexcept>
#include <system_error>

/** @brief Класс отлова исключений, связанных с сетевым взаимодействием
 * @details Отлавливает исключения, которые возбуждаются в результате ошибок в сетевом взаимодействии
 */

class server_error : public std::system_error {
public:
    server_error(int code, const std::string& s) : std::system_error(code, std::generic_category(), s) {}
};

/** @brief Класс отлова исключений, связанных с аутентификацией
 * @details Отлавливает исключения, которые возбуждаются в результате ошибок функции аутентификации
 */

class auth_error: public std::runtime_error {
public:
    auth_error(const std::string& s) : std::runtime_error(s) {}
    auth_error(const char * s) : std::runtime_error(s) {}
};

/** @brief Класс отлова исключений, связанных с векторами
 * @details Отлавливает исключения, которые возбуждаются в результате ошибок, связанных с std::vector
 */

class vector_error: public std::runtime_error {
public:
    vector_error(const std::string& s) : std::runtime_error(s) {}
    vector_error(const char * s) : std::runtime_error(s) {}
};

/** @brief Класс отлова исключений, связанных с работой с файлами
 * @details Отлавливает исключения, которые возбуждаются в результате ошибок работы с файлами
 */

class FileError : public std::runtime_error {
public:
    FileError(const std::string& s) : std::runtime_error(s) {}
};