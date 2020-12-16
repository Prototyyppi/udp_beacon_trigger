# UDP beacon trigger

For example to pause script execution until a trigger packet has been received.

## Usage

---

Beacons are sent with send_beacon tool. Instances of wait_for_beacon are listening for these packages and exit when a valid package is received. The beacon is attempted to be delivered to all ipv4 devices in local network so multiple scripts in different computers can be resumed by using one beacon only.

example/ directory contains a very simple shell script demonstrating the usage.

## Building and testing

---

To create the binaries in linux / osx:

```console
gcc -Wall -o send_beacon send_beacon.c
gcc -Wall -o wait_for_beacon wait_for_beacon.c
```

Note that the define BCN_TRIG_INET in send_beacon.c must be set to the appropriate ip range. This can be verified with ifconfig.

The binaries can be tested f. ex with the script in example folder.
