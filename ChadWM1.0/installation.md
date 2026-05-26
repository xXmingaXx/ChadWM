# compiling

First we need to compile the main.cpp file. For this we have the Makefile.
The makefile has 4 options: slow, normal, fast (Recomendet) and ffast. This refers to the
speed of the WM. 

You need the gcc compiler and the X11 dev librarys to compile the main.cpp file.

So you can compile main.cpp via: "make fast" which will create a "main" file.

# Paths

Now we need to put the main file into a certain directory.
Use the folLowing commands:
"mkdir /.void-env/"
"mkdir /.void-env/source/"
"sudo mv main /.void-env/source/"

Now move the .xinitrc file into the home directory:
"mv .xinitrc ~"

# Start! :3

Now you can start ChadWM via "startx" or better "dbus-run-session startx".

# Optional

Write the following into the .xinitrc file to start audio:
"pipewire &"
"wireplumber &"
"pipewire-pulse &"

# Support

If there are any problems or questions feel free to write to "min9a.official@gmail.com".
