# [CryptReserveNode](https://cryptreservellc.atlassian.net/wiki/spaces/BL/pages/33122/CryptReserveNode)
CryptReserveNode is the server hosting IPFS and the Blockchain.  Will expose a Wallet API.  This will eventually wind up on a docker image for ease of deploys.

## CryptReserve Passport System Setup and Installation

### Setup AWS

`sudo apt update`


* SSH
Add ~/.ssh/config and a git entry:

`Host ec2-18-222-40-162.us-east-2.compute.amazonaws.com`

	* User ubuntu

	* IdentityFile ~/.ssh/cryptreservenode-rsa.pem
	

`scp -i cryptreservenode-rsa.pem cryptreservenode-rsa.pem ubuntu@ec2-18-222-40-162.us-east-2.compute.amazonaws.com:/home/ubuntu/.ssh`


### Install IPFS

`mkdir ipfs`

`cd ipfs`

`wget https://dist.ipfs.io/go-ipfs/v0.4.15/go-ipfs_v0.4.15_linux-amd64.tar.gz`


`tar xvfz go-ipfs_v0.4.15_linux-amd64.tar.gz`


`rm go-ipfs_v0.4.15_linux-amd64.tar.gz`


`sudo mv go-ipfs`


`sudo mv go-ipfs/ipfs /usr/local/bin`


`rm -rf go-ipfs`


`echo 'export IPFS_PATH=/data/ipfs' >>~/.bash_profile`


`source ~/.bash_profile`


`sudo mkdir -p $IPFS_PATH`


`sudo chown ubuntu:ubuntu $IPFS_PATH`


`ipfs init -p server`

`ipfs config Datastore.StorageMax 20GB`

`ipfs config Addresses.Gateway /ip4/0.0.0.0/tcp/8080`

`sudo bash -c 'cat >/lib/systemd/system/ipfs.service <<EOL`
`..* [Unit]
Description=ipfs daemon
[Service]
ExecStart=/usr/local/bin/ipfs daemon --enable-gc
Restart=always
User=ubuntu
Group=ubuntu
Environment="IPFS_PATH=/data/ipfs"
[Install]
WantedBy=multi-user.target
EOL'`

`sudo systemctl daemon-reload`

`sudo systemctl enable ipfs.service`

`sudo systemctl start ipfs`

`sudo systemctl status ifs`


### C++

`sudo apt install gcc`

`sudo apt install make`

`Go-IPFS`

`git clone https://github.com/ipfs/go-ipfs.git`

`snap install go`

`cd go-ipfs`

`make install`


### Json-lib

`sudo apt install git`

`sudo apt install autoconf automake libtool`

`sudo apt install valgrind # optional`

`git clone https://github.com/json-c/json-c.git`

`cd json-c`

`sh autogen.sh`

* //followed by

`./configure ` # --enable-threading

`make`

`make install`


### To build and run the test programs:

`make check`

#optionally skip using valgrind
`make USE_VALGRIND=0 check`   


### Install Rest Client for C

>`sudo apt-get install libcurl.x86_64 libcurl4-openssl-dev`


### Jansson

`git clone git@github.com:akheron/jansson.git`

`cd jansson`

`autoreconf -i`

`./configure`

`make`

`sudo make install`

`make check`


### libulfius-dev (had issues with this)
`sudo apt-get install libulfius-dev`

`sudo apt-get install libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev`


### libulfius-dev (build from source)
`git clone https://github.com/babelouest/orcania.git`

`git clone https://github.com/babelouest/yder.git`

`git clone https://github.com/babelouest/ulfius.git`

`cd orcania/`

`make && sudo make install`

`cd ../yder/`

`apt-get update && apt-get install libsystemd-dev`

`make && sudo make install`

`cd ../ulfius/`

`make && sudo make install`

### OpenSSL

`mkdir /home/ubuntu/dev/cryptreserve/tools/openssl`

`cd /home/ubuntu/dev/cryptreserve/tools/openssl`

--`wget https://www.openssl.org/source/old/1.1.0/openssl-1.1.0g.tar.gz`

--`tar xzvf openssl-1.1.0g.tar.gz`

--`cd openssl-1.1.0g`

`git clone https://github.com/openssl/openssl.git`

`cd openssl

`./config`

`make`

`sudo make install`

`openssl version -a`

* Loads older version 1.0.2g which throws errors:
//usr/local/lib/libjwt.so: undefined reference to `ECDSA_SIG_get0'
//usr/local/lib/libjwt.so: undefined reference to `ECDSA_SIG_set0'
//usr/local/lib/libjwt.so: undefined reference to `EVP_MD_CTX_new'
//usr/local/lib/libjwt.so: undefined reference to `EVP_MD_CTX_free'

`--git@github.com:openssl/openssl.git`

`--cs openssl`

`--./config`

`--make`

`--make test`

`--sudo make install`

* Get rid of the man pages

### Check
`apt-get install check`


### LibOrcania-dev
`sudo apt-get update -y`

`sudo apt-get install -y liborcania-dev`

### Libjwt
`git clone https://github.com/babelouest/libjwt.git`

`cd libjwt`

`autoreconf -i`

`./configure`

`make all`

`make check`


### Install Hoel

`git clone https://github.com/babelouest/hoel.git`

`cd hoel/src/`

`make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1`

`sudo make install`


### glewlwyd

`sudo apt install glewlwd`

-or build-

#### using libgnutls28-dev

`sudo apt-get install autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls28-dev libconfig-dev libssl-dev libldap2-dev liboath-dev libcbor-dev`

`sudo apt-get install libcbor-dev`

`git clone https://github.com/babelouest/glewlwyd.git`

`mkdir glewlwyd/build`

`cd glewlwyd/build`

`cmake ..`

`make `

`sudo make install`

### JWT
`sudo add-apt-repository ppa:ben-collins/libjwt`


# Additional Apps/Setup

### Node

`curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -`

`sudo apt-get install -y nodes`

### React

`npm install redux`



### React Bindings and Developer Tools

`sudo npm install --save react-redux`

`sudo 
pm install --save-dev redux-devtools`
