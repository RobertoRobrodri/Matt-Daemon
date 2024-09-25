#include "daemon.hpp"

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

sock_in	Daemon::init_socket_struct(void)
{
	sock_in addr;
	// Init struct that the socket needs
	//  IPV4 addresses
	addr.sin_family      = AF_INET;
	//  Convert our port to a network address (host to network)
	addr.sin_port        = htons(4242);
	//  Our address as integer
	addr.sin_addr.s_addr = INADDR_ANY;
	return addr;
}

bool Daemon::init_server(void) {
	int opt = 1;
	sock_in addr = this->init_socket_struct();
	// Create the socket
	if ((this->_socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
			throw std::runtime_error("Failed to create socket");
	}
	std::cout << "Socket initialized" << std::endl;
	// Set socket options (reuse address)
	if (setsockopt(this->_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
			close(_socket_fd);
			throw std::runtime_error("Failed to set socket options");
	}
	// Bind the socket to the address and port
	if (bind(this->_socket_fd, (const sock_addr*)&addr, sizeof(addr)) == -1) {
			close(_socket_fd);
			throw std::runtime_error("Error binding socket");
	}
	// Start listening for connections, with MAX_CLIENTS backlog
	if (listen(this->_socket_fd, MAX_CLIENTS) == -1) {
			close(_socket_fd);
			throw std::runtime_error("Error starting to listen on socket");
	}
	std::cout << "Server is now listening" << std::endl;
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
	logger.log_entry("Entering daemon mode", "INFO");
	logger.log_entry("started, " + std::to_string(getpid()), "INFO");
}

// https://stackoverflow.com/questions/1599459/optimal-lock-file-method
void Daemon::create_lock_file(void) {
	// create file
	_lock_file_fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0666);
	if (_lock_file_fd < 0) {
		logger.log_entry("Error creating lock file", "ERROR");
		exit(EXIT_FAILURE);
	}
	// lock file
	if (flock(_lock_file_fd, LOCK_EX | LOCK_NB) == -1)
	{
		logger.log_entry("Error acquiring lock", "INFO");
		exit(EXIT_FAILURE);  
	}
}

void	Daemon::init_pollfd(void)
{
	memset(this->_poll_fds, 0, sizeof(this->_poll_fds));
	this->_poll_fds[0].fd 	   = this->_socket_fd;
	this->_poll_fds[0].events   = POLLIN;
}

void Daemon::server_listen(void) {
	int ret;
	
	this->init_pollfd();
	while (true)
	{
		ret = poll(this->_poll_fds, this->_active_fds, 0);
		if (ret < 0) {
			perror("Poll error");
			return;
		}
		if (ret == 0)
			continue;
		if (this->fd_ready() == 1)
			return ;
		// ping  users and disconnect inactive
	}
}

bool	Daemon::fd_ready( void )
{
	for (int i = 0; i < this->_active_fds; i++)
	{
		if (this->_poll_fds[i].revents == 0)
			continue;
		if (this->_poll_fds[i].fd == this->_socket_fd)
		{
			this->accept_communication();
			return 1;
		}
		else
		{
			this->receive_communication(i);
			return 1;
		}
	}
	return 0;
}

void	Daemon::accept_communication(void)
{
	int 	fd = 0;
	sock_in client_addr;
	socklen_t client_addr_size = sizeof(client_addr);
	char ip_addres[20];
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
	this->add_user(fd, client_addr);
}

void	Daemon::receive_communication(int poll_fd_pos)
{
	char buffer[MSG_SIZE];
	int len;

	std::cout << "Message received" << std::endl;
	memset(buffer, 0, MSG_SIZE); //Iniciar buffer con ceros porque mete mierda
	len = recv(this->_poll_fds[poll_fd_pos].fd, buffer, sizeof(buffer), 0);
	if (len < 0)
    {
		if (errno != EWOULDBLOCK)
			perror("  recv() failed");
        exit(EXIT_FAILURE);
    }
    if (len == 0)
    {
		std::cout << "  Connection closed" << std::endl;
		// Close fd >> Delete fd from poll >> Delete user from list_of_users
		this->delete_user(poll_fd_pos);
		return ;
    }
	buffer[len-1] = 0; //El intro lo ponemos a cero
	if (buffer[0] != 0)
	// add here a log entry with the message
		logger.log_entry(buffer, "INFO");
}

void	Daemon::add_user(int fd, sock_in client_addr)
{
	char ip_address[20];

	this->_poll_fds[this->_active_fds].fd = fd;
	this->_poll_fds[this->_active_fds].events = POLLIN;
	this->_active_fds++;
}

void	Daemon::delete_user(int poll_fd_pos)
{
	close(this->_poll_fds[poll_fd_pos].fd);
	for (int count = poll_fd_pos; count <= this->_active_fds - 1; count++)
		this->_poll_fds[count] = this->_poll_fds[count + 1];
	this->_poll_fds[this->_active_fds - 1].fd = 0;
	this->_poll_fds[this->_active_fds - 1].events = 0;
	this->_active_fds--;
}