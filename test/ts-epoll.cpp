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


#define MAXBUF 1024
#define MAXEPOLLSIZE 10000

/*
setnonblocking - 
*/
int setnonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1) {
        return -1;
    }
    return 0;
}

/*
handle_message - ����ÿ�� socket �ϵ���Ϣ�շ�
*/
int handle_message(int new_fd)
{
    char buf[MAXBUF + 1];
    int len;
    /* ��ʼ����ÿ���������ϵ������շ� */
    bzero(buf, MAXBUF + 1);
    /* ���տͻ��˵���Ϣ */
    len = recv(new_fd, buf, MAXBUF, 0);
    if (len > 0)
        printf
            ("%d������Ϣ�ɹ�:'%s'����%d���ֽڵ�����\n",
             new_fd, buf, len);
    else {
        if (len < 0)
            printf
                ("��Ϣ����ʧ�ܣ����������%d��������Ϣ��'%s'\n",
                 errno, strerror(errno));
        close(new_fd);
        return -1;
    }
    /* ����ÿ���������ϵ������շ����� */
    return len;
}

// filename: epoll-server.c
int server(int argc, char **argv)
{
    int listener, new_fd, kdpfd, nfds, n, ret, curfds;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int myport, lisnum;
    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];
    struct rlimit rt;

    if (argv[1])
        myport = atoi(argv[1]);
    else
        myport = 7838;

    if (argv[2])
        lisnum = atoi(argv[2]);
    else
        lisnum = 2;

    /* ����ÿ����������򿪵�����ļ��� */
    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1) {
        perror("setrlimit");
        exit(1);
    }
    else printf("����ϵͳ��Դ�����ɹ���\n");

    /* ���� socket ���� */
    if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    } else
        printf("socket �����ɹ���\n");

    setnonblocking(listener);

    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(myport);
    if (argv[3])
        my_addr.sin_addr.s_addr = inet_addr(argv[3]);
    else
        my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind
        (listener, (struct sockaddr *) &my_addr, sizeof(struct sockaddr))
        == -1) {
        perror("bind");
        exit(1);
    } else
        printf("IP ��ַ�Ͷ˿ڰ󶨳ɹ�\n");

    if (listen(listener, lisnum) == -1) {
        perror("listen");
        exit(1);
    } else
        printf("��������ɹ���\n");

    /* ���� epoll ������Ѽ��� socket ���뵽 epoll ������ */
    kdpfd = epoll_create(MAXEPOLLSIZE);
    len = sizeof(struct sockaddr_in);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listener;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0) {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", listener);
        return -1;
    } else
        printf("���� socket ���� epoll �ɹ���\n");
    curfds = 1;
    while (1) {
        /* �ȴ����¼����� */
        nfds = epoll_wait(kdpfd, events, curfds, -1);
        if (nfds == -1) {
            perror("epoll_wait");
            break;
        }
        /* ���������¼� */
        for (n = 0; n < nfds; ++n) {
            if (events[n].data.fd == listener) {
                new_fd = accept(listener, (struct sockaddr *) &their_addr,
                                &len);
                if (new_fd < 0) {
                    perror("accept");
                    continue;
                } else
                    printf("�����������ڣ� %d:%d�� ����� socket Ϊ:%d\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);

                setnonblocking(new_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_fd;
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_fd, &ev) < 0) {
                    fprintf(stderr, "�� socket '%d' ���� epoll ʧ�ܣ�%s\n",
                            new_fd, strerror(errno));
                    return -1;
                }
                curfds++;
            } else {
                ret = handle_message(events[n].data.fd);
                if (ret < 1 && errno != 11) {
                    epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,
                              &ev);
                    curfds--;
                }
            }
        }
    }
    close(listener);
    return 0;
}
/*
gcc -Wall epoll-server.c -o server

sudo ./server 7838 1
*/

//------------------------------------------------------------------------------------

#include <errno.h>
#include <string.h>
#include <iostream>
#include <signal.h>
#include <sys/stat.h>
#include <sys/un.h>

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

