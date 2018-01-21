/*
 * thread_cancel_test.cpp
 *
 *  Created on: 20 gen 2018
 *      Author: william
 */


/**
 * A simple example of the use of pthread_cancel(). The main program creates a
 * thread that executes an infinite loop, sleeping for a second and
 * printing the value of a loop counter. (This thread will terminate only if it
 * is sent a cancellation request or if the process exits.)
 * Meanwhile, the main program sleeps
 * for 3 seconds, and then sends a cancellation request to the thread that it created.
 * When we run this program, we see the following:
 */

#include <threadp.h>

#include <iostream>

#include <unistd.h>

using cpposix::ThreadP;
using namespace std;

class MyThread: public ThreadP {
public:
	MyThread(const char* name) : ThreadP(name) {}

	virtual void* handler(void) {
		int j;

		cout << this->name << " started" << endl; /* May be a cancellation point */

		for (j = 1; ; j++) {
			cout << "Loop " << j << endl; /* May be a cancellation point */
			sleep(1); /* A cancellation point */
		}
		/* NOTREACHED */

		return (void*) NULL;
	}

};

int main(int argc, char** argv) {
	int s;
	void *res;

	MyThread *th = new MyThread("Test cancel");

	s = th->create();
	if (s != 0) {
		cerr << "pthread_create" << endl;
	}
	sleep(3); /* Allow new thread to run a while */

	s = th->cancel();
	if (s != 0) {
		cerr << "pthread_cancel" << endl;
	}

	s = th->join(&res);
	if (s != 0) {
		cerr << "pthread_join" << endl;
	}

	if (res == PTHREAD_CANCELED) {
		cout << "Thread was canceled!" << endl;
	}
	else {
		cerr << "Thread was not canceled (should not happen!)" << endl;
	}

	delete th;
	return 0;
}

