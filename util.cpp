/*
 * util.cpp
 *
 *  Created on: Jan 16, 2017
 *      Author: fery
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"

void die(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    exit(1);
}

char *loadFile(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) {
        die("Could not open %s", filename);
    }
    fseek(f, 0, SEEK_END); // seek to end of file
    long int size = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file

    char *res = (char*)malloc(size+1);
    fread(res, 1, size, f);
    res[size]=0;
    return res;
}

Timer::Timer()
{
    startCount.tv_sec = startCount.tv_usec = 0;
    endCount.tv_sec = endCount.tv_usec = 0;

    stopped = 0;
    startTimeInMicroSec = 0;
    endTimeInMicroSec = 0;
}

Timer::~Timer() { }

void Timer::start()
{
    stopped = 0; // reset stop flag
    gettimeofday(&startCount, NULL);
}

void Timer::stop()
{
    stopped = 1; // set timer stopped flag
    gettimeofday(&endCount, NULL);
}

double Timer::getElapsedTimeInMicroSec()
{
    if(!stopped)
        gettimeofday(&endCount, NULL);
    startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
    endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
    return endTimeInMicroSec - startTimeInMicroSec;
}

double Timer::getElapsedTimeInMilliSec()
{
    return this->getElapsedTimeInMicroSec() * 0.001;
}

double Timer::getElapsedTimeInSec()
{
    return this->getElapsedTimeInMicroSec() * 0.000001;
}

double Timer::getElapsedTime()
{
    return this->getElapsedTimeInSec();
}
