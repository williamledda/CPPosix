/*
 * semp_test.cpp
 *
 *  Created on: 10 feb 2018
 *      Author: william
 */

#include <csemp.h>
#include <threadp.h>
using namespace cpposix;

#include <iostream>
using namespace std;

#include <unistd.h>

struct Params {
	CSemP sem;
	unsigned int loops;
	unsigned int counter;
};

class MyThread: public ThreadP {
public:
	MyThread(const char* name) : ThreadP(name) {}

	virtual void* handler(void) {
		Params *p = reinterpret_cast<Params*>(this->param.other);

		if(p != NULL) {
			int loc = 0;
			for(unsigned int i = 0; i < p->loops; i++) {
				if (p->sem.wait() == 0) {
					loc = p->counter;
					loc++;
					p->counter = loc;
					p->sem.post();
				}
				else {
					cerr << this->name << " error waiting on" << endl;
				}
			}
		}
		else {
			cerr << this->name << " was called without params!" << endl;
		}

		return (void*) NULL;
	}
};

int main(int argc, char** argv) {
	Params u_param;

	ThreadP *th1 = new MyThread("Unnamed Thread 1");
	ThreadP *th2 = new MyThread("Unnamed Thread 2");

	u_param.loops = 10000000;
	u_param.counter = 0;

	cout << "Starting Th1 and Th2 to increment global variable "
			"sharing the same unnamed semaphore" << endl;

	//th1 and th2 use the unnamed semaphore
	th1->create(static_cast<void*>(&u_param));
	th2->create(static_cast<void*>(&u_param));

	th1->join(); th2->join();

	cout << "Finished -> counter = " << u_param.counter << endl;

	delete th1; delete th2;

}
