# Database VM set up
The database VM serves the purpose to store data for our service in a database. For simplicity of this project, the database only keeps a table of sensistive data providade by the clients, in particular their username, password and phone number.

## 1. Built with
- mySQL - A a relational database management system developed by Oracle
  
## 2. Network adapters
The adapters for the bastion host should 
|  Interface | Name | Attached to | IP Address | Adapter | 
|------------|------|-------------|------------|---------|
| 1 | sw-3 | Internal Network | 192.168.2.4    | enp0s3 | 

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
              - 192.168.2.4/24
          routes:
              - to: 0.0.0.0/0
                via: 192.168.2.254
          nameservers:
              addresses: [8.8.8.8, 8.8.4.4]
```
## 3. Add the certificates keys to mySQL
### ...
## 4. Preparing the database
After installing mySQL run `sudo mysql -u root` to open the mySQL on root exectue the following SQL instructions:

```sql
CREATE DATABASE MusicMarkt
USE MusicMarkt
CREATE TABLE UserInfo(UserName varchar(255) NOT NULL UNIQUE, 
            UserPassword varchar(255) NOT NULL,
            cellPhoneNumber varchar(255) NOT NULL UNIQUE,
            salt varbinary(128) NOT NULL UNIQUE, 
            primary key(UserName));

CREATE USER 'musicMarkt'@'192.168.1.1' IDENTIFIED BY 'dees';
GRANT SELECT, INSERT, UPDATE, DELETE ON MusicMarkt.UserInfo TO 'musicMarkt'@'192.168.1.1';
FLUSH PRIVILEGES;
```

After creating the database, move the files in the `certificates_keys` corresponding to the necessary certificates and keys directory to `/var/lib/mysql/`, necessary for the database to use TLS while communicating with the webserver. In order for the database to be able to receive requests from external machines and apply SSL, you have to edit the corresponding `sudo nano /etc/mysql/mysql.conf.d/mysqld.cnf` file and change/add the lines:
```
[mysqld]
ssl-cert=/var/lib/mysql/database-cert.pem
ssl-key=/var/lib/mysql/database-key.pem
ssl-ca=/var/lib/mysql/ca.pem
require_secure_transport=ON

bind-address            = 192.168.2.4
mysqlx-bind-address     = 192.168.2.4
```

To apply these changes:

```
sudo service mysql restart
```