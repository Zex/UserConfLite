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
    struct mq_attr attr_customs;
    attr_customs.mq_maxmsg = 10;
    attr_customs.mq_msgsize = 1024;

    if ((mqd_t)-1 == (mq_customs = mq_open(mq_cust_name.c_str(), O_RDWR|O_CREAT, 0666, &attr_customs)))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }

    struct mq_attr attr_buf;

    if (0 > mq_getattr(mq_customs, &attr_buf))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }
 
    std::cout << "attr_buf:"
        << "\nflags: " << attr_buf.mq_flags
        << "\nmaxmsg: " << attr_buf.mq_maxmsg
        << "\nsize: " << attr_buf.mq_msgsize
        << "\ncurrentmsg: " << attr_buf.mq_curmsgs
        << "\n";

    std::string msgs[] = {"yellow bus", "blue bus", "huge aps"};

    for (size_t i = 0; i < 3; i++)
    {
        if (0 > mq_send(mq_customs, msgs[i].c_str(), msgs[i].size(), 0))
        {
            std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
            continue;//return;
        }
    if (0 > mq_getattr(mq_customs, &attr_buf))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }
 
    std::cout << "attr_buf:"
        << "\nflags: " << attr_buf.mq_flags
        << "\nmaxmsg: " << attr_buf.mq_maxmsg
        << "\nsize: " << attr_buf.mq_msgsize
        << "\ncurrentmsg: " << attr_buf.mq_curmsgs
        << "\n";
    }

    std::string buf;
    std::cin >> buf;

    if (0 > mq_close(mq_customs))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }

    if (0 > mq_unlink(mq_cust_name.c_str()))
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


