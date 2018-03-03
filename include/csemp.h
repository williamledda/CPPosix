/*
 * semp.h
 *
 *  Created on: 10 feb 2018
 *      Author: william
 */

#ifndef CSEMP_H_
#define CSEMP_H_

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

#include <string>

namespace cpposix {

/**
 * @class CSemP
 *
 * @brief Class providing implementation of Posix semaphore (both named and unnamed)
 *
 */

enum SemOpenMode {
	SEM_OPEN = 0,   /*!< Open an existing semaphore (default) */
	SEM_CREATE,     /*!< Create a new semaphore */
	SEM_EXCLUSIVE,  /*!< Create a new semaphore in exclusive mode */
	SEM_OPEN_UNKN
};
//
//enum SemAccessMode {
//	SEM_RD = 0, /*!< Read only access */
//	SEM_WR,     /*!< Read write access */
//	SEM_RDWR,   /*!< Read and write (Default) */
//	SEM_ACCESS_UNKN
//};

enum SemResult {
	SEM_UNKN_OPEN = -2,
	SEM_UNKN_ACCESS,
	SEM_ERROR = 0,
	SEM_OK,
};

class CSemP {
public:
	/**
	 * @brief Create and init an unnamed semaphore
	 */
	CSemP(int value = 1);

	/**
	 * Open or create a named semaphore
	 * @param name
	 * @param open_flag
	 * @param mode
	 * @param value
	 *
	 * @author William Ledda (villy80@hotmail.it)
	 * @since 1.0.0
	 */
	CSemP(const char* name, SemOpenMode open_flag = SEM_OPEN, int mode = 0666,
			int value = 1);
	virtual ~CSemP(void);

	/**
	 * @brief Closes the semaphore
	 *
	 * It release any resources that the system has associated with the semaphore.
	 *
	 * In case of named semaphore it behaves like the sem_close POSIX function
	 * decreasing the count of processes referencing the semaphore.
	 *
	 * In case of unnamed semaphore it behaves like sem_detroy
	 *
	 * @return Returns 0 on success, or –1 on error
	 *
	 * @note Open named semaphores are also automatically closed on process
	 * termination or if the process performs an exec().
	 *
	 * @warning Closing a semaphore does not delete it. For that purpose use
	 * @ref unlink().
	 *
	 * @see unlink
	 *
	 * @author William Ledda (villy80@hotmail.it)
	 * @since 1.0.0
	 */
	int close(void);

	/**
	 * @brief Decrements by 1 the value of the semaphore
	 *
	 * If the semaphore currently has a value greater than 0, it returns
	 * immediately otherwise it blocks until the semaphore value rises above 0.
	 * The semaphore is then decremented and it returns.
	 *
	 * @note If a blocked call is interrupted by a signal handler, it fails with
	 * the error EINTR.
	 *
	 * @return Returns 0 on success, or –1 on error
	 *
	 * @author William Ledda (villy80@hotmail.it)
	 * @since 1.0.0
	 *
	 * @see tryWait
	 */
	int wait(void);

	/**
	 * @brief Non blocking wait
	 *
	 * If the decrement operation can’t be performed immediately, it return with the error EAGAIN.
	 *
	 * @note If a blocked call is interrupted by a signal handler, it fails with
	 * the error EINTR.
	 *
	 * @return Returns 0 on success, or –1 on error
	 *
	 * @author William Ledda (villy80@hotmail.it)
	 * @since 1.0.0
	 *
	 * @see wait
	 */
	int tryWait(void);

	//timedWait

	/**
	 * @brief Increments by 1 the value of the semaphore
	 *
	 * @return Returns 0 on success, or –1 on error
	 *
	 * @author William Ledda (villy80@hotmail.it)
	 * @since 1.0.0
	 */
	int post(void);


//	SemAccessMode getAccessMode() const;
	int getAccessMode() const;
//	void setAccessMode(SemAccessMode accessMode);
	SemOpenMode getOpenMode() const;
//	void setOpenMode(SemOpenMode openMode);

protected:
	std::string name;
	sem_t sem;
	sem_t* p_sem;

	SemOpenMode   open_mode;
//	SemAccessMode access_mode;
	int access_mode;

	/**
	 * @brief Check if the semaphore is named or unnamed
	 *
	 * @return True if the semaphore has a valid name, false otherwise
	 *
	 * @author William Ledda (villy80@hotmail.it)
	 * @since 1.0.0
	 */
	bool isNamed(void) const;

	/**
	 * @brief Removes the semaphore (Only for named semaphore)
	 *
	 * The semaphore is destroyed once all processes cease using it (immediately
	 * if all processes that had the semaphore open have already closed it).
	 *
	 * @note this method doesn't have any effect in case of unnamed semaphore
	 *
	 * @return Returns 0 on success, or –1 on error
	 *
	 * @see close
	 *
	 * @author William Ledda (villy80@hotmail.it)
	 * @since 1.0.0
	 */
	int unlink(void);

private:

	/**
	 * @brief Open this semaphore
	 *
	 * Depending on if this semaphore is named or unnamed, create and open
	 * a new named semaphore according to its open and access mode (@ref setOpenMode
	 * , @ref setAccessMode) or initialize the unnamed semaphore
	 *
	 * @param oflag bit mask that determines whether we are opening an existing (when 0)
	 * semaphore or creating and opening a new semaphore. (refer to man page of sem_open for more details)
	 * @param mode  bit mask that specifies the permissions to be placed on the new semaphore
	 * @param value Initial value of the semaphore
	 * @return Returns 1 on success or SEM_FAILED on error
	 *
	 * @see setAccessMode
	 * @see setOpenMode
	 *
	 * @author William Ledda (villy80@hotmail.it)
	 * @since 1.0.0
	 */
	SemResult create(unsigned int value = 1);
};

} /* namespace cpposix */

#endif /* CSEMP_H_ */
