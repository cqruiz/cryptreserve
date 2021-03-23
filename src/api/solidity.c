#include "../../include/solidity.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

FILE *fpUBI;

//Solidity Compiler
void BuildContract(FILE *pFile)
{
    fpUBI=pFile;
    char buf[1024];
    char *args[64];

    // Split the string into arguments.
    parse(buf, args);
    execute(args);
}

/*
 * parse--split the command in buf into
 *         individual arguments.
 */
void parse(char *buf,char **args)
{
    while (*buf != NULL) {
        /*
         * Strip whitespace.  Use nulls, so
         * that the previous argument is terminated
         * automatically.
         */
        while ((*buf == ' ') || (*buf == '\t'))
            *buf++ = NULL;

        /*
         * Save the argument.
         */
        *args++ = buf;

        /*
         * Skip over the argument.
         */
        while ((*buf != NULL) && (*buf != ' ') && (*buf != '\t'))
            buf++;
    }

    *args = NULL;
}

// execute--spawn a child process and execute the program
void execute(char **args)
{
    int pid, status;

    /*
     * Get a child process.
     */
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);

	/* NOTE: perror() produces a short  error  message  on  the  standard
           error describing the last error encountered during a call to
           a system or library function.
       */
    }

    /*
     * The child executes the code inside the if.
     */
    if (pid == 0) {
        //execvp(`solc --bin --abi --optimize -o bin contract.sol`);
        execvp(*args, args);
        perror(*args);
        exit(1);

       /* NOTE: The execv() vnd execvp versions of execl() are useful when the
          number  of  arguments is unknown in advance;
          The arguments to execv() and execvp()  are the name
          of the file to be executed and a vector of strings  contain-
          ing  the  arguments.   The last argument string must be fol-
          lowed by a 0 pointer. 

          execlp() and execvp() are called with the same arguments  as
          execl()  and  execv(),  but duplicate the shell's actions in
          searching for an executable file in a list  of  directories.
          The directory list is obtained from the environment.
        */
    }

    /*
     * The parent executes the wait.
     */
    while (wait(&status) != pid)
        /* empty */ ;

/*    char contract;
    char *abi = fs.readFileSync('bin/contract.sol:Contract.abi');
    char* compiled = '0x' + fs.readFileSync("bin/Contract.bin");
*/
    //char storageOutput[256]=`solc --optimize --combined-json abi,bin,interface Storage.sol`" > storage.js

}


void CompileContract(){

    //exec solc > fprintf(fpUBI,ubi)
    //solc -o output --bin --ast --asm test.sol
    exec();

/*
exec(`solc --bin --abi --optimize -o bin contract.sol`);
char contract
byte *abi = fs.readFileSync('bin/contract.sol:Contract.abi');
var compiled = '0x' + fs.readFileSync("bin/Contract.bin");

char storageOutput[256]=`solc --optimize --combined-json abi,bin,interface Storage.sol`" > storage.js

*/


}

//Deploy byte code to Network
void DeployContract(enum Network net){

    switch(net)
    {
        case Robstein:
            break;

        case TestNet:
            break;

        case MainNet:
            break;
    }
}