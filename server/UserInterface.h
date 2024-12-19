/** @file
 * @author Храбров А.А.
 * @version 1.0
 * @date 18.12.2024
 * @copyright ИБСТ ПГУ
 * @warning Тестовый
 * @brief Заголовочный файл для модуля пользовательского интерфейса UserInterface.h
*/

#pragma once
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>

/** @namespace po
  * @brief Пространство имен библиотеки boost::program_options. Используется для парсинга командной строки
 */
namespace po = boost::program_options; 

/** @namespace fs
  * @brief Пространство имен библиотеки boost::filesystem. Используется для проверки существования файлов
 */
namespace fs = boost::filesystem;

/** @struct Params
  * @brief Структура для хранения значений, полученных из командной строки
 */
struct Params {
	std::string logfile; ///< Строка, содержащая название файла журнала
	std::string database; ///< Строка, содержащая название файла с базой клиентов
	unsigned short int port; ///< Значение сетевого порта, на котором работает сервер
};

/** @brief Класс UserInterface
 */
class UserInterface {
private:
	po::options_description desc; ///< Объект, содержащий справку к пользовательскому интерфейсу
	po::variables_map vm; ///< Ассоциативный массив, который используется для хранений знчений командной строки
    Params params; ///< Объект структуры Params

public:
    /**
     * @brief Конструктор класса UserInterface
     * @details Конструктор формирует справку к пользовательскому интерфейсу
     */
	UserInterface();
    
    /**
     * @brief Метод разбора значений командной строки
     * @param [in] argc количество параметров 
     * @param [in] argv значения параметров
     * @return false, при возникновении ошибок
     * @return true
     * @throw po::required_option, если отсутствуют обязательные параметры
     * @throw std::invalid_argument, при передачи недопустимых значений
     * @throw FileError, если файл(ы) не существуют
     */
	bool Parser(int argc, const char** argv);
    
    /**
     * @brief Метод проверки существования файлов
     * @param [in] filename название файла
     * @return false, если файл не существует
     * @return true
     */
    bool fileExists(const std::string& filename);
    
    /**
     * @brief Метод получения справки
     * @return ss.str(), строка из потока со справкой
     */
 	std::string getDescription();
    
    /**
     * @brief Метод получения параметров из структуры Params
     * @return params, объект структуры Params
     */
 	Params getParams() {
 		return params;
 	};
};
