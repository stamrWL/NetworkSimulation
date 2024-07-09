#ifndef CLASSRWMutex
#define CLASSRWMutex
#include <mutex>
#include <memory>
#include <condition_variable>
#include <shared_mutex>
class RWMutex;
class ThreadRWMutex;

class ThreadRWMutex{
private:
	int hasShare;
	int hasUnique;
	bool beforeRead;
    RWMutex* F;
public:
    ThreadRWMutex(){
        hasShare = hasUnique = 0;
        beforeRead = false;
        F = nullptr;
    };

	void shareLock(RWMutex &father);
	void releaseShareLock(RWMutex &father);
	void uniqueLock(RWMutex &father);
	void releaseUniqueLock(RWMutex &father);
    void releaseAll();
};

class RWMutex{
protected:
	std::mutex UpdateLock;
	int RCount;
	int WCount;
	std::condition_variable cv;
public:
	thread_local static ThreadRWMutex rwMutex;
    RWMutex(){RCount = WCount = 0;}
    friend class ThreadRWMutex;
};

#endif