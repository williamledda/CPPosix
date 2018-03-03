/*
 * mutexp_test.cpp
 *
 *  Created on: 11 gen 2018
 *      Author: william
 */

#include "mutexp.h"
#include "threadp.h"

#include <iostream>

#include <errno.h>
#include <unistd.h>

using namespace std;
using cpposix::MutexP;
using cpposix::ThreadP;

struct ThreadParams {
	MutexP mtx;
	unsigned int loops;
	unsigned int counter;
};

class MyThread: public ThreadP {
public:
	MyThread(const char* name) : ThreadP(name) {}

	virtual void* handler(void) {
		//MutexP *mtx = reinterpret_cast<MutexP*>(this->param.other);
		ThreadParams *p = reinterpret_cast<ThreadParams*>(this->param.other);

		if(p != NULL) {
			//cout << this->name << " mtx is -> " << hex << mtx << endl;
			int loc = 0;
			for(unsigned int i = 0; i < p->loops; i++) {
				//cout << this->name << " is locking Muetx." << endl;
				if (p->mtx.lock() == 0) {
					loc = p->counter;
					loc++;
					p->counter = loc;
					p->mtx.unlock();
				}
				else {
					cerr << this->name << " error locking mutex" << endl;
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

	//MutexP *mtx = new MutexP();
	//int glob = 0;

	ThreadParams p;

	p.loops = 10000000;
	p.counter = 0;



	ThreadP *th1 = new MyThread("Thread 1");
	ThreadP *th2 = new MyThread("Thread 2");
	ThreadP *th3 = new MyThread("Thread 3");

	cout << "Starting Th1 and Th2 to increment global variable that "
			"sharing the same Mutex" << endl;

	//Th1 and Th2 share the same Mutex!
	th1->create(static_cast<void*>(&p));
	th2->create(static_cast<void*>(&p));
	th3->create();

	th1->join(); th2->join(); th3->join();

	cout << "Finished -> counter = " << p.counter << endl;

	delete th1; delete th2; delete th3;//delete mtx;
}

#ifdef TESTED
#include <pthread.h>
#include "mutexp.h"
#include <iostream>

using namespace std;
using cpposix::MutexP;

static int glob = 0;


//static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

//MutexP mtx;

static void * /* Loop 'arg' times incrementing 'glob' */
threadFunc(void *arg)
{
	//int loops = *((int *) arg);
	MutexP *mtx = ((MutexP*) arg);
	//MutexP *mtx = new MutexP();
	int loops = 10000000;
	int loc, j, s;
	for (j = 0; j < loops; j++) {
		//s = pthread_mutex_lock(&mtx);

		s = mtx->lock();
		if (s != 0) cout << "pthread_mutex_lock" << endl;
		loc = glob;
		loc++;
		glob = loc;

		//s = pthread_mutex_unlock(&mtx);
		s = mtx->unlock();
		if (s != 0) cerr << "pthread_mutex_unlock" << endl;
	}
	return NULL;
}
int
main(int argc, char *argv[])
{
	pthread_t t1, t2;
	int s;
	MutexP *mtx = new MutexP();

	s = pthread_create(&t1, NULL, threadFunc, /*&loops*/(void*)mtx);
	if (s != 0)
		cerr << "pthread_create" << endl;
	s = pthread_create(&t2, NULL, threadFunc, /*&loops*/(void*)mtx);
	if (s != 0)
		cerr << "pthread_create" << endl;
	s = pthread_join(t1, NULL);
	if (s != 0)
		cerr << "pthread_join" << endl;
	s = pthread_join(t2, NULL);
	if (s != 0)
		cerr << "pthread_join" << endl;
	cout << "glob = " << glob << endl;
	exit(EXIT_SUCCESS);
}
#endif
