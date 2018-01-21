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
	//char* other;     //!< Raw byte array containing any optional argument passed required by the thread.
	void* other;
};

class ThreadP {
public:
	ThreadP(const char* name = "");
	virtual ~ThreadP(void);

	/**
	 * @brief create a new thread with the given arguments
	 * @return @see pthread_create
	 */
	int create(void);

	/**
	 * @brief Pure virtual method that each thread class must implement
	 */
	virtual void* handler(void) = 0;

	/**
	 * @brief Create a new thread passing additional argument and size
	 * @param args argument to provide to the thread
	 * @return @see pthread_create
	 */
	int create(void* args);

	/**
	 * @brief Waits for the thread identified by thread to terminate.
	 *
	 * @note If the thread has already terminated, returns immediately
	 * @return Returns 0 on success, or a positive error number on error
	 * @see pthread_join
	 *
	 */
	int join(void);

	/**
	 * @brief Waits for the thread identified by thread to terminate.
	 *
	 * If retval is not NULL it receives a copy of the terminated thread’s
	 * return value—that is, the value that was specified when the thread performed a
	 * return or called pthread_exit()
	 *
	 * @param retval Value returned by this thread upon termination.
	 * @return Returns 0 on success, or a positive error number on error
	 *
	 * @see pthread_join
	 */
	int join(void** retval);

	/**
	 * @brief terminates the calling thread, and specifies a return
	 * value that can be obtained in another thread by calling pthread_join().
	 * @param retval
	 */
	void exit(void *retval);

	/**
	 * @brief Returns the thread ID of this thread
	 * @return thread ID
	 */
	pthread_t getId() const;

	/**
	 * @brief Detaches the thread
	 *
	 * By default, a thread is joinable, meaning that when it terminates,
	 * another thread  can obtain its return status using pthread_join().
	 * Sometimes, we don’t care about the thread’s return status; we simply
	 * want the system to automatically clean up and remove the thread when
	 * it terminates.
	 *
	 * @warning Once a thread has been detached, it is no longer possible to
	 * use pthread_join() to obtain its return status and the thread can’t be
	 * made joinable again.
	 *
	 * @return Returns 0 on success, or a positive error number on error
	 *
	 * @see pthread_detach()
	 */
	int detach(void);

	/**
	 * @brief Sends a cancellation request to this thread.
	 *
	 * This doesn’t wait for the target thread to terminate. When this happens
	 * and what happens upon cancellation depends on that thread’s cancellation state
	 * and type.
	 *
	 * @return Returns 0 on success, or a positive error number on error
	 *
	 * @see pthread_cancel
	 * @see setCancelState
	 * @see setCancelType
	 */
	int cancel(void);

	/**
	 * @brief Sets the cancelability state to the value given in state.
	 *
	 * This argument has one of the following values:
	 *
	 * <ul>
	 * 	<li>PTHREAD_CANCEL_DISABLE: The thread is not cancelable. If a
	 * 	cancellation request is received, it remains pending until cancelability
	 * 	is enabled.</li>
	 * 	<li>PTHREAD_CANCEL_ENABLE: The thread is cancelable. This is the default
	 * 	cancelability state in newly created
	 * 	threads.</li>
	 * </ul>
	 *
	 * @note If a thread is cancelable then the
	 * treatment of a cancellation request is determined by the thread’s
	 * cancelability type,
	 *
	 * @param state    one of the possible cancelabity states
	 * @param oldstate previous cancelability state. It can be NULL
	 *
	 * @return 0 on success, or a positive error number on error
	 *
	 * @see setCancelType
	 */
	int setCancelState(int state, int *old = NULL);

	/**
	 *
	 * @brief Set the thread’s cancelability type,
	 *
	 *
	 * @param type This argument has one of the following values:
	 *
	 * <ul>
	 * 		<li>PTHREAD_CANCEL_ASYNCHRONOUS: The thread may be canceled at any
	 * 		time (perhaps, but not necessarily, immediately). </li>
	 * 		<li>PTHREAD_CANCEL_DEFERRED: The cancellation remains pending until
	 * 		a thread cancellation point is reached. (This is the default value)
	 * @param oldtype previous cancelability type. It can be NULL
	 *
	 * @return0 on success, or a positive error number on error
	 */
	int setCancelType(int type, int *old = NULL);


	bool operator==(const ThreadP& other) const;
	bool operator==(const pthread_t& thid) const;

//	bool isRunning() const;
//	void setRunning(bool running);

protected:
	//bool running;
	std::string name;

	pthread_t id;
	pthread_attr_t th_attr;

	ThreadParam param;
};

} /* namespace cpposix */

#endif /* THREADP_H_ */
