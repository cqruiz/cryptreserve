#ifndef SMARTCONTRACT_HEADER
#define SMARTCONTRACT_HEADER

#include <stdlib.h>
#include <stdio.h>


extern FILE *fpUBI;
extern enum Network {Robstein, MainNet, TestNet};
void CompileContract();
void DeployContract();
#endif