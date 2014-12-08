
#include <unistd.h>
#include "translate.h"

#include <sys/syscall.h>

void hello(int signo)
{
    LOG(__func__ << ':' << get_signame(signo))
}

//int sigsuspend(const sigset_t *mask);
void case1(int argc, char* argv[])
{
    sigset_t msk;
//    LOG(sizeof(msk.__val))
//    LOG(sigmask(SIGQUIT))
//    LOG((SIGQUIT))

//    for (unsigned long int i = SIGHUP; i < SIGRTMAX; i++)
    {
        msk.__val[SIGINT] = sigmask(SIGINT);
    }

    signal(SIGINT, hello);

    if (EFAULT == sigsuspend(&msk))
    {
        LOG_ERR(strerror(errno))
        return ;
    }

    LOG("Got signal\n")
}

#include <sys/uio.h>

int case14(int argc, char* argv[])
{
    int fd = STDOUT_FILENO, iov_nr = 2;
    struct iovec iov[iov_nr];

    char* base_0 = "start...";
    char* base_1 = "end...";

    iov[0].iov_base = base_0;
    iov[0].iov_len = 9;
    iov[1].iov_base = base_1;
    iov[1].iov_len = 7;

    if (0 > writev(fd, iov, iov_nr)); 
    {
        LOG_ERR(strerror(errno))
        return 0;
    }

//    std::cout << "iovec: "
//        << "\niov_base: " << (char*)iov.iov_base
//        << "\niov_len: " << iov.iov_len;
    std::cout << "\n******************************************\n";
    return 0;
}

int main(int argc, char* argv[])
{
//    case1(argc, argv);
    case14(argc, argv);
    return 0;
}
