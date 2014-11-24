#ifndef __TRANSLATE_H_
#define __TRANSLATE_H_

#include "errhdr.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <utmp.h>
#include <linux/limits.h>
#include <sys/inotify.h>

std::string get_family_name(int fa);
std::string get_sock_level(int so);
std::string get_sock_type(int so);
std::string get_ai_flag(int f);
std::string get_utmp_type(int t);
std::string get_inotify_mask(uint32_t m);

#endif // __TRANSLATE_H_
