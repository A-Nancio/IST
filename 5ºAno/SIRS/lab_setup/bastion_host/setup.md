# Bastion host VM set up
The bastion host serves as the firewall to protect the webserver and the database from external entities by filtering out unwanted packtes and masking their IP addresses. It requires no installation of external packages.

## 1. Network adapters
The adapters for the bastion host should 
|  Interface | Name | Attached to | IP Address | Adapter | 
|------------|------|-------------|------------------|---------|
| 1 | sw-1 | Internal Network | 192.168.0.10  | enp0s3 | 
| 2 | sw-2 | Internal Network | 192.168.1.254 | enp0s8 | 
| 3 | sw-3 | Internal Network | 192.168.2.254 | enp0s9 |  
| 4 |  -   | NAT Network      | INTERNET | enp0s10 | 

## 2. Netplan file
After adding the network adapters, to attach the respective IP addresses, change the `/etc/netplan/01-network-manager-all.yaml` to the following:

```
# Let networkManager manage all devices on this system
network:
  version: 2
  renderer: NetworkManager
  ethernets:
      enp0s3:
          addresses:
              - 192.168.0.10/24
          nameservers:
              addresses: [8.8.8.8, 8.8.4.4]
      enp0s8:
          addresses:
              - 192.168.1.254/24
          nameservers:
              addresses: [8.8.8.8, 8.8.4.4]
      enp0s9:
          addresses:
              - 192.168.2.254/24
          nameservers:
              addresses: [8.8.8.8, 8.8.4.4]
      enp0s10:
          dhcp4: yes
          nameservers:
            addresses: [8.8.8.8, 8.8.4.4]
```

## 3. Firewall

To build the firewall of the bastion host run: 
```shell
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
sudo /sbin/iptables -t nat -A PREROUTING -i enp0s3 -p tcp --dport 443 -j DNAT --to-destination 192.168.1.1:5000
sudo /sbin/iptables -A FORWARD -p tcp -d 192.168.1.1 -j ACCEPT
sudo /sbin/iptables -A FORWARD -p tcp -s 192.168.1.1 -j ACCEPT

# accept incoming mySQL queries from the webserver, redirecting them to the database
sudo /sbin/iptables -t nat -A PREROUTING -i enp0s8 -p tcp --dport 3306 -j DNAT --to-destination 192.168.2.4
sudo /sbin/iptables -A FORWARD -p tcp -d 192.168.2.4 -j ACCEPT
sudo /sbin/iptables -A FORWARD -p tcp -s 192.168.2.4 -j ACCEPT
```

## 4. Enable IPv4 forwarding

You should also enable IP forwarding in order to forward packets to the different machines. For that you need to edit `/etc/sysctl.conf` and uncomment the following line
```
net.ipv4.ip_forward=1
```