/*
 * mutexp.h
 *
 *  Created on: 11 gen 2018
 *      Author: william
 */

#ifndef MUTEXP_H_
#define MUTEXP_H_

#include <pthread.h>

namespace cpposix {

/**
 * @brief Class implementing a standard POSIX mutex
 */
class MutexP {
public:
	MutexP(void);
	virtual ~MutexP(void);

	int lock(void);
	int unlock(void);
	int tryLock(void);

private:
	pthread_mutex_t mtx;
	pthread_mutexattr_t mtx_attr;
};

} /* namespace cpposix */

#endif /* MUTEXP_H_ */
