#include <unistd.h>
#include <arpa/inet.h>
#include "translate.h"
// int getaddrinfo(const char *node, const char *service,
// const struct addrinfo *hints,
// struct addrinfo **res);
// 
// void freeaddrinfo(struct addrinfo *res);
// 
// const char *gai_strerror(int errcode);
// 
// struct addrinfo {
// int              ai_flags;
// int              ai_family;
// int              ai_socktype;
// int              ai_protocol;
// socklen_t        ai_addrlen;
// struct sockaddr *ai_addr;
// char            *ai_canonname;
// struct addrinfo *ai_next;
// };
// struct protoent {
// char  *p_name;       /* official protocol name */
// char **p_aliases;    /* alias list */
// int    p_proto;      /* protocol number */
// }

// g++ -o ts-dispatch ts-dispatch.cpp translate.cpp translate.h errhdr.h

int case1(int argc, char* argv[])
{
    LOG("")
    struct addrinfo *ai, hint;
    int ret;

    hint.ai_flags = AI_ALL|AI_PASSIVE|AI_CANONNAME|AI_V4MAPPED|AI_ADDRCONFIG;
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = 0;
    hint.ai_protocol = 0;

    if (ret = getaddrinfo("localhost", 0, &hint, &ai))
    {
        LOG_ERR(gai_strerror(ret))
        return 0;
    }

    for (struct addrinfo *cur = ai; cur != 0; cur = cur->ai_next)
    {
        std::cout
            << "\nflags: " << get_ai_flag(cur->ai_flags)
            << "\nfamily: " << get_family_name(cur->ai_family)
            << "\nsocktype: " << get_sock_type(cur->ai_socktype)
            << "\nprotocol: " << getprotobynumber(cur->ai_protocol)->p_name
            << "\naddrlen: " << cur->ai_addrlen;
        if (cur->ai_addr)
            std::cout << "\naddr.sin_family:" << get_family_name(((struct sockaddr_in*)cur->ai_addr)->sin_family)
            << "\naddr.sin_port:" << ntohs(((struct sockaddr_in*)cur->ai_addr)->sin_port)
            << "\naddr.sin_addr:" << inet_ntoa(((struct sockaddr_in*)(cur->ai_addr))->sin_addr);
        if (cur->ai_canonname)
            std::cout << "\ncanonname: " << cur->ai_canonname;

        std::cout << "\n******************************************\n";
    }

    freeaddrinfo(ai);

    return 0;
}

#include <mntent.h>
// 
// FILE *setmntent(const char *filename, const char *type);
// struct mntent *getmntent(FILE *fp);
// int addmntent(FILE *fp, const struct mntent *mnt);
// int endmntent(FILE *fp);
// char *hasmntopt(const struct mntent *mnt, const char *opt);
// struct mntent {
// char *mnt_fsname;   /* name of mounted file system */
// char *mnt_dir;      /* file system path prefix */
// char *mnt_type;     /* mount type (see mntent.h) */
// char *mnt_opts;     /* mount options (see mntent.h) */
// int   mnt_freq;     /* dump frequency in days */
// int   mnt_passno;   /* pass number on parallel fsck */
// };

int case2(int argc, char* argv[])
{
    LOG("")
    FILE *fd;

    if (!(fd = setmntent("/etc/mtab", "r")))
    {
        LOG_ERR(strerror(errno));
        return 0;
    }

    struct mntent* ent;

    while ((ent = getmntent(fd)))
    {
        std::cout
            << "\nmnt_fsname: " << ent->mnt_fsname
            << "\nmnt_dir: " << ent->mnt_dir
            << "\nmnt_type: " << ent->mnt_type
            << "\nmnt_opts: " << ent->mnt_opts
            << "\nmnt_freq: " << ent->mnt_freq
            << "\nmnt_passno: " << ent->mnt_passno;
        std::cout << "\n******************************************\n";
    }

    endmntent(fd);

    return 0;
}

#include <grp.h>
// struct group *getgrent(void);
// void setgrent(void);
// void endgrent(void);
// struct group {
// char   *gr_name;       /* group name */
// char   *gr_passwd;     /* group password */
// gid_t   gr_gid;        /* group ID */
// char  **gr_mem;        /* group members */
// };

int case3(int argc, char* argv[])
{
    LOG("")

    struct group *grp;

    setgrent();

    while ((grp = getgrent()))
    {
        std::cout
            << "\ngr_name:" << grp->gr_name
            << "\ngr_passwd:" << grp->gr_passwd
            << "\ngr_gid:" << grp->gr_gid
            << "\ngr_mem:";
        for (char** p = grp->gr_mem; *p != 0; p++)
            std::cout << *p << ",";
        std::cout << "\n******************************************\n";
    }

    endgrent();

    return 0;
}

#include <ifaddrs.h>
// struct ifaddrs {
// struct ifaddrs  *ifa_next;    /* Next item in list */
// char            *ifa_name;    /* Name of interface */
// unsigned int     ifa_flags;   /* Flags from SIOCGIFFLAGS */
// struct sockaddr *ifa_addr;    /* Address of interface */
// struct sockaddr *ifa_netmask; /* Netmask of interface */
// union {
// struct sockaddr *ifu_broadaddr;
// /* Broadcast address of interface */
// struct sockaddr *ifu_dstaddr;
// /* Point-to-point destination address */
// } ifa_ifu;
// #define              ifa_broadaddr ifa_ifu.ifu_broadaddr
// #define              ifa_dstaddr   ifa_ifu.ifu_dstaddr
// void            *ifa_data;    /* Address-specific data */
// };
int case4(int argc, char* argv[])
{
    struct ifaddrs *ifa_blk;

    if (0 > getifaddrs(&ifa_blk))
    {
        LOG_ERR(strerror(errno));
        return 0;
    }

    struct ifaddrs *cur = ifa_blk;

    while (cur)
    {
        if (! cur->ifa_addr)
            continue;

        std::cout << "\nifa_name: " << cur->ifa_name;
        std::cout << "\nifa_addr.sin_family:" << get_family_name(((struct sockaddr_in*)cur->ifa_addr)->sin_family)
                  << "\nifa_addr.sin_port:" << ntohs(((struct sockaddr_in*)cur->ifa_addr)->sin_port)
                  << "\nifa_addr.sin_addr:" << inet_ntoa(((struct sockaddr_in*)(cur->ifa_addr))->sin_addr)
                  << "\n";
        if (cur->ifa_broadaddr)
        std::cout << "\nifu_broadaddr.sin_family:" << get_family_name(((struct sockaddr_in*)cur->ifa_broadaddr)->sin_family)
                  << "\nifu_broadaddr.sin_port:" << ntohs(((struct sockaddr_in*)cur->ifa_broadaddr)->sin_port)
                  << "\nifu_broadaddr.sin_addr:" << inet_ntoa(((struct sockaddr_in*)(cur->ifa_broadaddr))->sin_addr)
                  << "\n";
        if (cur->ifa_dstaddr)
        std::cout << "\nifu_dstaddr.sin_family:" << get_family_name(((struct sockaddr_in*)cur->ifa_dstaddr)->sin_family)
                  << "\nifu_dstaddr.sin_port:" << ntohs(((struct sockaddr_in*)cur->ifa_dstaddr)->sin_port)
                  << "\nifu_dstaddr.sin_addr:" << inet_ntoa(((struct sockaddr_in*)(cur->ifa_dstaddr))->sin_addr)
                  << "\n";
        std::cout << "*********************************************************************\n";
        cur = cur->ifa_next;
    }

    freeifaddrs(ifa_blk);

    return 0;
}

// struct rpcent *getrpcent(void);
// struct rpcent *getrpcbyname(char *name);
// struct rpcent *getrpcbynumber(int number);
// void setrpcent(int stayopen);
// void endrpcent(void);
// struct rpcent {
// char  *r_name;     /* name of server for this RPC program */
// char **r_aliases;  /* alias list */
// long   r_number;   /* RPC program number */
// };

int case5(int argc, char* argv[])
{
    struct rpcent *ent;

    setrpcent(1);

    while ((ent = getrpcent()))
    {
        std::cout
            << "\nr_name: " << ent->r_name
            << "\nr_number: " << ent->r_number
            << "\nr_name: ";
        for (char** p = ent->r_aliases; *p != 0; p++)
            std::cout << *p << ",";
        std::cout << "\n******************************************\n";
    }

    endrpcent();

    return 0;
}

#include <sys/sysinfo.h>

// int get_nprocs(void);
// int get_nprocs_conf(void);
int case6(int argc, char* argv[])
{
    std::cout
        << "\nnumber of processors: " << get_nprocs_conf()
        << "\nnumber of available processors: " << get_nprocs();
    std::cout << "\n******************************************\n";

    return 0;
}

// #include <utmp.h>
// struct utmp *getutent(void);
// struct utmp *getutid(struct utmp *ut);
// struct utmp *getutline(struct utmp *ut);
// struct utmp *pututline(struct utmp *ut);
// 
// void setutent(void);
// void endutent(void);
// 
// int utmpname(const char *file);
// 
// struct utmp
// {
// short int ut_type;		/* Type of login.  */
// pid_t ut_pid;			/* Process ID of login process.  */
// char ut_line[UT_LINESIZE];	/* Devicename.  */
// char ut_id[4];		/* Inittab ID.  */
// char ut_user[UT_NAMESIZE];	/* Username.  */
// char ut_host[UT_HOSTSIZE];	/* Hostname for remote login.  */
// struct exit_status ut_exit;	/* Exit status of a process marked
// 				   as DEAD_PROCESS.  */
// /* The ut_session and ut_tv fields must be the same size when compiled
// 32- and 64-bit.  This allows data files and shared memory to be
// shared between 32- and 64-bit applications.  */
// #ifdef __WORDSIZE_TIME64_COMPAT32
// int32_t ut_session;		/* Session ID, used for windowing.  */
// struct
// {
// int32_t tv_sec;		/* Seconds.  */
// int32_t tv_usec;		/* Microseconds.  */
// } ut_tv;			/* Time entry was made.  */
// #else
// long int ut_session;		/* Session ID, used for windowing.  */
// struct timeval ut_tv;		/* Time entry was made.  */
// #endif
// 
// int32_t ut_addr_v6[4];	/* Internet address of remote host.  */
// char __unused[20];		/* Reserved for future use.  */
// };
// 
// struct exit_status {              /* Type for ut_exit, below */
// short int e_termination;      /* Process termination status */
// short int e_exit;             /* Process exit status */
// };

int case7(int argc, char* argv[])
{
    struct utmp *ent;
    char buf[128];

    setutent();

    while ((ent = getutent()))
    {
        std::cout
            << "\nut_type: " << get_utmp_type(ent->ut_type)
            << "\nut_pid: " << ent->ut_pid
            << "\nut_line: " << ent->ut_line
            << "\nut_id: " << ent->ut_id
            << "\nut_user: " << ent->ut_user
            << "\nut_host: " << ent->ut_host
            << "\nut_exit.e_termination: " << get_utmp_type(ent->ut_exit.e_termination)
            << "\nut_exit.e_exit: " << get_utmp_type(ent->ut_exit.e_exit)
            << "\nut_session: " << ent->ut_session
            << "\nut_tv.tv_sec: " << ent->ut_tv.tv_sec
            << "\nut_tv.tv_usec: " << ent->ut_tv.tv_usec
            << "\nut_addr_v6: ";

        if (inet_ntop(AF_INET, (in_addr*)&ent->ut_addr_v6[0], buf, 128))
            std::cout << buf;
// << "\nut_addr_v6: " << inet_ntoa(*(in_addr*)&ent->ut_addr_v6[0])
        std::cout << "\n__unused: " << ent->__unused
            << "\nlogin time: " << asctime(localtime(&ent->ut_tv.tv_sec));
        std::cout << "\n******************************************\n";
    }

    endutent();

    return 0;
}

// struct hostent *getipnodebyname(const char *name, int af, int flags, int *error_num);
// struct hostent *getipnodebyaddr(const void *addr, size_t len, int af, int *error_num);
// void freehostent(struct hostent *ip);
// 
// struct hostent {
// char  *h_name;
// char **h_aliases;
// int    h_addrtype;
// int    h_length;
// char **h_addr_list;
// };
int case8(int argc, char* argv[])
{
    struct hostent *ent;

// if ((ent = getipnodebyname()))

    return 0;
}
#include <sys/epoll.h>

// int inotify_init(void);
// int inotify_init1(int flags);
// int inotify_add_watch(int fd, const char *pathname, uint32_t mask)
// int inotify_rm_watch(int fd, int wd);
//           struct inotify_event {
//               int      wd;       /* Watch descriptor */
//               uint32_t mask;     /* Mask of events */
//               uint32_t cookie;   /* Unique cookie associating related
//                                     events (for rename(2)) */
//               uint32_t len;      /* Size of name field */
//               char     name[];   /* Optional null-terminated name */
//           };

int case9(int argc, char* argv[])
{
    int fd;

    if (0 > (fd = inotify_init()))
    {
        LOG_ERR(strerror(errno))
        return 0;
    }

    int wd;

    if (0 > (wd = inotify_add_watch(fd, "/tmp/buf", IN_ALL_EVENTS)))
    {
        LOG_ERR(strerror(errno))
        close(fd);
        return 0;
    }

//...........................................................
    int fd_epoll;
    if (0 > (fd_epoll = epoll_create(1)))
    {
        LOG_ERR(strerror(errno))
        close(fd);
        close(wd);
        return 0;
    }

    struct epoll_event ev_epoll;
    int ret = 0, EV_RECV_NR = 20;

    ev_epoll.events = EPOLLOUT|EPOLLIN|EPOLLPRI|EPOLLET;
    ev_epoll.data.fd = wd;

    if (0 > epoll_ctl(fd_epoll, EPOLL_CTL_ADD, wd, &ev_epoll))
    {
        LOG_ERR(strerror(errno))
        close(fd);
        close(wd);
        close(fd_epoll);
        return 0;
    }
//...........................................................
    while (1)
    {
        LOG("Start ...")
        struct epoll_event ev_recv[EV_RECV_NR];

        switch (ret = epoll_wait(fd_epoll, ev_recv, EV_RECV_NR, -1))
        {
            case -1:
            {
                LOG_ERR(strerror(errno))
                break;
            }
            case 0:
            {
                LOG("No fd is ready");
                break;
            }
            default:
            {
                for (int i = 0; i < ret; i++)
                {
                    if (ev_recv[i].data.fd == wd)
                    {
                        LOG("ev_recv[i].data.fd == wd")
                        char buf[sizeof(struct inotify_event) + NAME_MAX + 1] = {};
                        int ret;

                        if (0 > (ret = read(fd, buf, sizeof(buf))))
                        {
                            LOG_ERR(strerror(errno))
                            return 0;
                        }
                    
                        if (ret < sizeof(struct inotify_event))
                        {
                            LOG_ERR("read ev: " << ret << " expeted size >= " << sizeof(struct inotify_event))
                            return 0;
                        }
                    
                        struct inotify_event *ev = (struct inotify_event*)buf;
                    
                        std::cout
                            << "\nwd: " << ev->wd
                            << "\nmask: " << get_inotify_mask(ev->mask)
                            << "\ncookie: " << ev->cookie
                            << "\nlen: " << ev->len
                            << "\nname: ";
                        memcpy(ev->name, buf+sizeof(struct inotify_event), ev->len);
                        if (0 < ev->len)
                            std::cout << ev->name;
                        std::cout << "\n";
                        std::cout << "\n******************************************\n";

                        switch (ev->mask)
                        {
                            case IN_MOVE_SELF:
                            { 
                                break;
                            }
                            // ...
                            default:;
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

    inotify_rm_watch(fd, wd);
    close(fd);
    close(wd);

    return 0;
}


int main(int argc, char* argv[])
{
    case1(argc, argv);
    case2(argc, argv);
    case3(argc, argv);
    case4(argc, argv);
    case5(argc, argv);
    case6(argc, argv);
    case7(argc, argv);
    case8(argc, argv);
    case9(argc, argv);
    return 0;
}


