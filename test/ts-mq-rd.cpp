/*
 * g++ -lrt -o ts-mq ts-mq.cpp 
 */
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>

#include <errno.h>
#include <string.h>

#include <iostream>

void case_1()
{
    std::string mq_cust_name("/customs");

    mqd_t mq_customs;

    if ((mqd_t)-1 == (mq_customs = mq_open(mq_cust_name.c_str(), O_RDWR)))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }

    struct mq_attr attr_customs;

    if (0 > mq_getattr(mq_customs, &attr_customs))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }
 
    std::cout << "attr_customs:"
        << "\nflags: " << attr_customs.mq_flags
        << "\nmaxmsg: " << attr_customs.mq_maxmsg
        << "\nsize: " << attr_customs.mq_msgsize
        << "\ncurrentmsg: " << attr_customs.mq_curmsgs
        << "\n";

//    std::string msgs[] = {"yellow bus", "blue bus", "huge aps"};

    for (size_t i = 0; i < 3; i++)
    {
        char buf[1024] = {};

        if (0 > mq_receive(mq_customs, buf, sizeof(buf), 0))
        {
            std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
            continue;//return;
        }

        std::cout << "buf: " << buf << "\n";
    }

    std::string buf;
    std::cin >> buf;

    if (0 > mq_close(mq_customs))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }
}

int main(int argc, char* argv[])
{
    case_1();
    return 0;
}


