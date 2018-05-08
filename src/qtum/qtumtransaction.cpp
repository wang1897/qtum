#include <vector>
#include "qtumtransaction.h"
#include <streams.h>
#include <serialize.h>
#include <uint256.h>

static const uint8_t byteCodePre[]={'b','y','t','e','c','o','d','e','_'};
static const uint8_t dataPre[]={'d','a','t','a','_'};
static const uint8_t updatePre[]={'u','p','d','a','t','e','d','_'};

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

/* Live data of contract state: KEY is derived from %address%_data_%key% */
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

/* Live data of contract updated:  %address%_updated_%key%_ */
bool DeltaDB:: writeUpdatedKey(UniversalAddress address, valtype key, unsigned int blk_num, uint256 blk_hash){
	std::vector<uint8_t> K;
	std::vector<uint8_t> V;
	CDataStream dsValue(SER_DISK,0);
	dsValue<<blk_num;
	dsValue<<blk_hash;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), updatePre, updatePre + sizeof(updatePre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	V.insert(V.end(),dsValue.begin(),dsValue.end());
	return Write(K, V);

}

bool DeltaDB:: readUpdatedKey(UniversalAddress address, valtype key, unsigned int &blk_num, uint256 &blk_hash){
	std::vector<uint8_t> K;
	std::vector<uint8_t> V;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), updatePre, updatePre + sizeof(updatePre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	if(Read(K, V)){
		CDataStream dsValue(V,SER_DISK,0);
		dsValue>>blk_num;
		dsValue>>blk_hash;
		return true;
	}else{
        return false;
	}
}


