#include <unistd.h>
#include <iostream>
#include <time.h>

#include "threadp.h"

using namespace cpposix;

class MyThread: public ThreadP {
public:
	MyThread(const char* name) : ThreadP(name) {}

	virtual void* handler(void) {
		timespec now;
		timespec *wait = reinterpret_cast<timespec*>(this->param.other);

		for(int i = 0; i < 10; i++) {
			clock_gettime(CLOCK_REALTIME, &now);
			std::cout << "[" << now.tv_sec << "." << now.tv_nsec << "] [" <<
					this->name << "] " <<
					i << std::endl;
			nanosleep(wait, NULL);
		}

		return (void*) NULL;
	}
};

int main(int argc, char** argv) {
//	unsigned short wait_1 = 1, wait_2 = 2;
	MyThread th1("Th1"), th2("Th2");
	timespec wait_1 = {0, 500000000};
	timespec wait_2 = {1, 0};

	th1.create(static_cast<void*>(&wait_1));
	th2.create(static_cast<void*>(&wait_2));

	th1.join(); th2.join();

	std::cout << "Ended!" << std::endl;
	return 0;
}
