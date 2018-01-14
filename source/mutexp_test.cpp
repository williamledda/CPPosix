/*
 * mutexp_test.cpp
 *
 *  Created on: 11 gen 2018
 *      Author: william
 */

#include "mutexp.h"

#include <iostream>

#include <errno.h>
#include <unistd.h>

using namespace std;

int main(int argc, char** argv) {
	using cpposix::MutexP;

	MutexP *mtx = new MutexP();

	cout << "Try to lock a mutex." << endl;
	if(mtx->lock() == 0) {
		cout << "Mutex locked" << endl;
	}
	else {
		cerr << "Mutex lock error" << endl;
	}

	sleep(1);

	cout << "Try to lock the same mutex." << endl;
	if(mtx->tryLock() == EBUSY) {
		cerr << "Mutex is busy" << endl;
	}

	sleep(1);
	cout << "Unlocking mutex" << endl;
	if(mtx->unlock() == 0) {
		cout << "Mutex unlocked!" << endl;
	}
	else {
		cerr << "Error unlocking mutex" << endl;
	}

	delete mtx;
}
