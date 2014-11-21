#ifndef __ERRHDR_H_
#define __ERRHDR_H_

#include <errno.h>
#include <string.h>

#include <iostream>

#define _OUT_ std::cout
#define _ERR_ std::cerr

#define LOG(s) \
    _OUT_ << __PRETTY_FUNCTION__ << '[' << __FILE__ << ':' << __LINE__ << "]: " <<  s << '\n';

#define LOG_ERR(s) \
    _ERR_ << __PRETTY_FUNCTION__ << '[' << __FILE__ << ':' << __LINE__ << "]: " <<  s << '\n';


#endif // __ERRHDR_H_
