#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/user.h>

#include <errno.h>
#include <string.h>

#include <iostream>

struct shm_entry
{
    int id;
    void* addr;
    size_t size;
};

void case_1()
{
    shm_entry shme;
    shme.size = PAGE_SIZE*2;

    // int shmget(key_t key, size_t size, int shmflg)
    if (0 > (shme.id = shmget(IPC_PRIVATE, shme.size, IPC_EXCL)))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }

    void* p = (void*)0xb77da000;
    // void *shmat(int shmid, const void *shmaddr, int shmflg);
    if ((shme.addr = shmat(shme.id, p, SHM_RDONLY)) == (void*)-1)
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }

    char buf[shme.size];
    memcpy(buf, shme.addr, shme.size);
    std::cout << (char*)shme.addr << '\n';
    std::cout << "buf: " << buf << '\n';
    
    // int shmdt(const void *shmaddr);
    if (0 > shmdt(shme.addr))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }
}

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
// -lrt
void case_2()
{
    std::string shm_master("/master");
    int master_id;

    if (0 > (master_id = shm_open(shm_master.c_str(), O_RDWR, 0)))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }

//  std::string greeting("Wake UP red nose");
    char buf[1024] = {};

    if (0 > read(master_id, buf, sizeof(buf)))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
//        return;
    }

    std::cout << "buf: " << buf << "\n";

    std::string x;
    std::cin >> x;

//    if (0 > shm_unlink(shm_master.c_str()))
    if (0 > close(master_id))
    {
        std::cerr << __FILE__ << '[' << __LINE__ << ']' << strerror(errno) << '\n';
        return;
    }
}

int main(int argc, char* argv[])
{
//    case_1();
    case_2();
    return 0;
}

