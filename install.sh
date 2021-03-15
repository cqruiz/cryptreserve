show_help() {
    echo "Usage:
    
    ${0##*/}  [-h][-c][-v][-d][-r][-l LIBRARY]

Options:

    -h, --help
        display thiws help and exit

    -c, --clean
        clean the libraries and dependancies 
        
    -v, --verbose
        verbose
    
    -b, --build
        build the libraries and dependencies 

    -d, --debug
        debug run the install script libraries and dependencies 

    -r, --remove
        remove libraries and dependencies 

    -l, --lib
        rebuild particular library
        {glewlwyd,hoel,json-c,libcbor,linconfig,libjwt,openssl,orcania,ulfius,yder}
"    
    
    
}

log()
{
    if [ $verbose -eq 1 ]; then
        echo $1
    fi
}

download_git_repo()
{
    REPO=$1
    shift;
    # Having shifted, the rest is now comments ...
    COMMENTS=$@

    if [ ! -d $REPO ]; then
        echo "*********************************"
        echo "*    Git Clone repo=$REPO   *"
        echo "*********************************"
        if [ $debug -eq 0 ]; then
            git clone https://github.com/vergaraed/$REPO.git
        else
            log "Running in debug mode, not cloning $REPO."
        fi
    else
        echo "$REPO already exists."
    fi
}


# A POSIX variable
OPTIND=1         # Reset in case getopts has been used previously in the shell.

# Initialize our own variables:
verbose=0
clean=0
build=0
all=1
debug=0
remove=0

while getopts "h?vcbdrl:" opt; do
    echo "$opt"

    case "$opt" in
    h|\?)
        show_help
        exit 0
        ;;
    v)  
        verbose=1
        ;;
    c)  
        clean=1
        all=0
        ;;
    b) 
        build=1
        all=0
        ;;
    d) 
        debug=1
        ;;
    r) 
        remove=1
        ;;
    l)  
        library=$OPTARG
        ;;
    esac
done

# shift $((OPTIND-1))

 [ "${1:-}" = "--" ] && shift

log "verbose=$verbose, all=$all, clean=$clean, build=$build, debug=$debug, remove=$remove, Leftovers: $@"

if [ $clean -eq 1 ]; then
   log "Cleaning Option set - Cleaning all library Builds"
fi

if [ $build -eq 1 ]; then
    log "Build=$build Option set - Rebuilding all libraries"
fi

if [ $debug -eq 1 ]; then
    log "Debug Option set - Test without actually cleaning or installing."
fi

if [ $remove -eq 1 ]; then
    log "Remove Option set - Remove all libraries."
    remove=1
    clean=1
fi

if [[ ( "$all" -eq 1 ) || ( "$clean" -eq 1 && "$build" -eq 1 ) ]]; then
    log "Build All By Default - Cleaning and Rebuilding all libraries"
    clean=1
    build=1
    all=1
fi


##########################################
################   Start   ###############
##########################################

if [ ! -d tools ]; then
    echo "Create Tools directory." 
    mkdir tools
fi
# run under the tools dir.
cd tools

#######################################################################################
################################   Cleaning Libraries  ################################
#######################################################################################

if [ $clean -eq 1 ]; then

    echo "
    ###########################################
    #Conditional Clean - all default, -c clean#
    ###########################################"
    
    # Uninstall PreReqs
    echo "*****************************"
    echo "*	 Uninstall Prerequisites  *"
    echo "*****************************"

    if [ $debug -eq 0 ]; then

        if [[ "$OSTYPE" == "linux-gnu" ]]; then
            echo  "Ubuntu"
            sudo apt-get -y remove autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev
            echo "apt-get remove autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"
                # Ubuntu
        elif [[ "$OSTYPE" == "darwin"* ]]; then
            echo "Mac OSX"
            brew remove libmicrohttpd
            brew remove liboauth
            brew remove gnutls
            echo "brew remove libmicrohttpd liboath gnutls   "
            # Mac OSX
        fi
    else
        log "Debug Run Not Uninstalling Prerequisits."
    fi

    # Uninstall Orcania
    echo "*******************************************"
    echo "*  Check/Uninstall Orcania - C Utilities  *"
    log "*   Current Dir: ${PWD}  *"
    echo "*******************************************"
 
    if [ -d orcania ]; then
        echo "*****************************"
        echo "*    Uninstalling orcania   *"
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            cd orcania
            make clean
            cd src
            sudo make uninstall	
            log " ** make clean **"
            make clean
            cd ../..
            log ${PWD}
            if [ $remove -eq 1 ]; then
                echo "****-> Removing all of Orcania <-****"
                rm -rf orcania
            fi
        fi
        echo "*****************************"
        echo "*      orcania cleaned      *"
        echo "*****************************"
    else
        log "Debug Mode Not Unistalling Ocania"
    fi

    #Uninstall Yder
    echo "***********************************"
    echo "* Check/Uninstall Yder - C Logger *"
    log "* Current Dir: ${PWD} *"
    echo "***********************************"
    
    if [ -d yder ]; then
        echo "*****************************"
        echo "*     Uninstalling yder     *"
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            cd yder/src
            sudo make uninstall	
            echo " ** make clean **"
            make clean
            cd ../..
            if [ $remove -eq 1 ]; then
                echo "**** Removing all of Yder ****"
                rm -rf yder
            fi
        else
            log "Debug Run Not Uninstalling"
        fi
        echo "*****************************"
        echo "*      yder cleaned         *"
        echo "*****************************"
    fi

    #Uninstall hoel
    echo "*****************************"
    echo "*    Check/Uninstall Clone hoel   *"
    log "*    Current Dir: ${PWD}    *"
    echo "*****************************"

    if [ -d hoel ]; then
        echo "*****************************"
        echo "*     Uninstalling hoel     *"
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            cd hoel
            if [ -d build ]; then
                cd build
                echo " ** make clean build **"
                make clean
                make uninstall
                cd ..
            fi
            echo " ** make clean **"
            make clean
            cd ..
            if [ $remove -eq 1 ]; then
                echo "****   Removing all of Hoel   ****"
                sudo rm -rf hoel
            fi
        else
            log "Debug Run Not Uninstalling"
        fi
        echo "*****************************"
        echo "*       hoel cleaned        *"
        echo "*****************************"
    fi

    #Uninstall ulfius
    echo "*****************************"
    echo "*  Check/Uninstall Ulfius   *"
    log "*    Current Dir: ${PWD}    *"
    echo "*****************************"

    if [ -d ulfius ]; then
        cd ulfius
        echo "*****************************"
        echo "*    Uninstalling Ulfius    *"
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            sudo make uninstall	
            echo " ** make clean **"
            make clean
            cd ..
            if [ $remove -eq 1 ]; then
                echo "Removing all of Ulfius."
                sudo rm -rf ulfius
            fi
        else
            log "Debug Run Not Uninstalling"
        fi
        echo "*****************************"
        echo "*      Ulfius cleaned       *"
        echo "*****************************"
    fi

    #Uninstall Glewlwyd
    echo "*****************************"
    echo "* Check/Uninstall glewlwyd  *"
    log "\*  Current Dir: ${PWD}     \*"
    echo "*****************************"

    if [ -d glewlwyd ]; then
        echo "*****************************"
        echo "*      clean glewlwyd       *"
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            log "cd glewlwyd"
            cd glewlwyd
            log "make clean glewlwyd"
            make clean
            log "cd src"
            cd src
            make clean
            cd ..
            if [ -d build ]; then
                cd build 
                log "** make clean **"
                make clean
                echo "*****************************"
                echo "*      glewlwyd cleaned     *"
                echo "*****************************"
                cd ../..
                log "Current Dir: ${PWD}"
            else
                log "cd .."
                cd ..
                log "Glewlwyd - Current Dir: ${PWD}"
            fi
            if [ $remove -eq 1 ]; then
                echo "Removing all of Glewlwyd."
                sudo rm -rf glewlwyd
            fi
        fi
    fi

    echo "***********************************"
    echo "*   Check/Uninstall LibOAuth      *"
    log "*   Current Dir: ${PWD}            *"
    echo "***********************************"

    if [ -d liboauth ]; then
        echo "*****************************"
        echo "* LibOAuth Uninstalling...  *"
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            cd liboauth
            log "in liboauth: ${PWD}"
            log "cd src: ${PWD}"
            cd src
            log "make clean in dir: ${PWD}"
            make clean
            make uninstall
            cd ../..
            if [ $remove -eq 1 ]; then
                echo "Removing all of LibOAuth."
                sudo rm -rf liboauth
            fi
        else
            log "Not Uninstalling, Debug mode only."
        fi
        echo "*****************************"
        echo "* Done Unistalling LibOAuth *"
        echo "*****************************"
    fi

    echo "*****************************"
    echo "*   Check/Uninstall LibConfig     *"
    log "\*   Current Dir: ${PWD}     \*"
    echo "*****************************"

    echo "*****************************"
    echo "*  LibConfig Uninstalling..  *"
    echo "*****************************"

    if [ -d libconfig ]; then

        if [ $debug -eq 0 ]; then
            log "cd libconfig"
            cd libconfig
            echo "in libconfig: ${PWD}"
            echo "make clean in dir: ${PWD}"
            make clean
            make uninstall
            cd ..
            if [ $remove -eq 1 ]; then
                echo "Removing all of LibConfig."
                sudo rm -rf libconfig
            fi
        else
            echo "************************************"
            echo "* Done Debug Unistalling LibConfig *"
            echo "************************************"
        fi
        echo "*****************************"
        echo "* Done Unistalling LibConfig *"
        echo "*****************************"
        log "Done with Uninstall - now in tools directory: ${PWD}"
    fi
   
    echo "***********************************"
    echo "*   Check/Uninstall JSON-C        *"
    echo "*   Current Dir: ${PWD}           *"
    echo "***********************************"
            
    echo "*****************************"
    echo "json-c cleaning..."
    echo "*****************************"

    if [ $debug -eq 0 ]; then
        
        if [ -d json-c ]; then
            cd json-c
            log "Current Dir: ${PWD}"
            make clean
            log " ** make uninstall **"
            sudo make uninstall
            cd ..
            if [ $remove -eq 1 ]; then
                echo "Removing all of JSON-C."
                sudo rm -rf json-c
            fi
        fi
        echo "*****************************"
        echo "* json-c clean/uninstall completed *"
        log "* Current Dir: ${PWD} *"
        echo "*****************************"
    else
        
        log "Not Cleaning/Uninstalling, Debug mode only."
    fi

    ####################################
    # Uninstall libcbor
    ####################################
    echo "*****************************"
    echo "Uninstall LibCBOR"
    echo "*****************************"

    if [ -d libcbor ]; then
        echo "*****************************"
        echo "Clean LibCBOR."
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            echo "*****************************"
            echo "LibCBOR Building..."
            echo "*****************************"
            if [ -d build ]; then
                cd  libcbor/build
                log "Current Dir: ${PWD}"
                make clean
                cd ../..
            fi
            rm -rf libcbor/build

            if [ $remove -eq 1 ]; then
                echo "Removing all of libcbor."
                sudo rm -rf libcbor
            fi

        else
            log "Debug Mode - Not Uninstalling LibCbor."
        fi
    fi


    # libjwt
    echo "*****************************"
    echo "* Uninstall libjwt          *"
    echo "*****************************"

    echo "*****************************"
    echo "libjwt cleaning..."
    echo "*****************************"

    if [ -d libjwt ]; then
        if [ $debug -eq 0 ]; then
            cd libjwt
            make clean
            cd ..
            if [ $remove -eq 1 ]; then
                echo "Removing all of libcbor."
                sudo rm -rf libjwt
            fi
        else
            log "Debug Mode - Not Uninstalling."
        fi

        echo "*****************************"
        echo "libjwt clean completed."
        echo "*****************************"
            
    else
        echo "*****************************"
        echo "libjwt uninstallation error!"
        echo "*****************************"
    fi
    
    #openssl
    echo "*****************************"
    echo "*    Uninstall openssl      *"
    echo "*****************************"
    if [ -d openssl ]; then
        echo "*****************************"
        echo "*     openssl clean...      *"
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            cd openssl 
            echo "*****************************"
            echo "*    openssl uninstall...   *"
            log "* Current Dir: ${PWD}"
            echo "*****************************"
           ## make clean
            echo "*** make uinstall *** "
            ##sudo make uninstall
            cd ..
            if [ $remove -eq 1 ]; then
                echo "Removing all of libcbor."
             ##   sudo rm -rf openssl
            fi
        else
            log "Debug Mode - Not Installing."
        fi
        echo "*****************************"
        echo "openssl clean unsintall completed."
        echo "*****************************"
    else
        echo "*****************************"
        echo "openssl doesn't exist - Need to install!"
        echo "*****************************"
    fi
    log "Current Dir: ${PWD}"


fi

#######################################################################################
################################  Building Libraries  #################################
#######################################################################################

if [[ ( "$build" -eq 0 && "$all" -eq 0 ) ]]; then
    echo "Done with Clean, Build not requested."
    exit
fi

if [[ ( "$all" -eq 1 || "$build" -eq 1 ) ]]; then
    echo "
    ############################################
    #Conditional Compile -a default or -b build#
    ############################################"

    echo "
    ##################################
    ###        INSTALLATION        ###
    ##################################"

    # PreReqs
    echo "*****************************"
    echo "*	       Prerequisits	      *"
    echo "*****************************"
    
    if [ $debug -eq 0 ]; then

        if [[ "$OSTYPE" == "linux-gnu" ]]; then
            echo "apt-get autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"

            sudo apt-get -y install autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev

        elif [[ "$OSTYPE" == "darwin"* ]]; then
            brew install autoconf
            brew install automake
            brew install libtool
            brew install liboauth
            brew install sqlite3
            brew install libpq
            brew install libconfig
            brew install gnutls
            brew install libmicrohttpd
            echo "brew install autoconf automake libtool libmicrohttpd-dev sqlite3 libsqlite3-dev default-libmysqlclient-dev libpq-dev libgnutls-dev libconfig-dev libssl-dev libldap2-dev liboath-dev"
        fi
    fi

    # liblfds
    #echo "*****************************"
    #echo "*       Setup liblfds 	  *"
    #echo "*****************************"
    #if [ ! -d liblfds ]; then
    ##	echo "*****************************"
    #	echo "Dowload liblfds"
    #	echo "*****************************"
    #	git clone https://github.com/liblfds/liblfds.git
    #fi

    # Install liblfds 
    #echo "*****************************"
    #echo "Install liblfds"
    #echo "*****************************"
    ###if [ -d liblfds ]; then
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
    log "Current Dir: ${PWD}"
    echo "*****************************"

    if [ ! -d json-c ]; then
        echo "*****************************"
        echo "json-c installation required..."
        echo "*****************************"
        download_git_repo JSON-C
            
        if [ $debug -eq 0 ]; then
            cd json-c
            log "Current Dir: ${PWD}"
            sh autogen.sh
            echo "*****************************"
            echo "json-c building..."
            echo "*****************************"
            ./configure 
            make
            log " ** make install **"
            sudo make install
            cd ..
        else
            log "Debug Mode - Not Installing."
        fi
        echo "*****************************"
        echo "json-c build completed."
        echo "*****************************"
    else
        echo "*****************************"
        echo "json-c alreadey installed."
        echo "*****************************"
    fi
    log "Current Dir: ${PWD}"

    #openssl
    echo "*****************************"
    echo "Install openssl"
    echo "*****************************"
    if [ $debug -eq 0 ]; then
        if [ ! -d openssl ]; then
            echo "clone openssl required..."
            download_git_repo openssl
        fi

        echo "*****************************"
        echo "openssl installation required..."
        echo "*****************************"
        cd openssl 
        echo "*****************************"
        echo "openssl building..."
        log "* Current Dir: ${PWD}"
        echo "*****************************"
       ## ./config 
       ## make
        echo "*** make install_sw no man *** "
       ## sudo make install_sw
        cd ..
    else
        log "Debug Mode - Not Installing."
    fi
    echo "******************s***********"
    echo "openssl build completed."
    echo "*****************************"
    log "Current Dir: ${PWD}"

    # libjwt 
    echo "*****************************"
    echo "Install libjwt"
    echo "*****************************"
        
    download_git_repo libjwt
    
    echo "*****************************"
    echo "libjwt building..."
    echo "*****************************"

    if [ -d libjwt ]; then
        if [ $debug -eq 0 ]; then
            cd libjwt 
            log "Current Dir: ${PWD}"
            autoreconf -i
            ./configure OPENSSL_CFLAGS=" " OPENSSL_LIBS="-lssl -lcrypto"
            make all
            log " ** make install **"
            sudo make install
            cd ..
        else
            log "Debug Mode - Not Installing."
        fi
        
        echo "*****************************"
        echo "libjwt build completed."
        echo "*****************************"
    else
        echo "*****************************"
        echo "libjwt installation error!"
        echo "*****************************"
    fi

    log "Current Dir: ${PWD}"

    # Install Orcania
    echo "*****************************"
    echo "Install Orcania C Utilities"
    echo "Current Dir: ${PWD}"
    echo "*****************************"
    
    download_git_repo orcania
    
    if [ -d orcania ]; then
        echo "*****************************"
        echo "Orcania building..."
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            cd orcania
           
            echo "Current Dir: ${PWD}"
            echo " ** cmake/make install **"

            if [ ! -d build ]; then
                mkdir build
            fi
            cd build
            rm CMakeCache.txt
            if [[ "$OSTYPE" == "darwin"* ]]; then
                cmake -DWITH_JOURNALD=off ..
            else
                echo "Current Dir: ${PWD}"
                cmake ..
            fi
            make
            sudo make install
            cd ../..
        else
            echo "Debug Mode - Not Installing."
        fi
          
        echo "*****************************"
        echo "Orcania build completed."
        echo "*****************************"
    fi
        
    echo "Current Dir: ${PWD}"
    
    download_git_repo yder

    # Install Yder for logging
    echo "*****************************"
    echo "Install Yder for Logging"
    echo "Current Dir: ${PWD}"
    echo "*****************************"
    if [ -d yder ]; then
        echo "*****************************"
        echo "Yder building..."
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            cd yder
            echo "Current Dir: ${PWD}"
            if [ ! -d build ]; then
                mkdir build
            fi
            cd build
            rm CMakeCache.txt

            if [[ "$OSTYPE" == "linux-gnu" ]]; then
                echo " ** make install linux**"
                cmake ..
            elif [[ "$OSTYPE" == "darwin"* ]]; then
                echo " ** make install Disable Journal on MacOS **"
                #make Y_DISABLE_JOURNALD=1
                cmake -DWITH_JOURNALD=off ..
            fi
            make
            echo " ** make install **"
            sudo make install
            ##  Mac Error: /bin/sh: -c: line 0: `ifeq (Darwin,Darwin)'  Fix this
            cd ../..
        else
            echo "Debug Mode - Not Installing."
        fi
        echo "*****************************"
        echo "Yder build completed."
        echo "*****************************"
    fi
    echo "Current Dir: ${PWD}"

    # Install Ulfius
    echo "*****************************"
    echo "Install Ulfius"
    echo "*****************************"
    
    download_git_repo ulfius

    if [ -d ulfius ]; then
        echo "*****************************"
        echo "Ulfius building..."
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            cd ulfius
            log "Current Dir: ${PWD}"

            if [ ! -d build ]; then
                mkdir build
                log "Current Dir: ${PWD}"
            fi
            
            cd build
            log "Current Dir: ${PWD}"

            if [[ "$OSTYPE" == "darwin"* ]]; then
                rm CMakeCache.txt
                cmake -DWITH_JOURNALD=off ..
            else
                cmake ..
            fi

            # Fix Darwin error: incompatible pointer types passing 'int (void *, enum MHD_ValueKind, const char *, const char *)' to parameter of type 'MHD_KeyValueIterator'
            make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 UWSCFLAG=1 && sudo make install

            cd ../..
        else
            log "Debug Mode - Not Installing."
        fi
        
        echo "*****************************"
        echo "Ulfius build completed."
        echo "*****************************"
    fi
    log "Current Dir: ${PWD}"

    # Install Hoel
    echo "*****************************"
    echo "Install Hoel - Database for sqlite3"
    echo "*****************************"

    download_git_repo hoel

    if [ -d hoel ]; then
        echo "*****************************"
        echo "Hoel building..."
        echo "*****************************"

        if [ $debug -eq 0 ]; then
            cd hoel
            log "Current Dir: ${PWD}"

            if [ ! -d build ]; then
                log "mkdir build"
                mkdir build
            fi
            log "cd  build"
            cd build

            log "cmake hoel without MariaDB or PGSQL"
            if [[ "$OSTYPE" == "linux-gnu" ]]; then
                echo "Linux build enable JournalD"
                cmake -DWITH_MARIADB=off -DWITH_PGSQL=off ..
            elif [[ "$OSTYPE" == "darwin"* ]]; then
                echo "Darwin Build disable JournalD"
                cmake -DWITH_SQLITE3=on -DWITH_MARIADB=off -DWITH_PGSQL=off -DWITH_JOURNALD=off ..
            fi

            log $OSTYPE
            
            make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 DISABLE_JOURNALD=1 
            sudo make install
            cd ../..
        else
            log "Debug Mode - Not Installing."
        fi
        echo "*****************************"
        echo "Hoel build completed."
        echo "*****************************"
    fi
    log "Current Dir: ${PWD}"

    #Install libcbor
    echo "*****************************"
    echo "Install LibCBOR"
    echo "*****************************"

    if [ ! -d libcbor ]; then
        echo "*****************************"
        echo "Git clone LibCBOR."
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            download_git_repo libcbor
            #git clone https://github.com/PJK/libcbor    
        fi
        if [ -d "libcbor" ]; then
            echo "*****************************"
            echo "LibCBOR Building..."
            echo "*****************************"

            cd libcbor
            if [ ! -d "build" ]; then
                mkdir build
            fi
            if [ debug -eq 0 ]; then
                cd build
                log "Current Dir: ${PWD}"
                cmake -DCMAKE_BUILD_TYPE=Release -DCBOR_CUSTOM_ALLOC=ON ..
                make
                make install
            else
                log "Debug Mode - Not Installing LibCbor."
            fi
            cd ..
        fi
    fi
    log "Current Dir: ${PWD}"

    #Install Glewlwyd
    echo "*****************************"
    echo "Install Glewlwyd"
    echo "Current Dir: ${PWD}"
    echo "*****************************"

    if [ ! -d glewlwyd ]; then
        echo "*****************************"
        echo "Git clone Glewlwyd."
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            download_git_repo glewlwyd
        fi
    fi
    echo "*****************************"
    echo "Glewlwyd Building..."
    echo "*****************************"

    if [ $debug -eq 0 ]; then
        echo "Tools dir: cd glewlwyd: ${PWD}"
        echo "cd glewlwyd"
        cd glewlwyd
        echo "in glewlwyd: ${PWD}"

        if [ ! -d build ]; then
            export  LIBOATH_LIBRARY="/usr/local/lib/liboauth.dylib"
            mkdir build
            echo "mkdir build"
            echo "curr dir glewlwyd still: ${PWD}"
        fi

        cd build
        echo "cd build: ${PWD}"
        cmake -DWITH_JOURNALD=off -DWITH_MARIADB=off -DWITH_PGSQL=off ..
        echo "make in dir: ${PWD}"
        make DISABLE_MARIADB=1 DISABLE_POSTGRESQL=1 DISABLE_JOURNALD=1
        sudo make install
        cd ../..
    else
        echo "Debug Mode - Not Building."
    fi
    echo "*****************************"
    echo "Done Buidling Glewlwyd."
    echo "*****************************"


    #Install c-libp2p
    echo "*****************************"
    echo "Install c-libp2p"
    echo "Current Dir: ${PWD}"
    echo "*****************************"

    if [ ! -d c-libp2p ]; then
        echo "*****************************"
        echo "Git clone c-libp2p."
        echo "*****************************"
        if [ $debug -eq 0 ]; then
            download_git_repo c-libp2p
           #git clone https://github.com/Agorise/c-libp2p.git 
        fi
    fi

fi
