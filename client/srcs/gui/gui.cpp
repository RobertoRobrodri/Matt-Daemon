#include "gui.hpp"

x11_gui::x11_gui( void ) {

	std::cout << "x11_gui Default constructor called" << std::endl;
}

x11_gui::x11_gui( std::string str ) {
	(void)str;
	std::cout << "x11_gui Parameter constructor called" << std::endl;
	return ;
}

x11_gui::x11_gui( const x11_gui & var ) {
	
	std::cout << "x11_gui Copy constructor called" << std::endl;
	*this = var;
	return ;
}

x11_gui::~x11_gui( void ) {
	
	std::cout << "x11_gui Destructor called" << std::endl;
	return ;
}

// Overloading
x11_gui & x11_gui::operator=(const x11_gui &tmp) {
	(void) tmp;
	std::cout << "x11_gui Operator equalizer called" << std::endl;
	return (*this);
}

std::ostream &operator<<(std::ostream& os, const x11_gui &tmp) {
	(void) tmp;
	os << std::endl << "x11_gui Operator output called" << std::endl;
	return (os);
}

// member functions

void    x11_gui::display_client( int socket_fd ) {
    XEvent event;
    std::string input_buffer;

    Display* display = XOpenDisplay(NULL);
    Window w = XCreateSimpleWindow(display, DefaultRootWindow(display), WINDOW_X, WINDOW_Y, WINDOW_HEIGHT, WINDOW_LENGTH, 1, BlackPixel(display, 0), WhitePixel(display, 0));
    XMapWindow(display, w);
    XSelectInput(display, w, ExposureMask | KeyPressMask | ButtonPressMask);
    while (true) {
        XNextEvent(display, &event);
        if (event.type == Expose) {
            XClearWindow(display, w);
            XDrawString(display, w, DefaultGC(display, 0), 100, 100, input_buffer.c_str(), input_buffer.size());
        }

        if (event.type == KeyPress) {
            char buffer[1];
            KeySym keysym;
            XLookupString(&event.xkey, buffer, sizeof(buffer), &keysym, nullptr);

            if (keysym == XK_Return) {
                send_command(socket_fd, input_buffer);
                input_buffer.clear();
                XClearWindow(display, w);
            } else {
                input_buffer += buffer[0];
                XClearWindow(display, w);
                XDrawString(display, w, DefaultGC(display, 0), 100, 100, input_buffer.c_str(), input_buffer.size());
            }
        }
    }
    XDestroyWindow(display, w);
    XCloseDisplay(display);
}