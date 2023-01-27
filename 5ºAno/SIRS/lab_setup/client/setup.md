# Client VM set up
The client VM serves the example purpose of a client that wishes to interact with the services provided by MusicMarkt. This VM requires no installation since a user shouldn't be dependant on the installation of third party software to acess MusicMarkt.

## 1. Network adapters
The adapters for the bastion host should 
|  Interface | Name | Attached to | IP Address | Adapter | 
|------------|------|-------------|------------|---------|
| 1 | sw-3 | Internal Network | 192.168.0.100    | enp0s3 | 

## 3. Netplan file
After adding the network adapters, to attach the respective IP addresses, change the `/etc/netplan/01-network-manager-all.yaml` to the following:

```
# Let networkManager manage all devices on this system
network:
  version: 2
  renderer: NetworkManager
  ethernets:
      enp0s3:
          addresses:
              - 192.168.0.100/24
          routes:
              - to: 0.0.0.0/0
                via: 192.168.0.10
          nameservers:
              addresses: [8.8.8.8, 8.8.4.4]
```
## 4. Access to MusicMarkt
To access the service, it is only necessary to open the browser and open the page `https://192.168.0.10`. The browser will give a warning relevant to the signing of an unknown certificate authority, since it was generated and self-signed during the development of this project. For the sake of this project, the user will believe it is a trusted CA and proceed with the risk.

## 5. Two-factor authenticaton
In order to simulate the capabilities of a two-factor authentication service, a terminal is created in the client VM to represent a second device in the posession of the user. The two factor authentication works

Regarding the two-factor authentication, a terminal based on the client side is used to simulate the application. When logging-in into the website, a 6-digit pin is asked. The client should then open a terminal on its environment with directory where the file `authenticator.py` is located and run the following command:
```
python3 authenticator.py 
```
This generates a 6 digit PIN code to submit at two-factor authentication page, keep in mind that the code is only valid for 20 seconds, after it the script needs to be ran again to generate the new currently valid PIN code.
