#include <vector>
#include "qtumtransaction.h"
#include <serialize.h>
#include <streams.h>
#include <uint256.h>

static const uint8_t byteCodePre[]={'b','y','t','e','c','o','d','e','_'};
static const uint8_t dataPre[]={'d','a','t','a','_'};

/* Bytecode of contract: KEY is derived from %bytecode%_%address% */
bool DeltaDB:: writeByteCode(UniversalAddress address, valtype byteCode){
	std::vector<uint8_t> K(byteCodePre, byteCodePre + sizeof(byteCodePre)/sizeof(uint8_t));
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());
	return Write(K, byteCode);
}

bool DeltaDB:: readByteCode(UniversalAddress address, valtype& byteCode){
    std::vector<uint8_t> K(byteCodePre, byteCodePre + sizeof(byteCodePre)/sizeof(uint8_t));	
	K.insert(K.end(), address.version);
    K.insert(K.end(), address.data.begin(), address.data.end());
    return Read(K, byteCode);   
}

/* Live data of contract: KEY is derived from %address%_data_%key% */
bool DeltaDB:: writeState(UniversalAddress address, valtype key, valtype value){
	std::vector<uint8_t> K;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), dataPre, dataPre + sizeof(dataPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	return Write(K, value);
}

bool DeltaDB:: readState(UniversalAddress address, valtype key, valtype& value){
	std::vector<uint8_t> K;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), dataPre, dataPre + sizeof(dataPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	return Read(K, value);
}



