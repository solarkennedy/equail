# Script to configure Openwrt 
# Us as much or as little as you want

# Get openwrt bluetooth aware:
opkg install kmod-bluetooth
opkg install bluez-utils
# Wget the script to send beacons
wget -O /usr/sbin/send_beacon https://github.com/solarkennedy/equail/raw/master/openwrt_setup/send_beacon
# Test sending a beacon
/usr/sbin/send_beacon


# Now setup the xinetd service
opkg install xinetd
wget -O /usr/sbin/send_beacon_http https://github.com/solarkennedy/equail/raw/master/openwrt_setup/send_beacon_http
wget -O /etc/xinetd.conf https://github.com/solarkennedy/equail/raw/master/openwrt_setup/xinetd.conf
wget -O /etc/xinetd.d/equail https://github.com/solarkennedy/equail/raw/master/openwrt_setup/equail
/etc/init.d/xinetd start
/etc/init.d/xinetd enable


# I personally like avahi so I can connect to the server
# no matter with myserver.local
opkg install avahi-daemon
grep -q -e 'enable-dbus=no' /etc/avahi/avahi-daemon.conf || sed -i '/\[server\]/a enable-dbus=no' /etc/avahi/avahi-daemon.conf
/etc/init.d/avahi-daemon start
/etc/init.d/avahi-daemon enable
