#pragma once
#include <iostream>
#include <X11/Xlib.h>
#include <unistd.h>
#include <X11/keysym.h>
#include <vector>

using namespace std;

void focus_window(Display* dpy, Window win){
    XSetInputFocus(dpy, win, RevertToParent, CurrentTime);
    XRaiseWindow(dpy, win);
    XFlush(dpy);
}

void fullscreen_window(Display* dpy, Window &win, int screen){
    XMoveResizeWindow(dpy, win, 0, 0, DisplayWidth(dpy, screen), DisplayHeight(dpy, screen));
    XRaiseWindow(dpy, win);
    XFlush(dpy);
}

void destroywin(Display* dpy, Window &win, Window root){
    XUnmapWindow(dpy, win);
    XReparentWindow(dpy, win, root, 0, 0);
    XDestroyWindow(dpy, win);
}

void UmapWS(Display* dpy, vector<Window> WS){
    for (Window win : WS){
        XUnmapWindow(dpy, win);
    }
}

void mapWS(Display* dpy, vector<Window> WS){
    for (Window win : WS){
        XMapWindow(dpy, win);
    }
}


struct vec2{
    int x, y;
};