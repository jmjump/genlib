#pragma once

#include <time.h>
#include <sys/time.h>

#define NANOSEC_PER_SEC			(1000000000)							// 1,000,000,000 ns per sec
#define MICROSEC_PER_SEC		(1000000)								// 1,000,000 usec per sec
#define MILLISEC_PER_SEC		(1000)									// 1,000 ms per sec

#define MICROSEC_PER_MILLISEC	(MICROSEC_PER_SEC / MILLISEC_PER_SEC)	// 1,000 usec per ms
#define NANOSEC_PER_MILLISEC	(NANOSEC_PER_SEC / MILLISEC_PER_SEC)	// 1,000,000 ns per ms

extern struct timespec timespecInit ();
extern float timespecSubtract (struct timespec x, struct timespec y);
extern struct timespec timespecAdd (struct timespec x, struct timespec y);

extern void waitUntil (struct timespec stopTime);

/////////////////////////////////////////////////////////////////////////////////////////////////

extern void timeval_init (struct timeval& timeval);
extern struct timeval timeval_get (struct timeval& timeval);
extern struct timeval timeval_diff (struct timeval t1, struct timeval t0);
extern struct timeval timeval_add (struct timeval t1, struct timeval t0);
extern struct timeval timeval_max (struct timeval t1, struct timeval t0);
extern struct timeval timeval_min (struct timeval t1, struct timeval t0);

