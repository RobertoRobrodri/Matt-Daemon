#pragma once
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/file.h>
#include <csignal>
#include <poll.h>
#include "../../includes/includes.hpp"

#define	sock_in		struct sockaddr_in
#define	sock_addr	struct sockaddr
#define	poll_fd		struct pollfd
#define LOCK_FILE "/var/lock/matt_daemon.lock"
#define MAX_CLIENTS 2
#define MSG_SIZE	512

class	Daemon {

	private:
		int 				_lock_file_fd;
		int 				_socket_fd;
		int 				_active_fds;
		bool				_keep_running;
		poll_fd				_poll_fds[MAX_CLIENTS];

		sock_in	init_socket_struct(void);
		void	init_pollfd(void);
		bool	fd_ready(void);
		void	accept_communication(void);
		void	receive_communication(int poll_fd_pos);
		void	add_user(int fd, sock_in client_addr);
		void	delete_user(int poll_fd_pos);
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
};
std::ostream &operator<<(std::ostream& os, const Daemon &tmp);
