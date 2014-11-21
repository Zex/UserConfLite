#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/resource.h>


//------------------------------------------------------------------------------------

#include <errno.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <sys/stat.h>
#include <sys/un.h>

//#define __TS_SOCK_
#define __TS_DEV_

static int run;
static int fd;
static int fd_epoll;

void sigquit_cb(int signo)
{
    run = 0;
    close(fd_epoll);
    close(fd);
}

void case1()
{
    signal(SIGQUIT, sigquit_cb);
    signal(SIGINT, sigquit_cb);
    run = 1;
//...........................................................

#ifdef __TS_SOCK_
    if (0 > (fd = socket(AF_INET, SOCK_DGRAM, 0)))
#else
    if (0 > (fd = open("/dev/input/event0", O_RDONLY)))
#endif
    {
        std::cerr << strerror(errno) << '\n';
        return;
    }

//------------------------------------------------
#ifdef __TS_SOCK_
    struct sockaddr_in here, peer;

    here.sin_family = AF_INET;
    here.sin_port = htons(3456);
    here.sin_addr.s_addr = INADDR_ANY;
    bind(fd, (struct sockaddr *)&here, sizeof(here));
#endif
//------------------------------------------------
//  struct sockaddr_un here, peer;
//   here.sun_family = AF_UNIX;
//   strcpy(here.sun_path, "./buffer");
//   unlink(here.sun_path);
//   bind(fd, (struct sockaddr *)&here, sizeof(here.sun_family) + strlen(here.sun_path));
//------------------------------------------------

//    setnonblocking(fd);
//...........................................................

    if (0 > (fd_epoll = epoll_create(1)))
    {
        std::cerr << strerror(errno) << '\n';
        sigquit_cb(SIGQUIT);
        return;
    }
//...........................................................
    struct epoll_event ev_epoll;

    ev_epoll.events = EPOLLIN|EPOLLPRI|EPOLLET;
    ev_epoll.data.fd = fd;

    if (0 > epoll_ctl(fd_epoll, EPOLL_CTL_ADD, fd, &ev_epoll))
    {
        std::cerr << strerror(errno) << '\n';
        sigquit_cb(SIGQUIT);
        return;
    }

//...........................................................
    int ret;
    int EV_RECV_NR = 20;

    struct epoll_event ev_recv[EV_RECV_NR];

    while (run)
    {
        switch (ret = epoll_wait(fd_epoll, ev_recv, EV_RECV_NR, -1))
        {
            case -1:
            {
                std::cerr << strerror(errno);
                break;
            }
            case 0:
            {
                std::cout << "no fd is ready\n";
                break;
            }
            default:
            {
                for (int i = 0; i < ret; i++)
                {
                    if (ev_recv[i].data.fd == fd)
                    {
                        if (ev_recv[i].events & EPOLLIN)
                        {
#ifdef __TS_SOCK_
                            char buf[512] = {};
                            socklen_t len;
   
                            if (0 > sendto(ev_recv[i].data.fd, buf, sizeof(buf), 0, 
                                    (struct sockaddr *)&peer, sizeof(peer)))
#else // __TS_DEV_
                            char buf[32] = {};

                            if (0 > read(ev_recv[i].data.fd, buf, sizeof(buf)))
#endif
                            {
                                std::cerr << "buffer[in]: " << strerror(errno) << '\n'; continue;
                            }
                            else
                            {
                                std::cout << "buffer[in]: ";
                                std::cout << buf[0];
//                                for (size_t i = 0; i < sizeof(buf); i++)
//                                    std::cout << (int)buf[i] << ' ';
                                std::cout << "\n";
                            }
                        }
                        else if (ev_recv[i].events & EPOLLOUT)
                        {
                            char buf[512] = {};
#ifdef __TS_SOCK_
                            socklen_t len;

                            if (0 > recvfrom(ev_recv[i].data.fd, buf, sizeof(buf), 0, 
                             (struct sockaddr *)&peer, &len))
#else // __TS_DEV_
                            if (0 > read(ev_recv[i].data.fd, buf, sizeof(buf)))
#endif
                            {
                                std::cerr << "buffer[out]: " << strerror(errno) << '\n'; continue;
                            }
                            else
                            {
                                std::cout << "buffer[out]: " << buf << '\n';
                            
#ifdef __TS_SOCK_
                                if (0 > sendto(ev_recv[i].data.fd, buf, sizeof(buf), 0, 
                                        (struct sockaddr *)&peer, sizeof(peer)))
                                {
                                    std::cerr << "buffer[reply]: " << strerror(errno) << '\n'; continue;
                                }
                                else
                                {
                                    std::cout << "buffer[reply]: " << buf << '\n';
                                }
#endif
                            }
                        }
                        else
                        {
                            std::cout << "fd: " << ev_recv[i].data.fd << '\n';
                            std::cout << "event: " << ev_recv[i].events << '\n';
                        }
                    }
                    else
                    {
                        std::cout << "unknown fd: " << ev_recv[i].data.fd << '\n';
                    }
                }
            }
        }
    }
}


int main(int argc, char* argv[])
{
//    server(argc, argv);
    case1();

    std::cout << " case1 quit\n";
    return 0;
}

