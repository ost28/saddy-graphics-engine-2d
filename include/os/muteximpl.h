/*! \file       muteximpl.h
    \author    mamontov-cpp
     
    Contains crossplatform mutex implementation,  which uses pthread on 
    Linux and WinAPI on Windows.  A mutex can be locked or unlocked, blocking 
    execution of thread, or unblocking it. 
*/
#pragma once
#ifdef WIN32
    #ifndef NOMINMAX
    #define NOMINMAX 
    #endif
    #include  <windows.h>
#else
    #include  <pthread.h>
#endif

namespace sad
{

namespace os
{

/*! \class MutexImpl
    
    A simple non-recursive mutex, which can block execution, when other thread locks on it,
    or unblock it, when other thread is done working with it
*/
class MutexImpl
{
public:
    /*! Creates new non-locked mutex
     */
    MutexImpl();
    /*! Releases system resources, taken by mutex
     */
    ~MutexImpl();
    /*! Locks a mutex, blocking  execution of code in other threads
     */
    void lock();
    /*! Unlocks a mutex, allowing execution of code in other threads
     */
    void unlock();
protected:
#ifdef WIN32
    CRITICAL_SECTION  m_m;          //!< A system-dependent handle of mutex
#else
    pthread_mutex_t m_m;  //!< A system-dependent handle of mutex
    pthread_mutex_t m_guard; //!< A guarding mutex to ensure, that nobody will harm locked state
    pthread_t m_locked_thread;  //!< A recursive-call deadlock protection for mutex to make it really non-recursive
    bool m_locked;              //!< Whether it's locked
#endif
private:
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other mutex
     */
    MutexImpl(const sad::os::MutexImpl & o);
    /*! Cannot be copied, so this is disabled and not implemented
        \param[in] o other mutex
        \return self-reference
     */ 
    sad::os::MutexImpl & operator=(const sad::os::MutexImpl & o);
};

}

}
