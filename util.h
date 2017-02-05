/*
 * util.h
 *
 *  Created on: Jan 16, 2017
 *      Author: fery
 */

#ifndef FERY_LINUX_GTK_GLES_DEMO_UTIL_H_
#define FERY_LINUX_GTK_GLES_DEMO_UTIL_H_

#include <sys/time.h>

void die(const char *fmt, ...);
char *loadFile(const char *filename);

class Timer
{
public:
    Timer();                                    // default constructor
    ~Timer();                                   // default destructor

    void   start();                             // start timer
    void   stop();                              // stop the timer
    double getElapsedTime();                    // get elapsed time in second
    double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
    double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
    double getElapsedTimeInMicroSec();          // get elapsed time in micro-second
private:
    double startTimeInMicroSec;                 // starting time in micro-second
    double endTimeInMicroSec;                   // ending time in micro-second
    int    stopped;                             // stop flag
    timeval startCount;                         //
    timeval endCount;                           //
};

typedef long usec_time;

inline usec_time getCurrentTimeInMicroSec() {
    timeval tval;
    gettimeofday(&tval, NULL);
    return (tval.tv_sec * 1000000.0) + tval.tv_usec;
}

#endif /* FERY_LINUX_GTK_GLES_DEMO_UTIL_H_ */
