<center>
<img src="doc/img/logo.png" alt="table" />
</center>
<br>


# Qt Desktop Notify

A program to diplay notifications in Linux


## Build with CMake

```bash
cd qt-desktop-notify
mkdir build && cd build
cmake ..
make
../bin/qt-desktop-notify "Hello from Qt!"
```
## USage

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

<center>
<img src="doc/img/demo.png" alt="table" />
</center>
<br>
