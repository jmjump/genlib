#include <stdio.h>

#include "CThread.h"

#include "Common.h"

//////////////////////////////////////////////////////////////////////////////////

CThread::CThread (std::string name) : m_name(name) {
	// 20 Oct 2014 - JMJ
	// Can't start the thread in the ctor because "main" is pure virtual.
	// If we were to start here, then it might get called right away and that would be bad
	// because "main" won't get filled in until the ctor of the derived class has a chance
	// to run.
	m_started = false;
	m_canceled = false;
	m_joined = false;

	static int g_id = 1;
	m_id = g_id++;

	TRACE(1, "%s(name=%s) id=%d \n", __CLASSFUNCTION__, m_name.c_str(), m_id);
}

CThread::~CThread () {
	TRACE(1, "CThread::%s() name=%s, id=%d \n", __CLASSFUNCTION__, m_name.c_str(), m_id);

	if (m_started) {
		cancel();
	}
}

void CThread::start () {
	if (!m_started) {
		m_started = true;
    	pthread_create(&m_threadId, NULL, entryPoint, this);
		TRACE(1, "%s() %s:%d (0x%08x)\n", __CLASSFUNCTION__, m_name.c_str(), m_id, m_threadId);
	}
}

void* CThread::entryPoint (void* ctx) {
	CThread* self = (CThread*) ctx;

	TRACE(1, "CThread::%s() thread %s:%d starting!\n", __func__, self->m_name.c_str(), self->m_id);

	self->main();

	TRACE(1, "CThread::%s() thread %s:%d exiting!\n", __func__, self->m_name.c_str(), self->m_id);

// TBD: what is the right thing to do?
	//self->m_canceled = true; // fake our own death! (so the dtor doesn't call cancel)

	return NULL;
}

void CThread::cancel () {
	if (!m_canceled) {
		int status = pthread_cancel(m_threadId);
		if (status != 0) {
			TRACE(1, "%s() WARNING pthread_cancel(%d)=%d\n", __CLASSFUNCTION__, m_threadId, status);
		}

		join();

		m_canceled = true;
	}
}

void CThread::join () {
	if (!m_joined) {
		TRACE(1, "%s() %s:%d before call to pthread_join(0x%08x)\n",
			__CLASSFUNCTION__, m_name.c_str(), m_id, m_threadId);

		void* res;
		int status = pthread_join(m_threadId, &res);

		TRACE(1, "%s() %s:%d after call to pthread_join(0x%08x)=%d\n",
			__CLASSFUNCTION__, m_name.c_str(), m_id, m_threadId, status);

		if (status != 0) {
			TRACE(1, "%s() WARNING pthread_join(0x%08x)=%d\n", __CLASSFUNCTION__, m_threadId, status);
		}

		if (res == PTHREAD_CANCELED) {
			TRACE(1, "%s() pthread 0x%08x was canceled\n", __CLASSFUNCTION__, m_threadId);
		}

		m_joined = true;
		m_canceled = true; // no need to cancel!
	}
}

