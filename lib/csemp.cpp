/*
 * semp.cpp
 *
 *  Created on: 10 feb 2018
 *      Author: william
 */

#include "csemp.h"

#include <algorithm>

namespace cpposix {

CSemP::CSemP(int value) {
	this->create(value);

	this->p_sem = &this->sem;
	this->open_mode = SEM_OPEN;
	this->access_mode = 0;
}

CSemP::CSemP(const char* name, SemOpenMode open_flag, int mode, int value) {
	if(name != NULL) {
		this->p_sem = &this->sem;
		this->open_mode = open_flag;
		this->access_mode = mode;

		this->name = name;

		//Remove all spaces from the given name
		this->name.erase(remove(this->name.begin(), this->name.end(), ' ' ),
				this->name.end() );

		if(isNamed() && this->name[0] != '/') {
			//Semaphore name must start with '/'
			this->name.insert(0, 1, '/');
		}

		this->create(value);
	}
}

//CSemP::CSemP(const char* name) {
//
//}

CSemP::~CSemP() {
	close();

	if(isNamed()) {
		unlink();
	}
}

int CSemP::getAccessMode() const {
	return access_mode;
}

//void CSemP::setAccessMode(SemAccessMode accessMode) {
//	access_mode = accessMode;
//}
//

SemOpenMode CSemP::getOpenMode() const {
	return open_mode;
}

SemResult CSemP::create(unsigned int value) {
	SemResult res = SEM_OK;

	if(!isNamed()) {
		if(sem_init(&this->sem, 0, value) == -1) {
			res = SEM_ERROR;
		}
	}
	else {
		int oflag;

		switch(this->open_mode) {
		case 0:
			oflag = 0;
			break;
		case SEM_CREATE:
			oflag = O_CREAT;
			break;
		case SEM_EXCLUSIVE:
			oflag = O_CREAT | O_EXCL;
			break;
		default:
			res = SEM_UNKN_OPEN;
		}

//		if (res == SEM_OK) {
//			switch(this->access_mode) {
//			case SEM_RD:
//				mode = O_RDONLY;
//				break;
//			case SEM_WR:
//				mode = O_WRONLY;
//				break;
//			case SEM_RDWR:
//				mode = O_RDWR;
//				break;
//			default:
//				res = SEM_UNKN_ACCESS;
//			}
//		}

		if(res == SEM_OK) {
			this->p_sem = sem_open(this->name.c_str(), oflag, access_mode, value);
		}
	}

	return res;
}

int CSemP::close(void) {
	if(isNamed()) {
		return sem_close(this->p_sem);
	}
	else {
		return sem_destroy(this->p_sem);
	}
}

int CSemP::wait(void) {
	return sem_wait(this->p_sem);
}

int CSemP::tryWait(void) {
	return sem_trywait(this->p_sem);
}

int CSemP::post(void) {
	return sem_post(this->p_sem);
}

//void CSemP::setOpenMode(SemOpenMode openMode) {
//	open_mode = openMode;
//}

bool CSemP::isNamed(void) const {
	return (this->name.length() > 0);
}

int CSemP::unlink(void) {
	return sem_unlink(this->name.c_str());
}

} /* namespace cpposix */
