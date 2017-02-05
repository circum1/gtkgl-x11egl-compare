/*
 * main.cpp
 *
 *  Created on: Jan 11, 2017
 *      Author: fery
 *
 *  It is based on es2tri.c from here:
 *  https://fossies.org/linux/mesa-demos/src/egl/opengles2/es2tri.c
 */

/**
 * Creates GLES context
 */

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <EGL/egl.h>
#include <algorithm>

#include "util.h"
#include "gles-renderer.h"
#include "x11-egl.h"

using namespace GLES_RENDERER;

#define FLOAT_TO_FIXED(X)   ((X) * 65535.0)


/* new window size or exposure */
static void reshape(int width, int height) {
    glViewport(0, 0, (GLint) width, (GLint) height);
}

static void handleXEvents(Display *dpy, Window win, EGLDisplay egl_dpy,
        EGLSurface egl_surf) {
    while (XPending(dpy)) {
        XEvent event;
        XNextEvent(dpy, &event);

    }
}

static void event_loop(EGLData& egl, Display *dpy, Window win,
        EGLDisplay egl_dpy, EGLSurface egl_surf) {
    while (1) {
        int redraw = 1;

        handleXEvents(dpy, win, egl_dpy, egl_surf);

        if (redraw) {
            GLES_RENDERER::renderFrame();
            egl.swapBuffers();
        }
    }
}

int main(int argc, char *argv[]) {
    const int winWidth = 480, winHeight = 234;

    X11EglData egl = X11EglData::createEglContextForX11();

    GLES_RENDERER::initGlRenderer();

    /* Set initial projection/viewing transformation.
     * We can't be sure we'll get a ConfigureNotify event when the window
     * first appears.
     */
    reshape(winWidth, winHeight);

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    egl.initBeforeFirstDraw();

    event_loop(egl, egl.x_dpy, egl.win, egl.display, egl.surface);
//    event_loop_drm(egl);

    egl.close();

    return 0;
}

