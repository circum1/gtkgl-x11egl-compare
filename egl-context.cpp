/*
 * egl-context.cpp
 *
 *  Created on: Jan 22, 2017
 *      Author: fery
 */

#include <stdio.h>
#include "egl-context.h"

void EGLData::printEglInfo() {
    const char *s;
    s = eglQueryString(display, EGL_VERSION);
    printf("EGL_VERSION = %s\n", s);

    s = eglQueryString(display, EGL_VENDOR);
    printf("EGL_VENDOR = %s\n", s);

    s = eglQueryString(display, EGL_EXTENSIONS);
    printf("EGL_EXTENSIONS = %s\n", s);

    s = eglQueryString(display, EGL_CLIENT_APIS);
    printf("EGL_CLIENT_APIS = %s\n", s);
}


