
# Uninstall PreReqs
echo "*****************************"
echo "*	 Uninstall Pre Requisits  *"
echo "*****************************"

if [[ "$OSTYPE" == "linux-gnu" ]]; then
sudo apt-get -y remove autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev
echo "apt-get remove autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"
        # ...
elif [[ "$OSTYPE" == "darwin"* ]]; then
brew remove autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev
echo "brew remove autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"
        # Mac OSX
fi


# Uninstall Orcania
echo "*****************************"
echo "Check/Git Clone Orcania - C Utilities"
echo "*****************************"

if [ ! -d "orcania" ]
then
	echo "*****************************"
	echo "git clone orcania."
	echo "*****************************"
	git clone https://github.com/blockchainbpi/orcania.git
fi

if [ -d "orcania" ]
then
	cd orcania
	echo "*****************************"
	echo "Uninstalling orcania"
	echo "*****************************"
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "orcania cleaned"
	echo "*****************************"
	cd ..
fi

#Uninstall Yder
echo "*****************************"
echo "Check/Git Clone Yder - C Logger"
echo "*****************************"

if [ ! -d "yder" ]
then
	echo "*****************************"
	echo "*     git clone yder	  *"
	echo "*****************************"
	git clone https://github.com/blokchainbpi/yder.git
fi
if [ -d "yder" ]
then
	cd yder
	echo "*****************************"
	echo "Uninstalling yder"
	echo "*****************************"
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "yder cleaned"
	echo "*****************************"
	cd ..
fi

#Uninstall hoel
echo "*****************************"
echo "Check/Git Clone hoel"
echo "*****************************"

if [ ! -d "hoel" ]
then
	echo "*****************************"
	echo "*      git clone hoel	  *"
	echo "*****************************"
	git clone https://github.com/blockchainbpi/hoel.git
fi
if [ -d "hoel" ]
then
	cd hoel
	echo "*****************************"
	echo "Uninstalling hoel"
	echo "*****************************"
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "hoel cleaned"
	echo "*****************************"
	cd ..
fi

#Uninstall ulfius
echo "*****************************"
echo "Check/Git Clone Ulfius"
echo "*****************************"

if [ ! -d "ulfius" ]
then
	echo "*****************************"
	echo "git clone Ulfius."
	echo "*****************************"
	git clone https://github.com/blockchainbpi/ulfius.git
fi
if [ -d "ulfius" ]
then
	cd ulfius
	echo "*****************************"
	echo "Uninstalling Ulfius"
	echo "*****************************"
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "Ulfius cleaned"
	echo "*****************************"
	cd ..
fi

#Uninstall Glewlwyd
echo "*****************************"
echo "Check/Git Clone glewlwyd"
echo "*****************************"

if [ ! -d "glewlwyd" ]
then
	echo "*****************************"
	echo "git clone glewlwyd."
	echo "*****************************"
	git clone https://github.com/blockchainbpi/glewlwyd.git
fi
if [ -d "glewlwyd/build" ]
then
	cd glewlwyd/build 
	echo "*****************************"
	echo "Uninstalling glewlwyd"
	echo "*****************************"
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "glewlwyd cleaned"
	echo "*****************************"
	cd ../..
	rm -rf glewlwyd/build
fi


###  INSTALLATION ###

# PreReqs
echo "*****************************"
echo "*	      Pre Requisits 	  *"
echo "*****************************"
echo "apt-get autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"

sudo apt-get -y install autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev


# liblfds
echo "*****************************"
echo "*       Setup liblfds 	  *"
echo "*****************************"
if [ ! -d "liblfds"]; then
	echo "*****************************"
	echo "Dowload liblfds"
	echo "*****************************"
	git clone https://github.com/liblfds/liblfds.git
fi

# Install liblfds 
echo "*****************************"
echo "Install liblfds"
echo "*****************************"
if [ -d "liblfds" ]; then
	cd liblfds/liblfds/liblfds7.1.0/liblfds710/build/gcc_gnumake
	echo "*****************************"
	echo "liblfds building..."
	echo "*****************************"
	make
	cd ../../../../../..
	echo "*****************************"
	echo "liblfds build completed."
	echo "*****************************"
fi

#json-c
echo "*****************************"
echo "Install json-c"
echo "*****************************"
if [ ! -d "json-c" ]; then
	echo "*****************************"
	echo "json-c installation required..."
	echo "*****************************"
	git clone https://github.com/json-c/json-c.git
	cd json-c
	sh autogen.sh
	echo "*****************************"
	echo "json-c building..."
	echo "*****************************"
	./configure 
	make
	sudo make install
	cd ..
	echo "*****************************"
	echo "json-c build completed."
	echo "*****************************"
else
	echo "*****************************"
	echo "json-c alreadey installed."
	echo "*****************************"
fi

#openssl
echo "*****************************"
echo "Install openssl"
echo "*****************************"
if [ ! -d "openssl" ]; then
	echo "*****************************"
	echo "openssl installation required..."
	echo "*****************************"
	git clone https://github.com:openssl/openssl.git
fi

if [ -d "openssl" ]; then
	cd openssl 
	echo "*****************************"
	echo "openssl building..."
	echo "*****************************"
	./config  
	make
	sudo make install
	cd ..
	echo "*****************************"
	echo "openssl build completed."
	echo "*****************************"
else
	echo "*****************************"
	echo "openssl installation error!"
	echo "*****************************"
fi

# libjwt 
echo "*****************************"
echo "Install libjwt"
echo "*****************************"
if [ ! -d "libjwt" ]; then
	echo "*****************************"
	echo "libjwt installation required..."
	echo "*****************************"
	git clone https://github.com:blockchainbpi/libjwt.git
fi

if [ -d "libjwt" ]; then
	cd libjwt 
	autoreconf -i
	echo "*****************************"
	echo "libjwt building..."
	echo "*****************************"
	./configure OPENSSL_CFLAGS=" " OPENSSL_LIBS="-lssl -lcrypto"
	make 
	sudo make install
	cd ..
	echo "*****************************"
	echo "libjwt build completed."
	echo "*****************************"
else
	echo "*****************************"
	echo "libjwt installation error!"
	echo "*****************************"
fi

# Install Orcania
echo "*****************************"
echo "Install Orcania C Utilities"
echo "*****************************"
if [ -d "orcania" ]; then
	echo "*****************************"
	echo "Orcania building..."
	echo "*****************************"
	cd orcania/src
	make && sudo make install 
	cd ../..
	echo "*****************************"
	echo "Orcania build completed."
	echo "*****************************"
fi

# Install Yder for logging
echo "*****************************"
echo "Install Yder for Logging"
echo "*****************************"
if [ -d "yder" ]; then
	echo "*****************************"
	echo "Yder building..."
	echo "*****************************"
	cd yder/src
    
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
        make
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        make Y_DISABLE_JOURNALD=1
    fi
	
    sudo make install
	cd ../..
	echo "*****************************"
	echo "Yder build completed."
	echo "*****************************"
fi

# Install Hoel
echo "*****************************"
echo "Install Hoel - Database for sqlite3"
echo "*****************************"

if [ -d "hoel" ]; then
	echo "*****************************"
	echo "Hoel building..."
	echo "*****************************"
	cd hoel/src
	make clean
	sudo make uninstall
	sudo apt-get -y install libmysqlclient-dev 
    if [[ "$OSTYPE" == "linux-gnu" ]]; then
	    make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 
    elif [[ "$OSTYPE" == "darwin"* ]]; then
	    make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 
    fi

	sudo make install
	cd ../..
	echo "*****************************"
	echo "Hoel build completed."
	echo "*****************************"
fi

# Install Ulfius
echo "*****************************"
echo "Install Ulfius"
echo "*****************************"

if [ -d "ulfius" ]; then
	echo "*****************************"
	echo "Ulfius building..."
	echo "*****************************"
	cd ulfius/src
	make clean
	sudo make uninstall
	make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 && sudo make install
	cd ../..
	echo "*****************************"
	echo "Ulfius build completed."
	echo "*****************************"
fi

#Install libcbor
echo "*****************************"
echo "Install LibCBOR"
echo "*****************************"

if [ ! -d "libcbor" ]; then
	echo "*****************************"
	echo "Git clone LibCBOR."
	echo "*****************************"
	git clone https://github.com/PJK/libcbor
	if [ -d "libcbor" ]; then
		echo "*****************************"
		echo "LibCBOR Building..."
		echo "*****************************"

		cmake -DCMAKE_BUILD_TYPE=Release -DCBOR_CUSTOM_ALLOC=ON libcbor
		make
		make install
	fi
fi

#Install Glewlwyd
echo "*****************************"
echo "Install Glewlwyd"
echo "*****************************"

if [ ! -d "glewlwyd" ]; then
	echo "*****************************"
	echo "Git clone Glewlwyd."
	echo "*****************************"
	git clone https://github.com/babelouest/glewlwyd.git
fi

if [ ! -d "glewlwyd/build" ]; then
	mkdir glewlwyd/build
fi
echo "*****************************"
echo "Glewlwyd Building..."
echo "*****************************"

cd glewlwyd/build
sudo cmake ..
make
sudo make install
cd ../..
echo "*****************************"
echo "Done Buidling Glewlwyd."
echo "*****************************"
