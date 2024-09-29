#include "../includes/includes.hpp"
#include "daemon/daemon.hpp"

Tintin_reporter logger;

//static void signal_handler(int signal) {
//	logger.log_entry("Quitting", "INFO");
//	// THIS DOES NOT WORK, USE SIGSET
//	if (signal != 0) {
//    	switch (signal) {
//    	    case SIGINT:
//    	        logger.log_entry("SIGINT received, shutting down", "INFO");
//				clean();
//    	        break;
//    	    case SIGTERM:
//    	        logger.log_entry("SIGTERM received, terminating", "INFO");
//    	        break;
//    	    case SIGHUP:
//    	        logger.log_entry("SIGHUP received, reloading configuration", "INFO");
//    	        break;
//    	    default:
//    	        logger.log_entry("Unknown signal received", "WARNING");
//    	}
//	}
//	exit(EXIT_SUCCESS);
//}

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