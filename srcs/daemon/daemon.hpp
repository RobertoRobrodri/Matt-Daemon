#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/file.h>
#include <csignal>
#include <poll.h>
#include "../../includes/includes.hpp"
#include <vector>
#include <map>

#define	sock_in		struct sockaddr_in
#define	sock_addr	struct sockaddr
#define	poll_fd		struct pollfd
#define LOCK_FILE "/var/lock/matt_daemon.lock"
// TODO meter dependencias hpp en Makefile
// #define LOCK_FILE "/Users/cx02938/Desktop/matt_daemon.lock"
#define MAX_CLIENTS 3
#define MSG_SIZE	512

class	Daemon {

	private:
		sock_in		 				_addr;
		int 						_lock_file_fd;
		int 						_socket_fd;
		bool						_keep_running;
		std::vector<struct pollfd>	_poll_fds;

		void	init_socket_struct(void);
		void	init_pollfd(void);
		bool	fd_ready(void);
		void	accept_communication(void);
		void	receive_communication(std::vector<struct pollfd>::iterator it);
		void	add_user(int fd);
		void	delete_user(std::vector<struct pollfd>::iterator it);
	public:

		Daemon 			( void );
		Daemon 			( std::string port, std::string host );
		Daemon 			( const Daemon & var );
		~Daemon			( void );

		bool 			get_keep_running();
		void			set_keep_running(bool state);

		Daemon &operator=(const Daemon &tmp);

		// Member functions
		bool	init_server(void);
		void	server_listen();
		void 	Daemonize(void);
		void	create_lock_file(void);
		static void signal_handler(int signal);
		
		static std::map<int, Daemon *> instances;
};
std::ostream &operator<<(std::ostream& os, const Daemon &tmp);
