#include "daemon.hpp"

std::map<int, Daemon *> Daemon::instances;

Daemon::Daemon( void ) {

	std::cout << "Daemon Default constructor called" << std::endl;
	_keep_running = true;
	this->create_lock_file();
	logger.log_entry("Creating server", "INFO");
	try {
		if (this->init_server() == 0)
			throw std::runtime_error("Server not listening");
	}
	catch (std::exception &ex) {
		std::cout << ex.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	logger.log_entry("Server created", "INFO");
}

Daemon::Daemon( std::string port, std::string host ) {
	(void)port;
	(void)host;
	std::cout << "Daemon Parameter constructor called" << std::endl;
	return ;
}

Daemon::Daemon( const Daemon & var ) {
	
	std::cout << "Daemon Copy constructor called" << std::endl;
	*this = var;
	return ;
}

Daemon::~Daemon( void ) {
	
	std::cout << "Daemon Destructor called" << std::endl;
	flock(_lock_file_fd, LOCK_UN);
	close(_socket_fd);
	close(_lock_file_fd);
	remove(LOCK_FILE);
	return ;
}


// Overloading
Daemon & Daemon::operator=(const Daemon &tmp) {
	(void) tmp;
	std::cout << "Daemon Operator equalizer called" << std::endl;
	return (*this);
}

std::ostream &operator<<(std::ostream& os, const Daemon &tmp) {
	(void) tmp;
	os << std::endl << "Daemon Operator output called" << std::endl;
	return (os);
}

// Functions

bool 	Daemon::get_keep_running() {
	return _keep_running;
}

void	Daemon::set_keep_running(bool state) {
	_keep_running = state;
}

void	Daemon::init_socket_struct(void)
{
	// Init struct that the socket needs
	//  IPV4 addresses
	_addr.sin_family      = AF_INET;
	//  Convert our port to a network address (host to network)
	_addr.sin_port        = htons(4242);
	//  Our address as integer
	_addr.sin_addr.s_addr = INADDR_ANY; // Probar Localhost
}

bool Daemon::init_server(void) {
	int opt = 1;
	this->init_socket_struct();
	// Create the socket
	if ((this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			logger.log_entry("Failed to create socket", "ERROR");
			throw std::runtime_error("Failed to create socket");
	}
	std::cout << "Socket initialized" << std::endl;
	// Set socket options (reuse address)
	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
			close(_socket_fd);
			logger.log_entry("Failed to set socket options", "ERROR");
			throw std::runtime_error("Failed to set socket options");
	}
	// Bind the socket to the address and port
	if (bind(this->_socket_fd, (const sock_addr*)&_addr, sizeof(_addr)) == -1) {
			close(_socket_fd);
			logger.log_entry("Error binding socket", "ERROR");
			throw std::runtime_error("Error binding socket");
	}
	// Start listening for connections, with MAX_CLIENTS backlog
	if (listen(this->_socket_fd, MAX_CLIENTS) == -1) {
			close(_socket_fd);
			logger.log_entry("Error starting to listen on socket", "ERROR");
			throw std::runtime_error("Error starting to listen on socket");
	}
	return true;  // Successfully listening
}

// From the original Daemonize function
/* (This function forks, and if the fork(2) succeeds, the parent
			 calls _exit(2), so that further errors are seen by the child
			 only.)  On success Daemon() returns zero.  If an error occurs,
			 Daemon() returns -1 and sets errno to any of the errors specified
			 for the fork(2) and setsid(2). */

void Daemon::Daemonize(void) {
	pid_t pid = fork(); 

	if (pid == -1) { 
			logger.log_entry(strerror(errno), "ERROR");
			exit(EXIT_FAILURE); 
	} 
	if (pid > 0) { 
		 exit(EXIT_SUCCESS);
	}
	instances[getpid()] = this;
	logger.log_entry("Entering Daemon mode", "INFO");
	logger.log_entry("started. PID: " + std::to_string(getpid()) + ".", "INFO");
}

// https://stackoverflow.com/questions/1599459/optimal-lock-file-method
void Daemon::create_lock_file(void) {
	// create file
	_lock_file_fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0666);
	if (_lock_file_fd < 0) {
		logger.log_entry("Error creating lock file", "ERROR");
		logger.log_entry("Quitting", "INFO");
		exit(EXIT_FAILURE);
	}
	// lock file
	if (flock(_lock_file_fd, LOCK_EX | LOCK_NB) == -1)
	{
		logger.log_entry("Error file locked", "ERROR");
		logger.log_entry("Quitting", "INFO");
		exit(EXIT_FAILURE);  
	}
}

void	Daemon::init_pollfd(void)
{
	poll_fd local_struct = {this->_socket_fd, POLLIN, 0};
	this->_poll_fds.clear();
	this->_poll_fds.push_back(local_struct);
}

void Daemon::server_listen(void) {
	int ret;
	
	this->init_pollfd();
	while (true)
	{
		ret = poll(this->_poll_fds.data(), this->_poll_fds.size(), -1);
		if (ret < 0) {
			perror("Poll error");
			return;
		}
		if (ret == 0)
			continue;
		if (this->fd_ready() == 1)
			return ;
	}
}

bool	Daemon::fd_ready( void )
{
	for (std::vector<struct pollfd>::iterator it = _poll_fds.begin(); it != _poll_fds.end(); ++it)
	{
		if (it->revents == 0)
			continue;
		if (it->fd == this->_socket_fd)
		{
			this->accept_communication();
			logger.log_entry("New user connected", "INFO");
			break ;
		}
		else
		{
			this->receive_communication(it);
			break ;
		}
	}
	return 0;
}

void	Daemon::accept_communication(void)
{
	int 	fd = 0;
	sock_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	fd = accept(this->_socket_fd, (sock_addr*)&client_addr, &client_addr_size);
	if (fd < 0)
	{
		if (errno != EWOULDBLOCK)
			perror("  accept() failed");
		return ;
	}
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
	{
		perror(" FCNTL failed");
		return ;
	}
	this->add_user(fd);
}

void	Daemon::receive_communication(std::vector<struct pollfd>::iterator it)
{
	char buffer[MSG_SIZE];
	int len;

	memset(buffer, 0, MSG_SIZE); //Iniciar buffer
	len = recv(it->fd, buffer, sizeof(buffer), 0);
	if (len < 0)
	{
		if (errno != EWOULDBLOCK)
			perror("  recv() failed");
		exit(EXIT_FAILURE);
	}
	if (len == 0)
	{
		this->delete_user(it);
		logger.log_entry("Usuario desconectado", "INFO");
		return ;
	}
	buffer[len-1] = 0; //El intro lo ponemos a cero
	if (buffer[0] != 0)
	{	
		// check if QUIT msg has been sent
		if (strcmp(buffer, "quit") == 0) // Character sensitive
		{
			logger.log_entry("Request quit.", "INFO");
			logger.log_entry("Quitting", "INFO");
			instances[getpid()]->~Daemon();
			exit(EXIT_SUCCESS);
		}
		// add here a log entry with the message
		std::string user_input("User input: ");
		user_input+= buffer;
		logger.log_entry(user_input, "LOG");
	}
}

void	Daemon::add_user(int fd)
{
	poll_fd local_struct = {fd, POLLIN, 0};
	this->_poll_fds.push_back(local_struct);
}

void	Daemon::delete_user(std::vector<struct pollfd>::iterator it)
{
	close(it->fd);
	this->_poll_fds.erase(it);
}

void Daemon::signal_handler(int signal) {
	// THIS DOES NOT WORK, USE SIGSET
	if (signal != 0) {
    	switch (signal) {
    	    case SIGINT:
    	        logger.log_entry("SIGINT received, shutting down", "INFO");
				instances[getpid()]->~Daemon();
    	        break;
    	    case SIGTERM:
    	        logger.log_entry("SIGTERM received, terminating", "INFO");
    	        break;
    	    case SIGHUP:
    	        logger.log_entry("SIGHUP received, reloading configuration", "INFO");
    	        break;
    	    default:
    	        logger.log_entry("Unknown signal received", "WARNING");
    	}
	}
	exit(EXIT_SUCCESS);
}