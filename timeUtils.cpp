#include "timeUtils.h"

#include <unistd.h>

/////////////////////////////////////////////////////////////////////////////////////////////////

struct timespec timespecInit () {
	struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

	return now;
}

float timespecSubtract (struct timespec x, struct timespec y)
{
    struct timespec temp;
    temp.tv_sec = x.tv_sec - y.tv_sec;
    temp.tv_nsec = x.tv_nsec - y.tv_nsec;

	float f = temp.tv_sec + ((float) temp.tv_nsec / (float) NANOSEC_PER_SEC);

    return f;
}

struct timespec timespecAdd (struct timespec x, struct timespec y)
{
    struct timespec temp;
    temp.tv_sec = x.tv_sec + y.tv_sec;
    temp.tv_nsec = x.tv_nsec + y.tv_nsec;

    if (temp.tv_nsec > NANOSEC_PER_SEC) {
        temp.tv_sec += 1;
        temp.tv_nsec -= NANOSEC_PER_SEC;
    }

	return temp;
}

void waitUntil (struct timespec stopTime)
{
	while (1) {
		struct timespec now;
    	clock_gettime(CLOCK_REALTIME, &now);

		float deltaT = timespecSubtract(stopTime, now);

		// If we get close enough to the expiration time, just go with it! (Don't start another timer)
#define FUDGE_FACTOR_1		(0.000500)	// 500 usec = 0.5 ms
		if (deltaT <= FUDGE_FACTOR_1)
			break;

		// we always seem to run a little long, so shave a little!
#define FUDGE_FACTOR_2		1492
		int usecToSleep = (int)(deltaT * MICROSEC_PER_SEC);
		if (usecToSleep > FUDGE_FACTOR_2)
			usecToSleep -= FUDGE_FACTOR_2;
		usleep(usecToSleep);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void timeval_init (struct timeval& timeval) {
	timeval.tv_sec = 0;
	timeval.tv_usec = 0;
}

struct timeval timeval_get (struct timeval& timeval) {
    struct timezone sTZ;
    gettimeofday(&timeval, &sTZ);

	return timeval;
}

struct timeval timeval_diff (struct timeval t1, struct timeval t0) {
	struct timeval diff;

	diff.tv_sec = t1.tv_sec - t0.tv_sec;
	diff.tv_usec = t1.tv_usec - t0.tv_usec;
	if (diff.tv_usec < 0) {
		diff.tv_sec--;
		diff.tv_usec += 1000000;
	}

	return diff;
}

struct timeval timeval_add (struct timeval t1, struct timeval t0) {
	struct timeval sum;

	sum.tv_sec = t1.tv_sec + t0.tv_sec;
	sum.tv_usec = t1.tv_usec + t0.tv_usec;
	if (sum.tv_usec >= 1000000) {
		sum.tv_sec++;
		sum.tv_usec -= 1000000;
	}

	return sum;
}

struct timeval timeval_max (struct timeval t1, struct timeval t0) {
	if (t1.tv_sec > t0.tv_sec)
		return t1;
	if ((t1.tv_sec == t0.tv_sec) && (t1.tv_usec > t0.tv_usec))
		return t1;
	return t0;
}

struct timeval timeval_min (struct timeval t1, struct timeval t0) {
	if (t1.tv_sec < t0.tv_sec)
		return t1;
	if ((t1.tv_sec == t0.tv_sec) && (t1.tv_usec < t0.tv_usec))
		return t1;
	return t0;
}

