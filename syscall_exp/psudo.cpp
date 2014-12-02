
#include "translate.h"

#include <sys/signal.h>
#include <sys/syscall.h>

void hello(int signo)
{
    LOG(__func__)
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

int main(int argc, char* argv[])
{
    case1(argc, argv);
    return 0;
}
