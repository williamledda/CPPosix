/*
 * mutexp.cpp
 *
 *  Created on: 11 gen 2018
 *      Author: william
 */

#include "mutexp.h"

#include <iostream>

namespace cpposix {

MutexP::MutexP() {
	/*Both returns always zero. No error check is needed*/
	pthread_mutexattr_init(&this->mtx_attr);
	pthread_mutex_init(&this->mtx, &this->mtx_attr);
}

MutexP::~MutexP() {
	if(pthread_mutex_destroy(&this->mtx) != 0) {
		std::cerr << "Error destroying mutex" << std::endl;
	}

	if(pthread_mutexattr_destroy(&this->mtx_attr) != 0) {
		std::cerr << "Error destroying mutex attribute" << std::endl;
	}
}

int MutexP::lock(void) {
	return pthread_mutex_lock(&this->mtx);
}

int MutexP::unlock(void) {
	return pthread_mutex_unlock(&this->mtx);
}

int MutexP::tryLock(void) {
	return pthread_mutex_trylock(&this->mtx);
}

} /* namespace cpposix */
