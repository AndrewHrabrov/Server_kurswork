#pragma once
#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
#include <string>
namespace po = boost::program_options;
namespace fs = boost::filesystem;

struct Params {
	std::string logfile;
	std::string database;
	unsigned short int port;
};

class UserInterface {
private:
	po::options_description desc;
	po::variables_map vm;
    	Params params;

public:
	UserInterface();
	bool Parser(int argc, const char** argv);
   	bool fileExists(const std::string& filename);
 	std::string getDescription();
 	Params getParams() {
 		return params;
 	};
};
