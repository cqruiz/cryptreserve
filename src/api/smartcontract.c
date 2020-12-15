#include "../../include/smartcontract.h"

extern FILE *fpUBI;

//Solidity Compiler
void CompileContract(){

    //exec solc > fprintf(fpUBI,ubi)
    //solc -o output --bin --ast --asm test.sol

/*
exec(`solc --bin --abi --optimize -o bin contract.sol`);
char contract
byte *abi = fs.readFileSync('bin/contract.sol:Contract.abi');
var compiled = '0x' + fs.readFileSync("bin/Contract.bin");

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