/*
 * threadp.h
 *
 *  Created on: 01 gen 2018
 *      Author: william
 */

#ifndef THREADP_H_
#define THREADP_H_

#include <pthread.h>

#include <string>

namespace cpposix {

class ThreadP;   //Forward declaration of

/**
 * @brief Internal structure to define the thread parameters
 */
struct ThreadParam
{
	ThreadP *this_p; //!< Pointer to this ThreadP class
	char* other;     //!< Raw byte array containing any optional argument passed required by the thread.
};

class ThreadP {
public:
	ThreadP(const char* name = "");
	virtual ~ThreadP(void);

	/**
	 * @brief create a new thread with the given arguments
	 * @return @see pthread_create
	 */
	int Create(void);

	/**
	 * @brief Create a new thread passing additional argument and size
	 * @param args argument to provide to the thread
	 * @param size size of the argument provided
	 * @return @see pthread_create
	 */
	int Create(void* args, size_t size);

	/**
	 * @brief Join on this thread
	 * @return @see pthread_join
	 */
	int Join(void);

	/**
	 * @brief Pure virtual method that each thread class must implement
	 */
	virtual void Handler(void) = 0;

	bool isRunning() const;
	void setRunning(bool running);

protected:
	bool running;
	std::string name;

	pthread_t th;
	pthread_attr_t th_attr;

	ThreadParam param;
};

} /* namespace cpposix */

#endif /* THREADP_H_ */
