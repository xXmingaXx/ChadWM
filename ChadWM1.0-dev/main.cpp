#include <iostream>
#include <X11/Xlib.h>
#include <unistd.h>
#include <X11/keysym.h>
#include <cstdlib>
#include <vector>
#include "libs/SXlib/SXlib.h"
#include "libs/file.h"
#include <fstream>
#include <filesystem>
#include <string>

//void-env (or ChadWM) is a extremly simple and fast desctop environment
//Version: 1.0.1-dev


//Namespaces
using namespace std;
namespace fs = filesystem;

int main() {

    //struct vec2 (int x, y) for Mouse Position
    vec2 mouspos;

    //Connection to X-Server
    Display* disp = XOpenDisplay(NULL);

    //#### Keybinds ###################################################

    //Mod = Alt | for Super(Windows-key) as mod use Mod4Mask
    unsigned int mod = Mod1Mask;

    KeyCode esc = XKeysymToKeycode(disp, XK_Escape);

    KeyCode L = XKeysymToKeycode(disp, XK_Left);
    KeyCode R = XKeysymToKeycode(disp, XK_Right);

    KeyCode Full = XKeysymToKeycode(disp, XK_f);
    KeyCode X = XKeysymToKeycode(disp, XK_x);
    KeyCode T = XKeysymToKeycode(disp, XK_t);


    //Checking connection to X-Server
    if (!disp){
        cerr << "[ERROR] Faild connecting to the X-Server!\n";
        return -1;
    }
    else{
        cout << "[OK] Connected to the X-Server!\n";
    }

    //creating screen
    int screen = DefaultScreen(disp);

    //display-hight/width
    int dpyw = DisplayWidth(disp, screen);
    int dpyh = DisplayHeight(disp, screen);

    //creating root window
    Window root = DefaultRootWindow(disp);

    //setting Input for root
    XSelectInput(disp, root, SubstructureRedirectMask | SubstructureNotifyMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
    XSync(disp, False);
    XFlush(disp);

    //"Grabing" the keys for the keybinds
    XGrabKey(disp, esc, mod, root, True, GrabModeAsync, GrabModeAsync);

    XGrabKey(disp, L, mod, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(disp, R, mod, root, True, GrabModeAsync, GrabModeAsync);

    XGrabKey(disp, Full, mod, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(disp, X, mod, root, True, GrabModeAsync, GrabModeAsync);
    XGrabKey(disp, T, mod, root, True, GrabModeAsync, GrabModeAsync);

    //8 Workspaces each with 4 Windows max
    vector<vector<Window>> WSPACES  = {{}, {}, {}, {}, {}, {}, {}, {}};
    //fullscreen variable for each workspace
    vector<bool> fxr = {0, 0, 0, 0, 0, 0, 0, 0};
    //current workspace index
    int WSindex = 0;

    //dummy-variables
    Window dummyW;
    int dummyInt;

    //Grabing mouse actions for focusing on windows
    XGrabButton(disp, AnyButton, mod, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);

    //declaring some X-Event variables
    XEvent event;
    XMapRequestEvent* e;
    XKeyEvent* ek;
    XDestroyWindowEvent* de;
    Window fwin;

    XWindowAttributes attr;

    Status s;


    //commands for config-file
    string homedir = std::getenv("HOME"); 
    string command1 = "mkdir " + homedir + "/.config/ChadWM/";
    string command2 = "touch " + homedir + "/.config/ChadWM/config.conf";

    //###### Conf-file #################################################################################
    if (!fs::exists(homedir + "/.config/ChadWM/config.conf")){
        system(command1.c_str());
        system(command2.c_str());
        cout << "Creating conf-file ...\n";
        if (fs::exists(homedir + "/.config/ChadWM/config.conf")){
            cout << "[OK]Created conf-file!\n";
        }
        else{
            cout << "[ERROR]Couldn't create conf-file!\n";
            return -1;
        }
    }

    //opening config file
    ifstream conffile(homedir + "/.config/ChadWM/config.conf");

    string line;
    string command;

    /*  Going through the conf-file line by line and executing commands.
    Commands:
    init <command> -> executes the command when the WM starts
    kbmap <layout> -> sets the keyboard layout with setxkbmap    
    */
    while(getline(conffile, line)){
        if (line != ""){
            //init-command: init <command>
            if(splitstring(line)[0] == "init" && splitstring(line).size() > 1){
                system(unite(splitstring(line), 1).c_str());
            }

            //kbmap-command: kbmap <layout>
            else if(splitstring(line)[0] == "kbmap" && splitstring(line).size() > 1){
                command = "setxkbmap " + splitstring(line)[1];
                system(command.c_str());
            }

            
        }
    }


    //MAIN-LOOP

    while(true){
        //Getting events
        XNextEvent(disp, &event);

        //Checking if a window wants to be mapped
        if (event.type == MapRequest && WSPACES[WSindex].size() < 4){
            e = &event.xmaprequest;

            WSPACES[WSindex].push_back(e->window);

            XMapWindow(disp, e->window);
        }
        
        //Checking keys for keybinds
        if (event.type == KeyPress){
            ek = &event.xkey;

            //Alt + t == kitty opens
            if (ek->keycode == T && (ek->state & mod)){
                system("kitty&");
                //system("xterm&");
            }

            //Alt + f == fullscreen
            else if (ek->keycode == Full && (ek->state & mod)){
                if (!fxr[WSindex]){
                    XGetInputFocus(disp, &fwin, &dummyInt);
                    fullscreen_window(disp, fwin, screen);
                    fxr[WSindex] = true;
                } else {fxr[WSindex] = false;}

                
            }

            //Close Windows if Mod + x is pressed

            else if (ek->keycode == X && (ek->state & mod)){
                XGetInputFocus(disp, &fwin, &dummyInt);
                for (int i=0; i<WSPACES[WSindex].size();i++){
                    if (fwin == WSPACES[WSindex][i]){
                        destroywin(disp, WSPACES[WSindex][i], root);
                        WSPACES[WSindex].erase(WSPACES[WSindex].begin()+i);
                        fxr[WSindex] = false;
                        break;
                    }
                }
            }


            //Switch Workspaces


            //Mod + Left == Subtract 1 from WSindex
            else if (ek->keycode == L && (ek->state & mod) && WSindex > 0){
                WSindex--;
                UmapWS(disp, WSPACES[WSindex+1]);
                mapWS(disp, WSPACES[WSindex]);
            }

            //Mod + Right == Add 1 to WSindex
            else if (ek->keycode == R && (ek->state & mod) && WSindex < 7){
                WSindex++;
                UmapWS(disp, WSPACES[WSindex-1]);
                mapWS(disp, WSPACES[WSindex]);
            }

            //Alt + ESC == close WM
            else if (ek->keycode == esc && (ek->state & mod)){
                //breaking the main loop
                break;
            }

        }

        //check if windows are still alive
        if (event.type == DestroyNotify){
            de = &event.xdestroywindow;

            for (int i=0; i<WSPACES[WSindex].size();i++){
                if (de->window == WSPACES[WSindex][i]){
                    WSPACES[WSindex].erase(WSPACES[WSindex].begin()+i);
                    i--;
                    fxr[WSindex] == false;
                }
            }
        }

        

        //#### Manage Windows #############################################################

        /*
            This secion desides where the windows are mapped an how/where they are mapped.
            If a window gets pressed (mouse-click) while the mod key is held, it gets focused.
            Mod + f makes the focused window fullscreen and back to normal if it's already fullscreen.
        */

        //if there is no fullscreen window in the workspace, the windows (max 4) get mapped the following way:
        if (!fxr[WSindex]){
            if (WSPACES[WSindex].size() == 2){
                XMoveResizeWindow(disp, WSPACES[WSindex][0], 0, 0, dpyw/2, dpyh);
                XMoveResizeWindow(disp, WSPACES[WSindex][1], dpyw/2, 0, dpyw/2, dpyh);
            }

            if (WSPACES[WSindex].size() == 3){
                XMoveResizeWindow(disp, WSPACES[WSindex][0], 0, 0, dpyw/2, dpyh/2);
                XMoveResizeWindow(disp, WSPACES[WSindex][1], dpyw/2, 0, dpyw/2, dpyh/2);
                XMoveResizeWindow(disp, WSPACES[WSindex][2], 0, dpyh/2, dpyw, dpyh/2);
            }

            if (WSPACES[WSindex].size() == 4){
                XMoveResizeWindow(disp, WSPACES[WSindex][0], 0, 0, dpyw/2, dpyh/2);
                XMoveResizeWindow(disp, WSPACES[WSindex][1], dpyw/2, 0, dpyw/2, dpyh/2);
                XMoveResizeWindow(disp, WSPACES[WSindex][2], 0, dpyh/2, dpyw/2, dpyh/2);
                XMoveResizeWindow(disp, WSPACES[WSindex][3], dpyw/2, dpyh/2, dpyw/2, dpyh/2);
            }

            //Focusing on pressed window (if mod is pressed)

            if (event.type == ButtonPress){
                mouspos.x = event.xbutton.x_root;
                mouspos.y = event.xbutton.y_root;

                if (WSPACES[WSindex].size() == 1){
                    focus_window(disp, WSPACES[WSindex][0]);
                }

                else if (WSPACES[WSindex].size() == 2){
                    if (mouspos.x < dpyw/2){
                        focus_window(disp, WSPACES[WSindex][0]);
                    } else{
                        focus_window(disp, WSPACES[WSindex][1]);
                    }
                }

                else if (WSPACES[WSindex].size() == 3){
                    if (mouspos.y > dpyh/2){
                        focus_window(disp, WSPACES[WSindex][2]);
                    } else if(mouspos.x < dpyw/2){
                        focus_window(disp, WSPACES[WSindex][0]);
                    } else {
                        focus_window(disp, WSPACES[WSindex][1]);
                    }
                }

                else if (WSPACES[WSindex].size() == 4){
                    if (mouspos.y < dpyh/2 && mouspos.x < dpyw/2){
                        focus_window(disp, WSPACES[WSindex][0]);
                    } else if (mouspos.y < dpyh/2 && mouspos.x > dpyw/2){
                        focus_window(disp, WSPACES[WSindex][1]);
                    } else if (mouspos.y > dpyh/2 && mouspos.x < dpyw/2){
                        focus_window(disp, WSPACES[WSindex][2]);
                    } else if (mouspos.y > dpyh/2 && mouspos.x > dpyw/2){
                        focus_window(disp, WSPACES[WSindex][3]);
                    }
                }

                XAllowEvents(disp, ReplayPointer, event.xbutton.time);


            }
        }

    }

    
    //closing connection to X-Server if the main loop is broken (Alt + ESC)
    XCloseDisplay(disp);
    return 0;
}