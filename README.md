<center>
<img src="doc/img/logo.png" alt="table" />
</center>
<br>


# Qt Desktop Notify

A program to diplay notifications in Linux


## Usage

```
Usage: ./bin/notify [options]
Qt System Tray Notification

Options:
  -h, --help                                Displays help on commandline
                                            options.
  --help-all                                Displays help, including generic Qt
                                            options.
  -v, --version                             Displays version information.
  -t, --title <string>                      Notification title
  -m, --message <string>                    Notification message
  -p, --priority <low|normal|high>          Notification priority (low, normal,
                                            high)
  -c, --category <system|critical|network>  Notification category (system,
                                            critical, network)
  -d, --delay <ms>                          Notification delay in milliseconds
  -i, --icons                               List available icons in the
                                            embedded resources
```


## **📌 Example Usage**
### ✅ **List All Embedded Icons**
```sh
notify --icons
```

### ✅ **Send a System Notification**
```sh
notify --title "System Update" --message "A new update is available!" --category system --delay 4000
```

### ✅ **Send a Critical Notification**
```sh
notify --title "Disk Full!" --message "You are running out of disk space!" --category critical --priority high
```

### ✅ **Send a Network Notification**
```sh
notify --title "WiFi Connected" --message "You are now connected to HomeWiFi" --category network
```


<center>
<img src="doc/img/demo.png" alt="table" />
</center>
<br>



### The project structure:

```
qt-desktop-notify
├── CMakeLists.txt
├── notify.pro
├── src
│   ├── main.cpp
└── bin  (binary output)
```


### Build with CMake

```sh
cd qt-desktop-notify
mkdir build && cd build
cmake ..
make
.notify "Hello from Qt!"
```

### Build with qmake

```sh
cd qt-desktop-notify
qmake
make
notify "Hello from Qt!"
```

After compilation, the executable will be located in `bin/`. Running the program:

```sh
notify -t "Alert Notification" -m "Hello this is an alert!"
```

### Troubleshooting & Fixes

If `QSystemTrayIcon::isSystemTrayAvailable()` returns `false`, it means your desktop environment (DE) may not support system tray notifications by default or requires additional configuration.

First, try running a system tray (`trayer` or enabling AppIndicators for GNOME).

### 1. Check if Your Desktop Environment Supports System Tray

Some minimal Linux desktop environments (like i3, Sway, Openbox, Wayland-based desktops) may not have a system tray by default.

Try running:
```sh
qdbus org.kde.StatusNotifierWatcher /StatusNotifierWatcher StatusNotifierItems
```
or
```sh
dbus-send --print-reply --dest=org.freedesktop.DBus / org.freedesktop.DBus.ListNames
```
If there is no system tray service running, Qt notifications won't work.

## Enable a System Tray Service**

Since your environment does not have `org.kde.StatusNotifierWatcher`, you can try running a system tray manually.

### **Try Running a System Tray in Your DE**
1. **For GNOME Users** (Install AppIndicator Extension)
   ```sh
   sudo apt install gnome-shell-extension-appindicator
   gnome-extensions enable appindicatorsupport@rgcjonas.gmail.com
   ```
   Then **restart GNOME** (log out and log in).

2. **For XFCE / i3 / Openbox** (Use `stalonetray` or `trayer`)
   ```sh
   sudo apt install trayer
   trayer --edge top --align right --SetDockType true --expand true
   ```

3. **For KDE Users** (Ensure Plasma's System Tray is Enabled)
   ```sh
   kquitapp5 plasmashell && kstart5 plasmashell
   ```

Then, **try running your Qt notification app again**.

Fix: If you are on a minimal DE, you might need to install a system tray:
- KDE Plasma: Supported natively.
- GNOME: Requires `gnome-shell-extension-appindicator` (`sudo apt install gnome-shell-extension-appindicator`).
- i3/Sway: Install `trayer` or `stalonetray`.
  ```sh
  sudo apt install trayer
  trayer --edge top --align right --SetDockType true --expand true
  ```


#### 2. Check If You're Using Wayland (Instead of X11)

Qt's `QSystemTrayIcon` does not work on Wayland by default.

To check if you're using Wayland:
```sh
echo $XDG_SESSION_TYPE
```
If it returns `wayland`, try running your app in X11 mode:
```sh
export QT_QPA_PLATFORM=xcb
notify "Hello from Qt!"
```
Alternatively, use `Xwayland`:
```sh
QT_QPA_PLATFORM=xcb notify "Hello from Qt!"
```

### Other Recommendations

- If you're on X11, try ensuring a system tray is running.
- If you need full Qt-based notifications, consider frameworks like KNotifications (`find_package(KF6Notifications)`).

## Test

This code is to test all the categories

```sh
./bin/notify -t "System Alert" -m "System check completed" -c system && \
./bin/notify -t "System Alert 64" -m "Running on 64-bit system" -c system64 && \
./bin/notify -t "Alert" -m "Important system alert!" -c alert && \
./bin/notify -t "Critical Warning" -m "Flammable substance detected!" -c critical && \
./bin/notify -t "Radiation Alert" -m "Non-ionizing radiation detected!" -c radiation && \
./bin/notify -t "Notification" -m "You have a new notification!" -c notify && \
./bin/notify -t "Package Delivered" -m "Your package has been delivered!" -c delivery && \
./bin/notify -t "Plex Media" -m "Plex media server is running!" -c plex && \
./bin/notify -t "Warning Sign" -m "Check the system warnings!" -c sign && \
./bin/notify -t "System Tray" -m "This is a systray test!" -c systray && \
./bin/notify -t "Urgent Alert" -m "Immediate attention required!" -c urgent && \
./bin/notify -t "Secure Vault" -m "Vault access granted!" -c vault && \
./bin/notify -t "VPN Connected" -m "Your VPN is active!" -c vpn && \
./bin/notify -t "General Warning" -m "System warning issued!" -c warning && \
./bin/notify -t "Warning Type 1" -m "System warning type 1 detected!" -c warning1 && \
./bin/notify -t "Warning Type 2" -m "System warning type 2 detected!" -c warning2 && \
./bin/notify -t "Web Alert" -m "New activity detected online!" -c web && \
./bin/notify -t "Network Alert" -m "You are now connected to the network!" -c network && \
./bin/notify -t "Network Alternative" -m "Secondary network detected!" -c network2 && \
./bin/notify -t "YouTube Notification" -m "New video uploaded!" -c youtube && \
./bin/notify -t "YouTube 64" -m "HD video ready to stream!" -c youtube64
```