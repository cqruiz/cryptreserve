#ifndef SOLIDITY_HEADER
#define SOLIDITY_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <mba/hashmap.h>
#include <jansson.h>

struct SolidityFile {
    char* content;
};
typedef struct SolidityFile* SolidityFilePtr;

struct SolidityArtifact {
    char* contract_name;
    char* file_name;
    char* source_path;
    char* source;
    char* bytecode;
    char* deployed_bytecode;
    char* source_map;
    char* deployed_source_map;
    json_t *abi;
    json_t *ast;
};
typedef struct SolidityArtifact* SolidityArtPtr;

struct SolcBytecodeOutput {
    char* object;
    char* source_map;
    // link_references,
    // opcodes,
};
typedef struct SolcBytecodeOutput* SolcBytecodeOutPtr;

struct SolcSource {
    json_t* ast;
};
typedef struct SolcSource* SolcSrcPtr;

struct SolcContractEvm {
    SolcSrcPtr bytecode;
    SolcSrcPtr deployed_bytecode;
};
typedef struct SolcContractEvm* SolcContractEvmPtr;

struct SolcContract {
    SolcContractEvmPtr evm;
    json_t* abi;
};
typedef struct SolcContract* SolcContractPtr;


struct SolcError {
    char * severity;
    char * formatted_message;
};
typedef struct SolcError* SolcErrPtr;

struct SolcOutput {
    hashmapPtr contracts;
    hashmapPtr sources;
    SolcErrPtr errors;
};
typedef struct SolcOutput* SolcOutPtr;

extern FILE *fpUBI;
extern enum Network {Robstein, MainNet, TestNet};
void CompileContract();
void DeployContract();
#endif
