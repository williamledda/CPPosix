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
		return p->this_p->handler();
		//return (void*) NULL;
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

	id = -1;
//	running = false;

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
	/*if(param.other != NULL) {
		delete[] param.other;
	}*/
	param.other = NULL;
#ifdef TH_DEBUG
	clog << "Destroying a ThreadP" << endl;
#endif
}

//bool ThreadP::isRunning() const {
//	return running;
//}

int ThreadP::create(void) {
	/*int res = 0;

	res = pthread_create(&this->th, &this->th_attr, th_handler, (void*) (this));

	return res;*/

	return this->create(NULL);
}

int ThreadP::create(void* args) {
	int res = 0;

	/*if(args != NULL && size != 0) {
		//allocate new space for the additional argument
		if(this->param.other != 0) {
			delete[] this->param.other;
		}

		this->param.other = new char[size];
		memcpy(this->param.other, reinterpret_cast<char*>(args), size);
	}*/

	this->param.other = args;
	res = pthread_create(&this->id, &this->th_attr, th_handler,
			static_cast<void*>(&this->param));

	return res;
}

int ThreadP::join(void) {
	return join(NULL);
}


int ThreadP::join(void** retval) {
	return pthread_join(this->id, retval);
}

//void ThreadP::setRunning(bool running) {
//	this->running = running;
//}

void ThreadP::exit(void* retval) {
	pthread_exit(retval);
}

pthread_t ThreadP::getId() const {
	return id;
}

bool ThreadP::operator ==(const ThreadP& other) const {
	return (pthread_equal(this->id, other.id) != 0);
}

int ThreadP::cancel(void) {
	return pthread_cancel(this->id);
}

int ThreadP::setCancelState(int state, int* old) {
	int res = 0;
	int tmp_old;

	/*Since there is no standard definition for oldstate == NULL, store the
	 * value in a local variable*/
	res = pthread_setcancelstate(state, &tmp_old);

	if(old != NULL) {
		*old = tmp_old;
	}

	return res;
}

int ThreadP::setCancelType(int type, int* old) {
	int res = 0;
	int tmp_old;

	/*Since there is no standard definition for oldType == NULL, store the
	 * value in a local variable*/
	res = pthread_setcanceltype(type, &tmp_old);

	if(old != NULL) {
		*old = tmp_old;
	}

	return res;
}

bool ThreadP::operator ==(const pthread_t& thid) const {
	return (pthread_equal(this->id, thid) != 0);
}

int ThreadP::detach(void) {
	return pthread_detach(this->id);
}

} /* namespace cpposix */
