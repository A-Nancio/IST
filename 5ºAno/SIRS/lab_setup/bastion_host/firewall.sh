## in VM2
# flush previous rules
sudo /sbin/iptables -F
sudo /sbin/iptables --delete-chain
sudo /sbin/iptables -t nat -F

# redirect requests to the internet
sudo /sbin/iptables -t nat -A POSTROUTING  -o enp0s10 -j MASQUERADE

#reject all other traffic
sudo /sbin/iptables -P INPUT DROP
sudo /sbin/iptables -P FORWARD DROP
sudo /sbin/iptables -P OUTPUT ACCEPT

# accept incoming HTTP packets from the external network, redirecting them to the webserver
sudo /sbin/iptables -t nat -A PREROUTING -d 192.168.0.10 -p tcp --dport 443 -j DNAT --to-destination 192.168.1.1:5000
sudo /sbin/iptables -A FORWARD -p tcp -d 192.168.1.1 -j ACCEPT
sudo /sbin/iptables -A FORWARD -p tcp -s 192.168.1.1 -j ACCEPT

# accept incoming mySQL queries from the webserver, redirecting them to the database
sudo /sbin/iptables -t nat -A PREROUTING -i enp0s8 -p tcp --dport 3306 -j DNAT --to-destination 192.168.2.4
sudo /sbin/iptables -A FORWARD -p tcp -d 192.168.2.4 -j ACCEPT
sudo /sbin/iptables -A FORWARD -p tcp -s 192.168.2.4 -j ACCEPT