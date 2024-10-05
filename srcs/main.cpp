#include "../includes/includes.hpp"
#include "daemon/daemon.hpp"

Tintin_reporter logger;

int main()
{
	// create server
	logger.log_entry("Started", "INFO");
	Daemon Daemon_server;
	// init Daemon
	Daemon_server.Daemonize();
	// signal handler
	std::signal(SIGINT, Daemon_server.signal_handler);
	std::signal(SIGTERM, Daemon_server.signal_handler);
	std::signal(SIGHUP, Daemon_server.signal_handler);
	std::signal(SIGQUIT, Daemon_server.signal_handler);
	std::signal(SIGABRT, Daemon_server.signal_handler);
	std::signal(SIGKILL, Daemon_server.signal_handler);
	// listening
	Daemon_server.server_listen();
	return EXIT_SUCCESS;
}