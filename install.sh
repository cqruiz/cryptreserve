
echo "Uninstall and Clean orcania, yder, hoel, glewlwyd"
make -f  orcania/src/Makefile uninstall
make uninstall -f yder/src/Makefile
make uninstall -f hoel/src/Makefile
make uninstall -f ulfius/src/Makefile
make uninstall -f glewlwyd/Makefile
make clean -f orcania/src/Makefile
make clean -f yder/src/Makefile
make clean -f hoel/src/Makefile
make clean -f ulfius/src/Makefile
make clean -f glewlwyd/src/Makefile


# Uninstall Orcania
echo "Check/Git Clone Orcania - C Utilities"

if[ ! -d "orcania"]
then
	git clone https://github.com/babelouest/orcania.git
else
fi
cd orcania/src
sudo make uninstall	
make clean


#Uninstall Yder
echo "Check/Git Clone Yder - C Logger"

if[ ! -d "yder"]
then
	git clone https://github.com/babelouest/yder.git
else
fi
cd yder/src
sudo make uninstall	
make clean


echo "Check/Git Clone Yder - C Logger"

if[ ! -d "ulfius"]
then
	git clone https://github.com/babelouest/ulfius.git
else
fi
cd yder/src
sudo make uninstall	
make clean
echo "Remove Yder then Git load"
rm -rf yder
git clone https://github.com/babelouest/yder.git

echo "Remove Ulfius then git reload it"
rm -rf ulfius
git clone https://github.com/babelouest/ulfius.git

echo "Remove Hoel then git reload it"
rm -rf hoel
git clone https://github.com/babelouest/hoel.git

echo "Remove Glewyd then git reload it"
rm -rf glewlwyd
git clone https://github.com/babelouest/glewlwyd.git


echo "Uninstall and Clean orcania, yder, hoel, glewlwyd"
sudo make uninstall
sudo make uninstall -f yder/src/Makefile
sudo make uninstall -f hoel/src/Makefile
sudo make uninstall -f ulfius/src/Makefile
sudo make uninstall -f glewlwyd/Makefile



make clean -f orcania/src/Makefile
make clean -f yder/src/Makefile
make clean -f hoel/src/Makefile
make clean -f ulfius/src/Makefile
make clean -f glewlwyd/src/Makefile


echo "Remove Orcania then Git load"
rm -rf orcania
git clone https://github.com/babelouest/orcania.git

echo "Remove Yder then Git load"
rm -rf yder
git clone https://github.com/babelouest/yder.git

echo "Remove Ulfius then git reload it"
rm -rf ulfius
git clone https://github.com/babelouest/ulfius.git

echo "Remove Hoel then git reload it"
rm -rf hoel
git clone https://github.com/babelouest/hoel.git

echo "Remove Glewyd then git reload it"
rm -rf glewlwyd
git clone https://github.com/babelouest/glewlwyd.git


# Install Orcania
echo "Install Orcania C Utilities"

if[ ! -d "orcania/build"]
then
	mkdir orcania/build
fi

cd orcania/build
sudo cmake .. clean
cmake ..
make && sudo make install 
cd ../..


# Install Yder for logging
echo "Install Yder for Logging"

if[ ! -d "yder/build"]
then
	mkdir yder/build
fi

cd yser/build
sudo cmake .. clean
cmake ..
make && sudo make install
# Or make Y_DISABLE_JOURNALD=1 to disable journald logging


# Install Ulfius
echo "Install Ulfius REST API in C, using JSON"

if [ ! -d "ulfius/build"]
then
	mkdir ulfius/build
fi

cd ulfius/build
sudo cmake .. clean
cmake ..
make && sudo make install 
cd ../..


# Install Hoel
echo "Install Hoel - Database for sqlite3"

if [ ! -d "hoel/build"]
then
	mkdir hoel/build
fi

cd hoel/build
sudo cmake .. clean
cmake .. -DWITH_SQLITE3=on -DWITH_MARIADB=off -DWITHPGSQL=off
make && sudo make install

#cmake ..  -DWITH_MARIADB=off -DWITH_PGSQL=off
#make DISABLE_POSTGRESQL=1 DISABLE_MARIADB=1 -f hoel/src/Makefile


# Install Glewdwy OAuth2
echo "Install Glewdwyd OAuth2"

if [ ! -d "glewdwyd/build"]
then
	mkdir glewdwyd/build
fi

cd glewdwyd/build
sudo cmake .. clean
#DISABLE_MARIADB=1
mkdir glewdwyd/build
cd glewdwyd/build
cmake .. -DWITH_MARIADB=off -DWITHPGSQL=off
make && sudo make install


