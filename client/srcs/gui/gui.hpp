# pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstring>

#define WINDOW_X 50
#define WINDOW_Y 50
#define WINDOW_LENGTH 400
#define WINDOW_HEIGHT 400

class   x11_gui {

    private:

	public:

		x11_gui ( void );
		x11_gui ( std::string str );
		x11_gui ( const x11_gui & var );
		~x11_gui ( void );

        x11_gui &operator=(const x11_gui &tmp);
        void    display_client(int socket_fd);
};

std::ostream &operator<<(std::ostream& os, const x11_gui &tmp);
void send_command(int sockfd, const std::string& command);