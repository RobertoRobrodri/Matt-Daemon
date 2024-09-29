#include "../includes/includes.hpp"
#include "daemon/daemon.hpp"

Tintin_reporter logger;

int main()
{
	if (geteuid())
	{
		std::cout << "Make sure you have root privileges" << std::endl;
		return EXIT_FAILURE ;
	}
	// create server
	logger.log_entry("Started", "INFO");
	Daemon Daemon_server;
	// init Daemon
	Daemon_server.Daemonize();
	// signal handler
	std::signal(SIGINT, Daemon_server.signal_handler);
	// listening
	Daemon_server.server_listen();
	return EXIT_SUCCESS;
}