#include <errno.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

//int main()
//{
//    int fd;
//
//    if (0 > (fd = socket(AF_INET, SOCK_DGRAM, 0)))
//    {
//        std::cerr << strerror(errno) << '\n';
//        return 0;
//    }
//
//    struct sockaddr_in peer;
//
//    peer.sin_family = AF_INET;
//    peer.sin_port = htons(3456);
//    peer.sin_addr.s_addr = inet_addr("192.168.0.116");
//
//    char buf[512] = {};
//    strcpy(buf, "hello server ......................................................");
//
//    if (0 > sendto(fd, buf, sizeof(buf), 0, (struct sockaddr *)&peer, sizeof(peer)))
//    {
//        std::cerr << "buffer[out]: " << strerror(errno) << '\n';
//    }
//    else
//    {
//        std::cout << "buffer[out]: " << "ok" << '\n';
//       
//        memset(buf, '\0', sizeof(buf));
//        socklen_t len;
// 
//        struct sockaddr peer_rcv;
//
//        if (0 > recvfrom(fd, buf, sizeof(buf), 0, 
//           (struct sockaddr *)&peer_rcv, &len))
//        {
//            std::cerr << "buffer[reply]: " << strerror(errno) << '\n';
//        }
//        else
//        {
//            std::cout << "buffer[reply]: " << buf << '\n';
//        }
//    }
//
//    close(fd);
//    return 0;
//}

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

    if (0 > (fd = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        std::cerr << strerror(errno) << '\n';
        return;
    }

//  struct sockaddr_un here, peer;
    struct sockaddr_in here, peer;

    here.sin_family = AF_INET;
    here.sin_port = htons(3456);
    here.sin_addr.s_addr = INADDR_ANY;
    bind(fd, (struct sockaddr *)&here, sizeof(here));

//   here.sun_family = AF_UNIX;
//   strcpy(here.sun_path, "./buffer");
//   unlink(here.sun_path);
//   bind(fd, (struct sockaddr *)&here, sizeof(here.sun_family) + strlen(here.sun_path));

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

    ev_epoll.events = EPOLLIN|EPOLLOUT|EPOLLET;
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
                            char buf[512] = {};
                            socklen_t len;
    
                            if (0 > sendto(ev_recv[i].data.fd, buf, sizeof(buf), 0, 
                                    (struct sockaddr *)&peer, sizeof(peer)))
                            {
                                std::cerr << "buffer[in]: " << strerror(errno) << '\n'; continue;
                            }
                            else
                            {
                                std::cout << "buffer[in]: " << buf << '\n';
                            }
                        }
                        else if (ev_recv[i].events & EPOLLOUT)
                        {
                            char buf[512] = {};
                            socklen_t len;

                            if (0 > recvfrom(ev_recv[i].data.fd, buf, sizeof(buf), 0, 
                                (struct sockaddr *)&peer, &len))
                            {
                                std::cerr << "buffer[out]: " << strerror(errno) << '\n'; continue;
                            }
                            else
                            {
                                std::cout << "buffer[out]: " << buf << '\n';

                                if (0 > sendto(ev_recv[i].data.fd, buf, sizeof(buf), 0, 
                                        (struct sockaddr *)&peer, sizeof(peer)))
                                {
                                    std::cerr << "buffer[reply]: " << strerror(errno) << '\n'; continue;
                                }
                                else
                                {
                                    std::cout << "buffer[reply]: " << buf << '\n';
                                }
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
