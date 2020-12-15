echo "*****************************"
echo "cd into tools dir.."
echo "*****************************"
cd ../tools
echo `pwd`

# Uninstall PreReqs
#echo "*****************************"
#echo "*	 Uninstall Pre Requisits  *"
#echo "*****************************"
#echo "apt-get uninstall autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"

#if [[ "$OSTYPE" == "linux-gnu" ]]; then
#sudo apt-get -y remove autoconf automake sqlite3 libsqlite3-dev libpq-dev  libconfig-dev libssl-dev libldap2-dev liboath-dev 
 #       # ...
#elif [[ "$OSTYPE" == "darwin"* ]]; then
#sudo brew install remove autoconf automake libmicrohttpd-dev sqlite3 libsqlite3-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev
  #      # Mac OSX
#fi


# Uninstall Orcania
echo "*****************************"
echo "Check for Orcania installation - C Utilities"
echo "*****************************"

if [ ! -d "orcania" ]
then
	echo "*****************************"
	echo "  Orcania not installed"
	echo "*****************************"
	#git clone https://github.com/babelouest/orcania.git
fi

if [ -d "orcania" ]
then
	cd orcania
	echo "*****************************"
	echo "Uninstalling Orcania"
	echo "*****************************"
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "Orcania cleaned"
	echo "*****************************"
	cd ..
fi

#Uninstall Yder
echo "***************************************"
echo "Check for Yder installation - C Logger*"
echo "***************************************"

if [ ! -d "yder" ]
then
	echo "*****************************"
	echo "*     Yder not installed	  *"
	echo "*****************************"
	#git clone https://github.com/babelouest/yder.git
fi
if [ -d "yder" ]
then
	cd yder
	echo "*****************************"
	echo "Uninstalling Yder"
	echo "*****************************"
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "Yder cleaned"
	echo "*****************************"
	cd ..
fi

#Uninstall hoel
echo "***************************************************"
echo "Check for Hoel installation - Database abstraction*"
echo "***************************************************"

if [ ! -d "hoel" ]
then
	echo "*****************************"
	echo "*      Hoel not installed	  *"
	echo "*****************************"
#	git clone https://github.com/babelouest/hoel.git
fi
if [ -d "hoel" ]
then
	cd hoel
	echo "*****************************"
	echo "Uninstalling Hoel"
	echo "*****************************"
	sudo make uninstall	
	make clean
	echo "*****************************"
	echo "Hoel cleaned"
	echo "*****************************"
	cd ..
fi

#Uninstall ulfius
echo "*****************************************************************"
echo "Check for Ulfius installation - Web framework for Rest endpoints*"
echo "*****************************************************************"

if [ ! -d "ulfius" ]
then
	echo "*****************************"
	echo "Ulfius is not installed"
	echo "*****************************"
	#git clone https://github.com/babelouest/ulfius.git
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
echo "Check/Git Clone Glewlwyd"
echo "*****************************"

if [ ! -d "glewlwyd" ]
then
	echo "*****************************"
	echo " Glewlwyd is not installed."
	echo "*****************************"
	#git clone https://github.com/babelouest/glewlwyd.git
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
echo "*	     Install Pre Requisits 	  *"
echo "*****************************"
#echo "apt-get autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"
sudo apt-get update

REQUIRED_PKGS="automake libtool sqlite3 openssl libpq-dev libsqlite3-dev libpq-dev libconfig-dev libssl-dev libldap2-dev liboath-dev libsystemd-dev libgnutls28-dev libmicrohttpd-dev libcurl4-gnutls-dev"

for REQUIRED_PKG in $REQUIRED_PKGS; do
	PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
	echo Checking for $REQUIRED_PKG: $PKG_OK
	if [ "" = "$PKG_OK" ]; then
		echo "Missing $REQUIRED_PKG. Installing $REQUIRED_PKG."
		sudo apt-get -y install $REQUIRED_PKG 
	else
		echo "$REQUIRED_PKG alread installed.  Skipping $REQUIRED_PKG."
	fi
done


#sudo apt-get -y install --reinstall -dev 
echo "*****************************"
echo "*	       autoconf		  *"
echo "*****************************"
sudo apt-get -y install autoconf pkg-config

# liblfds
echo "*****************************"
echo "*       Setup liblfds 	  *"
echo "*****************************"
if [ ! -d "liblfds" ]; then
	echo "*************************************************************"
	echo "* Download liblfds (https://github.com/liblfds/liblfds.git) *"
	echo "*************************************************************"
	#git clone https://github.com/liblfds/liblfds.git
fi

# Install liblfds 
if [ -d "liblfds" ]; then
	echo "*****************************"
	echo "Install liblfds"
	echo "*****************************"

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

# Jansson 
echo "*****************************"
echo "Install Jansson"
echo "*****************************"
if [ ! -d "jansson" ]; then
        echo "*****************************"
        echo "jansson installation required..."
        echo "*****************************"
	git clone https://github.com/akheron/jansson.git
fi

if [ -d "jansson" ]; then
	cd jansson
	autoreconf -i
	./configure
	echo "*****************************"
	echo "jansson building..."
	echo "*****************************"
	make
	sudo make install
	cd ..
	echo "*****************************"
	echo "jansson build completed."
	echo "*****************************"
else
	echo "*****************************"
	echo "jansson installation error!"
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
	git clone https://github.com/babelouest/libjwt.git
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
	make && sudo make install
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
	#sudo make uninstall
	sudo apt-get -y install libmysqlclient-dev 
	#make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 && sudo make install
	make && sudo make install
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
	#sudo make uninstall
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
fi
if [ -d "libcbor" ]; then
	echo "*****************************"
	echo "LibCBOR Building..."
	echo "*****************************"
	cd libcbor
	cmake -DCMAKE_C_FLAGS=-fPIC -DCMAKE_BUILD_TYPE=Release -DCBOR_CUSTOM_ALLOC=ON 
	make
	sudo make install
	cd ..
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


#Install Solidity
echo "*****************************"
echo "Install Solidity"
echo "*****************************"

if [ ! -d "solidity" ]; then
	echo "*****************************"
	echo "Git clone Solidity."
	echo "*****************************"
	git clone --recursive https://github.com/ethereum/solidity.git
	echo "*****************************"
	echo "Dependencies: Boost..."
	echo "*****************************"
	cd solidity
	./scripts/install_deps.sh

	echo "*****************************"
	echo "Solidity Building..."
	echo "*****************************"
	mkdir build
	cd build
	cmake .. && make
	cd ../..
	echo "*****************************"
	echo "Done Buidling Solidity."
	echo "*****************************"

fi


