#include <stdio.h>

#include "Common.h"

#include "CThread.h"

#include "timeUtils.h"

class MyThread : public CThread {
	public:
		MyThread (std::string name, int duration) : CThread(name) {
			m_duration = duration;
		}

		void main ();

	private:
		int				m_duration;
};

void MyThread::main () {
	TRACE(0, "%s(%s, duration=%d)\n", __CLASSFUNCTION__, m_name.c_str(), m_duration);

	static int numMs = 10; //250;
	struct timespec packetArrivalRate;
	packetArrivalRate.tv_sec = 0;
	packetArrivalRate.tv_nsec = numMs * NANOSEC_PER_MILLISEC; // 250 ms

	struct timespec t0;
	clock_gettime(CLOCK_REALTIME, &t0); // 10 Oct 2016 - JMJ     Init t0

	for (int i=0; i<((m_duration * 1000) / numMs); i++) {
		// sleep for N ms
		t0 = timespecAdd(t0, packetArrivalRate);

		waitUntil(t0);

		TRACE(0, "%s - This message should print out about every %d ms\n", m_name.c_str(), numMs);
	}


	TRACE(0, "%s(%s) out\n", __CLASSFUNCTION__, m_name.c_str());
}


class HogThread : public CThread {
	public:
		HogThread (std::string name, int duration) : CThread(name) {
			m_duration = duration;
		}

		void main ();

	private:
		int				m_duration;
};

void HogThread::main () {
	int z;
    for (int i=0; i<1000000; i++) {
		for (int j=0; j<1000000; j++) {
			for (int k=0; k<1000000; k++) {
				z = i + j + k;
			}
		}
		TRACE(1, "%s() i=%d\n",
			__CLASSFUNCTION__, i);
	}
}


void applicationSpecific () {
	MyThread* thread1 = new MyThread("hello", 30);
	MyThread* thread2 = new MyThread("world", 50);

	thread1->start();
	thread2->start();

	HogThread* hog = new HogThread("hog", 100);
	hog->start();

	for (int i=0; i<20; i++) {
		TRACE(0, "This is \"main\" speaking: i=%d\n", i);
		sleep(1);
	}

	thread1->join();
	thread2->join();
	hog->join();
}
