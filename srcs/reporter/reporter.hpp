#pragma once
#include <string>
#include <iostream>
#include <fstream>  
#include <ctime>
#include <sys/stat.h>
#include <filesystem>
#include <time.h>

#define LOG_PATH  "/var/log/matt_daemon/"
#define LOG_NAME  "matt_daemon.log"
#define LOG_FILE  LOG_PATH LOG_NAME
// #define LOG_FORMAT "[{}] [ {} ] - Matt_daemon: {}." // Reminder to use c++20 format function but i cant install that shit
class	Tintin_reporter {

	private:
		std::ofstream _log_file;

		Tintin_reporter ( std::string str );

		std::string time_formatted();
	public:

		Tintin_reporter ( void );
		Tintin_reporter ( const Tintin_reporter & var );
		~Tintin_reporter ( void );

		// Overloading
		Tintin_reporter &operator=	(const Tintin_reporter &tmp);
		
		// Member functions
		bool log_entry(std::string log_msg, std::string log_level);
};

std::ostream &operator<<(std::ostream& os, const Tintin_reporter &tmp);