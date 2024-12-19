#include <iostream>
#include <system_error>
#include "UserInterface.h"
#include "server.h"
#include "auth.h"

int main(int argc, const char** argv) {
    try {
	UserInterface ui;
	if (ui.Parser(argc, argv)) {
		Params params = ui.getParams();
		std::cout << "database: " << params.database << "\n";
		std::cout << "logfile: " << params.logfile << "\n";
		std::cout << "port: " << params.port << "\n";
		Server server(params.database, params.port, params.logfile);
    		server.run();
	}
	
	}  catch (const server_error& e) {
		std::cerr << "error: " << e.what() << "\n";
		return 1;
        
    } catch(...) {
		std::cerr << "Exception of unknown type!\n";
		std::terminate();
	} 
}
