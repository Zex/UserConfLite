#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/user.h>
#include "errhdr.h"

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
        LOG_ERR("");
        return;
    }

    void* p = (void*)0xb77da000;
    // void *shmat(int shmid, const void *shmaddr, int shmflg);
    if ((shme.addr = shmat(shme.id, p, SHM_RDONLY)) == (void*)-1)
    {
        LOG_ERR("");
        return;
    }

    char buf[shme.size];
    memcpy(buf, shme.addr, shme.size);
    std::cout << (char*)shme.addr << '\n';
    std::cout << "buf: " << buf << '\n';
    
    // int shmdt(const void *shmaddr);
    if (0 > shmdt(shme.addr))
    {
        LOG_ERR("");
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
        LOG_ERR("");
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
        LOG_ERR("");
        return;
    }
}

int main(int argc, char* argv[])
{
//    case_1();
    case_2();
    return 0;
}

