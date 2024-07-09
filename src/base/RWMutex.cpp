#include "RWMutex.h"

thread_local ThreadRWMutex RWMutex::rwMutex;

void ThreadRWMutex::shareLock(RWMutex &father){
	F = &father;
	if(hasUnique == 0 && hasShare == 0){
		std::unique_lock<std::mutex> lk(father.UpdateLock);
		father.cv.wait(lk, [&father](){return father.WCount == 0;});
		father.RCount += 1;
		beforeRead = false;
	}
    // if(hasUnique != 0 && !beforeRead){
    //     beforeRead = true;
    // }
	hasShare += 1;
} 

void ThreadRWMutex::uniqueLock(RWMutex &father){
	F = &father;
	if(hasUnique == 0)
	{
		std::unique_lock<std::mutex> lk(father.UpdateLock);
		if(hasShare != 0 && !beforeRead){
			father.RCount -= 1;
			beforeRead = true;
		}
		father.cv.wait(lk, [&father](){return father.WCount == 0 && father.RCount == 0;});
		father.WCount += 1;
	}
	hasUnique += 1;
}

void ThreadRWMutex::releaseShareLock(RWMutex &father){
	F = &father;
	hasShare -= 1;
	if(hasUnique == 0 && hasShare == 0 && beforeRead){
		std::unique_lock<std::mutex> lk(father.UpdateLock);
		father.RCount -= 1;
        beforeRead = false;
		father.cv.notify_all();
	}
}

void ThreadRWMutex::releaseUniqueLock(RWMutex &father){
	F = &father;
	hasUnique --;
	if(hasUnique == 0)
	{
		std::unique_lock<std::mutex> lk(father.UpdateLock);
		if(beforeRead){
			father.RCount += 1;
			beforeRead = false;
		}
		father.WCount -= 1;
		father.cv.notify_all();
	}
}

void ThreadRWMutex::releaseAll(){
    if(F != nullptr){
        if(hasUnique != 0){
            std::unique_lock<std::mutex> lk(F->UpdateLock);
            if(beforeRead){
                F->RCount += 1;
                beforeRead = false;
            }
            F->WCount -= 1;
        }
        if(hasShare != 0){
            std::unique_lock<std::mutex> lk(F->UpdateLock);
            F->RCount -= 1;
        }
        F->cv.notify_all();
    }    
    hasUnique = 0;
    hasShare = 0;
    beforeRead = false;
    F = nullptr;
}