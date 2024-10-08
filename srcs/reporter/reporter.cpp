#include "reporter.hpp"
#include "../../includes/includes.hpp"

Tintin_reporter::Tintin_reporter( void ) {
	std::cout << "Default reporter constructor called" << std::endl;
	// create directory. Ignore error if directory already exists
	std::filesystem::create_directory(LOG_PATH);
	// create file
	_log_file.open(LOG_FILE, std::fstream::out | std::fstream::app);
	// check rd_state for failbit
	if (_log_file.fail()) {
		std::cout << "Error creating log file" << std::endl;
		exit(EXIT_FAILURE);
	}
}

Tintin_reporter::Tintin_reporter( std::string str ) {
	(void)str;
	std::cout << "Parameter reporter constructor called" << std::endl;
	
}

Tintin_reporter::Tintin_reporter( const Tintin_reporter & var ) {

	std::cout << "Copy reporter constructor called" << std::endl;
	
}

Tintin_reporter::~Tintin_reporter( void ) {
	std::cout << "Destructor reporter called" << std::endl;
	// close log file
	_log_file.close();
}

// OVERLOADING
Tintin_reporter & Tintin_reporter::operator=(const Tintin_reporter &tmp)
{
	(void)tmp;
	std::cout << "Tintin_reporter Operator equalizer called" << std::endl;
	return *this;
}

std::ostream &operator<<(std::ostream& os, const Tintin_reporter &tmp)
{
	(void)tmp;
	return (os);
}

// FUNCTIONS

std::string Tintin_reporter::time_formatted() {
		const int MAXLEN = 80;
		char s[MAXLEN];
		time_t t = time(0);
		std::tm* local_time = std::localtime(&t);

		strftime(s, sizeof(s), "%d/%m/%Y-%H:%M:%S", local_time);
		return s;
}

// writes to log file with timestamp
bool Tintin_reporter::log_entry(std::string log_msg, std::string log_level) {
	std::string log_time = time_formatted();
	_log_file << "[" + log_time + "]" + "[ " + log_level + " ]" + " - Matt_daemon: " + log_msg << std::endl;
	_log_file.flush();
	return 1;
}