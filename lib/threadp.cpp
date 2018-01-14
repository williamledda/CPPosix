/*
 * threadp.cpp
 *
 *  Created on: 01 gen 2018
 *      Author: william
 */

#include "threadp.h"

#include <unistd.h>
#include <string.h>

#include <iostream>
using namespace std;

namespace cpposix {

static void* th_handler(void* arg) {
	if (arg != NULL) {
		ThreadParam *p = reinterpret_cast<ThreadParam*>(arg);
		//ThreadP *thread = reinterpret_cast<ThreadP*>(arg);
		p->this_p->Handler();
		return (void*) NULL;
	}
#ifdef TH_DEBUG
	else {
		cerr << "Improper call of th_handler without passing a "
				"pointer to thread class" << endl;
	}
#endif
	return (void*) NULL;
}

ThreadP::ThreadP(const char* name) {
	int res = 0;

	th = -1;
	running = false;

	//Create a default set of thread attributes
	res = pthread_attr_init(&this->th_attr);
	if(res != 0) {
		clog << "Error initializing thread attribute" << endl;
	}

	this->name = name;

	//Initialize the internal thread parameter
	this->param.this_p = this;
	this->param.other = NULL;

#ifdef TH_DEBUG
	clog << "Creating a new ThreadP" << endl;
#endif
}

ThreadP::~ThreadP() {
	// TODO Complete with a proper clean (including also additional arguments)
	//Free addtional arguments if they exists
	if(param.other != NULL) {
		delete[] param.other;
	}
	param.other = NULL;
#ifdef TH_DEBUG
	clog << "Destroying a ThreadP" << endl;
#endif
}

bool ThreadP::isRunning() const {
	return running;
}

int ThreadP::Create(void) {
	/*int res = 0;

	res = pthread_create(&this->th, &this->th_attr, th_handler, (void*) (this));

	return res;*/

	return this->Create(NULL, 0);
}

int ThreadP::Create(void* args, size_t size) {
	int res = 0;

	if(args != NULL && size != 0) {
		//allocate new space for the additional argument
		if(this->param.other != 0) {
			delete[] this->param.other;
		}

		this->param.other = new char[size];
		memcpy(this->param.other, reinterpret_cast<char*>(args), size);
	}

	res = pthread_create(&this->th, &this->th_attr, th_handler,
			static_cast<void*>(&this->param));

	return res;
}

int ThreadP::Join(void) {
	return pthread_join(this->th, NULL);
}

void ThreadP::setRunning(bool running) {
	this->running = running;
}

} /* namespace cpposix */
