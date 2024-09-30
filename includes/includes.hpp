#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <syslog.h>
#include <string>
#include <iostream>
#include <cerrno>
#include <string.h>
#include "../srcs/reporter/reporter.hpp"

extern Tintin_reporter logger;