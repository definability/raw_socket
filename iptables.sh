sudo iptables -A OUTPUT -p tcp --tcp-flags RST RST -s 192.168.1.2 -d 91.198.36.48 -j DROP
