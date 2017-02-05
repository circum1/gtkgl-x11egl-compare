/*
 * egl-context.h
 *
 *  Created on: Jan 22, 2017
 *      Author: fery
 */

#ifndef FERY_LINUX_GTK_GLES_DEMO_EGL_CONTEXT_H_
#define FERY_LINUX_GTK_GLES_DEMO_EGL_CONTEXT_H_

#include <EGL/egl.h>

// copyable; hopefully all the members are pointers
class EGLData {
public:
    EGLDisplay display;
    EGLConfig config;
    EGLContext context;
    EGLSurface surface;
    void printEglInfo();
    virtual void close() {}
    virtual void swapBuffers() {}
    virtual void initBeforeFirstDraw() {};
    virtual ~EGLData() {}
};

#endif /* FERY_LINUX_GTK_GLES_DEMO_EGL_CONTEXT_H_ */
