// included from ../ipc.h to hide implementation of the template

namespace IPC {

template <class T, int NMutexes, int NConds>
SynchroMem<T, NMutexes, NConds> *SynchroMem<T, NMutexes, NConds>::create(const char *name)
{
    SynchroMem<T, NMutexes, NConds> *res = 0;

    int fd_sync = shm_open(name, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if (fd_sync < 0)
        return nullptr;

    int rv = ftruncate(fd_sync, sizeof(SynchroMem<T, NMutexes, NConds>));    
    if (rv == -1)
        return nullptr;

    void* addr_sync = mmap(0, sizeof(SynchroMem<T, NMutexes, NConds>), PROT_READ|PROT_WRITE, MAP_SHARED, fd_sync, 0);
    if (addr_sync == MAP_FAILED)
        return nullptr;

    res = new (addr_sync) SynchroMem<T, NMutexes, NConds>;

    res->Mutexes<NMutexes>::init(true);
    res->Conds<NConds>::init(true);

    return res;    
}

template <class T, int NMutexes, int NConds>
SynchroMem<T, NMutexes, NConds> *SynchroMem<T, NMutexes, NConds>::attach(const char *name)
{
    SynchroMem<T, NMutexes, NConds> *res = 0;

    int fd_sync = shm_open(name, O_RDWR, S_IRUSR|S_IWUSR);
    if (fd_sync < 0)
        return nullptr;

    void* addr_sync = mmap(0, sizeof(SynchroMem<T, NMutexes, NConds>), PROT_READ|PROT_WRITE, MAP_SHARED, fd_sync, 0);
    if (addr_sync == MAP_FAILED)
        return nullptr;

    res = static_cast<SynchroMem<T, NMutexes, NConds> *> (addr_sync);

    return res;    
}

template <class T, int NMutexes, int NConds>
void SynchroMem<T, NMutexes, NConds>::destroy(SynchroMem<T, NMutexes, NConds> *mem, const char *name)
{
    mem->Mutexes<NMutexes>::fini();
    mem->Conds<NConds>::fini();

    munmap(mem, sizeof(SynchroMem<T, NMutexes, NConds>));
    shm_unlink(name);
}

template <class T, int NMutexes, int NConds>
void SynchroMem<T, NMutexes, NConds>::detach(SynchroMem<T, NMutexes, NConds> *mem)
{
    munmap(mem, sizeof(SynchroMem<T, NMutexes, NConds>));
}



} // ns IPC
