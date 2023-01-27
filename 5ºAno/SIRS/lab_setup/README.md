# Projeto SIRS
This is the project for the SIRS course developed by:
**Group 9**
- António Venâncio 93689
- Guilherme Carvalho 96868
- Miguel Gonçalves 94238

## 1. Set up
Each virtual machine it should first be given the ability for each machine to connect to the internet in order to install the necessary packages.

The order of set up of the different machines should be the following:

1. Database
2. Bastion Host
3. Webserver
4. Client

The instructions for each virtual machine can be found in their respective folders.

## 2. Testing the different security aspects
### 2.1. Encrypted channels
To verify if the packets are going through the different channels encrypted, on each virtual machine wirseshark is installed and can capture the traffic in each interface of the VM.

### 2.2. Database security
Verifying if the passwords are stored fully encrypted can be done by checking the database records on the database VM as root. 

### 2.3. HTTPS usage
By using wireshark on the bastion host, it is possible to test for encrypted packets headed for port 443, corresponding to the HTTPS protocol. The client can verify the correctness of the certificates in the browser when loading the webpage.

### 2.4. Webserver security
It is possible to check for the two-factor authentication features by testing different correct and non-correct PIN codes when submitting it, while also checking for it's lock feature to prevent online brute force attacks. Resistance for replay attacks can be tested by submitting old generated PIN codes.