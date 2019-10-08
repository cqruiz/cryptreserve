
# Uninstall PreReqs
echo "*****************************"
echo "*	 Uninstall Pre Requisits  *"
echo "*****************************"

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    sudo apt-get -y remove autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev
    echo "apt-get remove autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"
        # Ubuntu
elif [[ "$OSTYPE" == "darwin"* ]]; then
    brew remove libpq
    brew remove libconfig
    brew remove gnutls
    brew remove libmicrohttpd
    
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
    cd src
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "hoel cleaned"
	echo "*****************************"
	cd ../..
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
cd glewlwyd
make clean
cd src
make clean
cd ..
if [ -d "build" ]
then
	cd build 
	echo "*****************************"
	echo "Uninstalling glewlwyd"
	echo "*****************************"
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "glewlwyd cleaned"
	echo "*****************************"
	cd ../..
    echo "Current Dir: ${PWD}"
	rm -rf glewlwyd/build
else
    cd ..
    echo "Current Dir: ${PWD}"
fi


###  INSTALLATION ###

# PreReqs
echo "*****************************"
echo "*	      Pre Requisits 	  *"
echo "*****************************"

if [[ "$OSTYPE" == "linux-gnu" ]]; then
    echo "apt-get autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"

    sudo apt-get -y install autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev

elif [[ "$OSTYPE" == "darwin"* ]]; then
    brew install autoconf
    brew install automake
    brew install libtool
    brew install sqlite3
    brew install libpq
    brew install libconfig
    brew install gnutls
    brew install libmicrohttpd
    echo "brew install autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"
fi

# liblfds
#echo "*****************************"
#echo "*       Setup liblfds 	  *"
#echo "*****************************"
#if [ ! -d "liblfds" ]; then
##	echo "*****************************"
#	echo "Dowload liblfds"
#	echo "*****************************"
#	git clone https://github.com/liblfds/liblfds.git
#fi

# Install liblfds 
#echo "*****************************"
#echo "Install liblfds"
#echo "*****************************"
###if [ -d "liblfds" ]; then
#	cd liblfds/liblfds/liblfds7.1.0/liblfds710/build/gcc_gnumake
#	echo "*****************************"
#	echo "liblfds building..."
##	echo "*****************************"
##	make
#	cd ../../../../../..
#	echo "*****************************"
#	echo "liblfds build completed."
#	echo "*****************************"
#fi

#json-c
echo "*****************************"
echo "Install json-c"
echo "Current Dir: ${PWD}"
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
echo "Current Dir: ${PWD}"

#openssl
echo "*****************************"
echo "Install openssl"
echo "*****************************"
if [ ! -d "openssl" ]; then
	echo "*****************************"
	echo "openssl installation required..."
	echo "*****************************"
	git clone https://github.com/openssl/openssl.git
	cd openssl 
	echo "*****************************"
	echo "openssl building..."
    echo "* Current Dir: ${PWD}"
	echo "*****************************"
	./config  
	make
	sudo make install_sw
	cd ..
	echo "*****************************"
	echo "openssl build completed."
	echo "*****************************"
else
	echo "*****************************"
	echo "openssl previously installed!"
    openssl version
	echo "*****************************"
fi
echo "Current Dir: ${PWD}"

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
    echo "Current Dir: ${PWD}"
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
echo "Current Dir: ${PWD}"

# Install Orcania
echo "*****************************"
echo "Install Orcania C Utilities"
echo "Current Dir: ${PWD}"
echo "*****************************"
if [ -d "orcania" ]; then
	echo "*****************************"
	echo "Orcania building..."
	echo "*****************************"
	cd orcania/src
    echo "Current Dir: ${PWD}"
	make && sudo make install 
	cd ../..
	echo "*****************************"
	echo "Orcania build completed."
	echo "*****************************"
fi
echo "Current Dir: ${PWD}"

# Install Yder for logging
echo "*****************************"
echo "Install Yder for Logging"
echo "Current Dir: ${PWD}"
echo "*****************************"
if [ -d "yder" ]; then
	echo "*****************************"
	echo "Yder building..."
	echo "*****************************"
	cd yder/src
    echo "Current Dir: ${PWD}"
    
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
echo "Current Dir: ${PWD}"

# Install Ulfius
echo "*****************************"
echo "Install Ulfius"
echo "Current Dir: ${PWD}"
echo "*****************************"

if [ -d "ulfius" ]; then
	echo "*****************************"
	echo "Ulfius building..."
	echo "*****************************"
	cd ulfius
    echo "Current Dir: ${PWD}"

    if [ ! -d "build"]; then
        mkdir build
        echo "Current Dir: ${PWD}"
    fi
    
    cd build
    echo "Current Dir: ${PWD}"


	cmake ..
	make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 && sudo make install

	cd ../..
	echo "*****************************"
	echo "Ulfius build completed."
	echo "*****************************"
fi
echo "Current Dir: ${PWD}"

# Install Hoel
echo "*****************************"
echo "Install Hoel - Database for sqlite3"
echo "*****************************"

if [ -d "hoel" ]; then
	echo "*****************************"
	echo "Hoel building..."
	echo "*****************************"

    cd hoel/src
    echo "Current Dir: ${PWD}"

  #  if [ ! -d "build" ]; then
   #     echo "mkdir build"
  #      mkdir build
   # fi
 #   echo "cd  build"
#	cd build

 #   echo "cmake hoel"
#	cmake ..

    echo $OSTYPE

    if [[ "$OSTYPE" == "linux-gnu" ]]; then
	    make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 
        echo "linux-gnu"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
	    make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 
        echo "darwin"
    fi

	sudo make install
	cd ../..
	echo "*****************************"
	echo "Hoel build completed."
	echo "*****************************"
fi
echo "Current Dir: ${PWD}"


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

        cd  libcbor/src
        echo "Current Dir: ${PWD}"
		cmake -DCMAKE_BUILD_TYPE=Release -DCBOR_CUSTOM_ALLOC=ON libcbor
		make
		make install
	fi
fi
echo "Current Dir: ${PWD}"

#Install Glewlwyd
echo "*****************************"
echo "Install Glewlwyd"
echo "Current Dir: ${PWD}"
echo "*****************************"

if [ ! -d "glewlwyd" ]; then
	echo "*****************************"
	echo "Git clone Glewlwyd."
	echo "*****************************"
	git clone https://github.com/blockchainbpi/glewlwyd.git
fi
echo "Tools dir: (cd glewlwyd):  ${PWD}"
echo "cd glewlwyd"
cd glewlwyd
echo "in glewlwyd: ${PWD}"

if [ ! -d "build" ]; then
	mkdir build
    echo "mkdir build"
    echo "curr dir glewlwyd still: ${PWD}"
fi
echo "*****************************"
echo "Glewlwyd Building..."
echo "*****************************"

echo "cd src: ${PWD}"
cd src
echo "make in dir: ${PWD}"
make
sudo make install
cd ../..
echo "*****************************"
echo "Done Buidling Glewlwyd."
echo "*****************************"
