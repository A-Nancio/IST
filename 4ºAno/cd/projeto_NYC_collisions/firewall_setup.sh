# --------------------- on VM2
sudo /sbin/iptables -F
sudo iptables --delete-chain


# rule to foward all http connections to vm3
sudo /sbin/iptables -t nat -A PREROUTING -p tcp --sport 80 -j DNAT --to-destination 192.168.1.1

#all ssh connections from the external network are redirected to VM4
...

#requests from the internal 192.168.2.0/24 are only accepted if destined to the ssh port
...

# all other traffic is rejected
sudo /sbin/iptables -P INPUT DROP
sudo /sbin/iptables -P OUTPUT ACCEPT



# -----------------------on VM3
#accept http connections from both the internal and external networks
...

#accepts ssh connections from the internal networks
...

#does not start new connections
...

# reject everything else
...

# ......................on VM4
#accept only shh requests
...

#is able to open ssh connections to both external network and DMZ
...