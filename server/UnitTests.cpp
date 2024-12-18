#include <UnitTest++/UnitTest++.h>
#include "UserInterface.h"
#include "server.h"
#include "serv_error.h"
#include "auth.h"

SUITE(HelpTest) {
    TEST(ShortHelp) {
        UserInterface ui;
        const char* argv[] = {"test", "-h", nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(LongHelp) {
        UserInterface ui;
        const char* argv[] = {"test", "--help", nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(HelpAndDatabase) {
        UserInterface ui;
        const char* argv[] = {"test", "-h", "-d", "base.conf", nullptr}; 
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(DatabaseAndHelp) {
        UserInterface ui;
        const char* argv[] = {"test", "-d", "base.conf", "-h", nullptr}; 
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
}

SUITE(TestWorkParams) {
    
    TEST(ShortParams) {
        UserInterface ui;
        std::string database("base.conf");
        std::string logfile("logfile.log");
        unsigned short int port = 33333;
        const char* argv[] = {"test", "-d", database.c_str(), 
                                      "-l", logfile.c_str(),
                                      "-p", std::to_string(port).c_str(), nullptr}; 
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(ui.Parser(argc, argv));
        CHECK_EQUAL(ui.getParams().database, database);
        CHECK_EQUAL(ui.getParams().logfile, logfile);
        CHECK_EQUAL(ui.getParams().port, port);
    }
    
    TEST(LongParams) {
        UserInterface ui;
        std::string database("base.conf");
        std::string logfile("logfile.log");
        unsigned short int port = 33333;
        const char* argv[] = {"test", "--database", database.c_str(), 
                                      "--logfile", logfile.c_str(),
                                      "--port", std::to_string(port).c_str(), nullptr}; 
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(ui.Parser(argc, argv));
        CHECK_EQUAL(ui.getParams().database, database);
        CHECK_EQUAL(ui.getParams().logfile, logfile);
        CHECK_EQUAL(ui.getParams().port, port);
    }
    
    TEST(LogBasePort) {
        UserInterface ui;
        std::string database("base.conf");
        std::string logfile("logfile.log");
        unsigned short int port = 33333;
        const char* argv[] = {"test", "-l", logfile.c_str(), 
                                      "-d", database.c_str(),
                                      "-p", std::to_string(port).c_str(), nullptr}; 
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(ui.Parser(argc, argv));
        CHECK_EQUAL(ui.getParams().database, database);
        CHECK_EQUAL(ui.getParams().logfile, logfile);
        CHECK_EQUAL(ui.getParams().port, port);
    }
    
    TEST(PortLogBase) {
        UserInterface ui;
        std::string database("base.conf");
        std::string logfile("logfile.log");
        unsigned short int port = 33333;
        const char* argv[] = {"test", "-p", std::to_string(port).c_str(), 
                                      "-l", logfile.c_str(),
                                      "-d", database.c_str(), nullptr}; 
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(ui.Parser(argc, argv));
        CHECK_EQUAL(ui.getParams().database, database);
        CHECK_EQUAL(ui.getParams().logfile, logfile);
        CHECK_EQUAL(ui.getParams().port, port);
    }
}

SUITE(ExceptionTest) {
    TEST(NoParams) {
        UserInterface ui;
        const char* argv[] = {"test", nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(OnlyBase) {
        UserInterface ui;
        std::string database("base.conf");
        const char* argv[] = {"test", "-d", database.c_str(), nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(OnlyLog) {
        UserInterface ui;
        std::string logfile("logfile.log");
        const char* argv[] = {"test", "-l", logfile.c_str(), nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(OnlyPort) {
        UserInterface ui;
        unsigned short int port = 33333;
        const char* argv[] = {"test", "-p", std::to_string(port).c_str(), nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(OnlyParamBase) {
        UserInterface ui;
        const char* argv[] = {"test", "-d", nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(OnlyParamLog) {
        UserInterface ui;
        const char* argv[] = {"test", "-l", nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(NoBaseValue) {
        UserInterface ui;
        std::string logfile("logfile.log");
        unsigned short int port = 33333;
        const char* argv[] = {"test", "-d", "", 
                                      "-l", logfile.c_str(),
                                      "-p", std::to_string(port).c_str(), nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(NoLogValue) {
        UserInterface ui;
        std::string database("base.conf");
        unsigned short int port = 33333;
        const char* argv[] = {"test", "-d", database.c_str(), 
                                      "-l", "",
                                      "-p", std::to_string(port).c_str(), nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
        CHECK(!ui.getDescription().empty());
    }
    
    TEST(DatabaseNotExist) {
        UserInterface ui;
        std::string database("bse.conf");
        std::string logfile("logfile.log");
        unsigned short int port = 33333;
        const char* argv[] = {"test", "-d", database.c_str(), 
                                      "-l", logfile.c_str(),
                                      "-p", std::to_string(port).c_str(), nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
    }
    
    TEST(LogNotExist) {
        UserInterface ui;
        std::string database("base.conf");
        std::string logfile("logfle.log");
        unsigned short int port = 33333;
        const char* argv[] = {"test", "-d", database.c_str(), 
                                      "-l", logfile.c_str(),
                                      "-p", std::to_string(port).c_str(), nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
    }
    
    TEST(BigPort) {
        UserInterface ui;
        std::string database("base.conf");
        std::string logfile("logfle.log");
        int port = 100000; 
        const char* argv[] = {"test", "-d", database.c_str(),
                              "-l", logfile.c_str(),
                              "-p", std::to_string(port).c_str(), nullptr};
        int argc = sizeof(argv) / sizeof(nullptr) - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
    }
    
    TEST(SystemPort) {
        UserInterface ui;
        std::string database("base.conf");
        std::string logfile("logfle.log");
        unsigned short int port = 7;
        const char* argv[] = {"test", "-d", database.c_str(), 
                                      "-l", logfile.c_str(),
                                      "-p", std::to_string(port).c_str(), nullptr};
        int argc = sizeof argv / sizeof nullptr - 1;
        REQUIRE CHECK(!ui.Parser(argc, argv));
    }
}

SUITE(AuthTest) {
    TEST(ValidHash) {
        std::string salt = "29F7BEF72E3278B0";
        std::string pass = "SupErP@$$w0rd";
        std::string expectedHash = "48B1C7912D446DCA3E91CDFAFB3BD6A3A0EA1F50";
        Auth auth(123, "logfile.log");
        std::string calculatedHash = auth.calculateHash(pass, salt);
        CHECK_EQUAL(expectedHash, calculatedHash);
    }
}

SUITE(getPassTest) {
    TEST(FoundPass) {
        std::string database = "base.conf";
        std::string user = "user";
        std::string expectedPass = "P@ssW0rd";
        Auth auth(123, "logfile.log");
        std::string pass = auth.getPass(database, user);
        CHECK_EQUAL(expectedPass, pass);
    }
    
    TEST(NotFoundPass) {
        std::string database = "base.conf";
        std::string user = "noname";
        Auth auth(123, "logfile.log");
        CHECK_THROW(auth.getPass(database, user), auth_error);
    }
    
    TEST(DatabaseDoesNotExist) {
        std::string database = "bse.conf";
        std::string user = "user";
        Auth auth(123, "logfile.log");
        CHECK_THROW(auth.getPass(database, user), FileError);
    }
}

int main()
{
    return UnitTest::RunAllTests();
}
