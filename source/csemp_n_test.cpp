/*
 * semp_test.cpp
 *
 *  Created on: 10 feb 2018
 *      Author: william
 */

#include <csemp.h>
#include <threadp.h>
using namespace cpposix;

#include <string>
#include <iostream>
using namespace std;

#include <unistd.h>

struct Params {
	string name;
	unsigned int loops;
	unsigned int counter;
};

class MyThread: public ThreadP {
public:
	MyThread(const char* name) : ThreadP(name) {}

	virtual void* handler(void) {
		CSemP* sem = NULL;
		Params *p = reinterpret_cast<Params*>(this->param.other);

		if(p != NULL) {
			sem = new CSemP(p->name.c_str());

			int loc = 0;
			for(unsigned int i = 0; i < p->loops; i++) {
				if (sem->wait() == 0) {
					loc = p->counter;
					loc++;
					p->counter = loc;
					sem->post();
				}
				else {
					cerr << this->name << " error waiting on" << endl;
				}
			}
		}
		else {
			cerr << this->name << " was called without parameters!" << endl;
		}

//		cerr << this->name << " has finished. Wait for a little" << endl;
//		sleep(2);

		if(sem != NULL) {
			delete sem;
		}

		return (void*) NULL;
	}
};

int main(int argc, char** argv) {
	CSemP* sem;
	Params n_param;
	Params n_param2;

	ThreadP *th1 = new MyThread("Thread named 1");
	ThreadP *th2 = new MyThread("Thread named 2");

	//Create a new named semaphore
	sem = new CSemP("named", SEM_CREATE);

	n_param.name = "named";
	n_param.loops = 10000000;
	n_param.counter = 0;

	cout << "Starting Th1 and Th2 to increment global variable "
			"sharing the same named semaphore" << endl;

	//th1 and th2 use the named semaphore
	th1->create(static_cast<void*>(&n_param));
	th2->create(static_cast<void*>(&n_param));

	th1->join(); th2->join();

	cout << "Finished -> counter = " << n_param.counter << endl;

	//sleep(1);

	delete th1; delete th2; delete sem;
}
