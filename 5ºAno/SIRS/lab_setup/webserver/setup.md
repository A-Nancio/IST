# Webserver VM setup
The webserver provides an interface for external users to interact with the MusicMarkt services by allowing to log-in and create an account.

# Database VM set up
The database VM serves the purpose to store data for our service in a database. For simplicity of this project, the database only keeps a table of sensistive data providade by the clients, in particular their username, password and phone number.

## 1. Built with
- Python - a programming language that supports the web application
- Miniconda - an open source package management system and environment management system for Python

## 2. Network adapters
The adapters for the bastion host should 
|  Interface | Name | Attached to | IP Address | Adapter | 
|------------|------|-------------|------------|---------|
| 1 | sw-2   | Internal Network | 192.168.1.1  | enp0s3 | 

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
              - 192.168.1.1/24
          routes:
              - to: 0.0.0.0/0
                via: 192.168.1.254
          nameservers:
              addresses: [8.8.8.8, 8.8.4.4]
```

## Python environment
The webserver can only be run after creating a python environment using python3.8 and installing the following packages:

```
pip install flask
pip install flask-login
pip install mysql-connector-python
pip install cryptography
```

Copy the files in the current folder to the working working directory in the VM and start the webserver by running. **Note the database needs to be already set up, otherwise the webserver will crash:**
```
flask --app webserver run --host=192.168.1.1 --cert=certificates_keys/webserver-cert.pem --key=certificates_keys/webserver-key.pem
``` 
With this the webserver is active and ready for requests.