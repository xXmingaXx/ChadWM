# Info
Version: 1.0.0-dev
Release date: 23.05.2026
Author: Minga

ChadWM is a super minimalistic, Open-Source WM based on X11. ChadWM is intendet for minimalistic systems (originally Void-linux).
Feel free to improve the source-code yourself and share your work.

There are 8 Workspaces each with 4 Windows max.

# Installation

Read "Installation.md" for the step by step instructions.

# Keybinds
You can change the mod-key in the source code but the default is Alt.

With "mod + T" you can open the kitty terminal. (If ChadWM crashes, you need to install kitty via "xbps-install kitty" or "apt install kitty" or replace the terminal in the source code)
With "mod + esc" you can close the WM.
With "mod + Button" (button meaning any mouse-button) you can change the focus to another window.
With "mod + f" you can make the currently focused window fullscreen.
With "mod + x" you can close the currently focused window.
With "mod + -->/<--" you can change workspace.

# Config File

ChadWM automatically creates a config file. You can find it in "~/.config/ChadWM/config.conf".
There are two commands:
    init --> with init you can execute a command on start for example: "init NetworkManager"
    kbmap --> sets keyboard layout (default en)


# Stability and compability

This version is pretty stable.

# Support

If there are any problems or questions feel free to write to "min9a.official@gmail.com".

# Tips

If some Apps give an Error becouse they can't connect to the bus (steam for example), try starting ChadWM via "dbus-run-session startx".

