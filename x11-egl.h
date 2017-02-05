/*
 * x11-egl.h
 *
 *  Created on: Jan 22, 2017
 *      Author: fery
 */

#ifndef FERY_LINUX_GTK_GLES_DEMO_X11_EGL_H_
#define FERY_LINUX_GTK_GLES_DEMO_X11_EGL_H_

#include "egl-context.h"

class X11EglData : public EGLData {
    int initGl(const char *dpyName);
public:
    // TODO this is public for event handling;
    // should be incorparated into the responsibilities of this class
    Display *x_dpy;
    Window win;
    static X11EglData createEglContextForX11();
    void close();
    void swapBuffers();
};


#endif /* FERY_LINUX_GTK_GLES_DEMO_X11_EGL_H_ */
