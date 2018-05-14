#include <vector>
#include "qtumtransaction.h"
#include <streams.h>
#include <serialize.h>
#include <uint256.h>

static const uint8_t byteCodePre[]={'b','y','t','e','c','o','d','e','_'};
static const uint8_t dataPre[]={'d','a','t','a','_'};
static const uint8_t updatePre[]={'u','p','d','a','t','e','d','_'};
static const uint8_t keysPre[]={'k','e','y','s','_'};
static const uint8_t iteratorPre[]={'i','t','e','r','t', 'o', 'r','_'};
static const uint8_t infoPre[]={'i','n','f','o','_'};
static const uint8_t oldPre[]={'o','l','d','_'};

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
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), updatePre, updatePre + sizeof(updatePre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	
	CDataStream dsValue(SER_DISK,0);
	dsValue<<blk_num;
	dsValue<<blk_hash;
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

/* the raw unhashed key to be looked up by hash: %address%_keys_%key% */
bool DeltaDB:: writeRawKey(UniversalAddress address,	valtype key, valtype rawkey){
	std::vector<uint8_t> K;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), keysPre, keysPre + sizeof(keysPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	return Write(K, rawkey);
}

bool DeltaDB:: readRawKey(UniversalAddress address, valtype key, valtype &rawkey){
	std::vector<uint8_t> K;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), keysPre, keysPre + sizeof(keysPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	return Read(K, rawkey);
}

/* current iterator of a key: %address%_iterator_%key% */
bool DeltaDB:: writeCurrentIterator(UniversalAddress address, valtype key, uint64_t iterator){
	std::vector<uint8_t> K;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), iteratorPre, iteratorPre + sizeof(iteratorPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	return Write(K, iterator);
}

bool DeltaDB:: readCurrentIterator(UniversalAddress address,		valtype key, uint64_t &iterator){
	std::vector<uint8_t> K;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), iteratorPre, iteratorPre + sizeof(iteratorPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	return Read(K, iterator);
}

/*  key's data at point of iterator: %address%_data_%key%_%iterator% */
bool DeltaDB:: writeStateWithIterator(UniversalAddress address,		 valtype key, uint64_t iterator, valtype value){
	std::vector<uint8_t> K;
	CDataStream dsKey(SER_DISK,0);
	dsKey<<iterator;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), dataPre, dataPre + sizeof(dataPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());
	K.insert(K.end(), dsKey.begin(),dsKey.end());	
	return Write(K, value);
}


bool DeltaDB:: readStateWithIterator(UniversalAddress address,		valtype key, uint64_t iterator, valtype &value){
	std::vector<uint8_t> K;
	CDataStream dsKey(SER_DISK,0);
	dsKey<<iterator;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), dataPre, dataPre + sizeof(dataPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	K.insert(K.end(), dsKey.begin(),dsKey.end());	
	return Read(K, value);
}

/* block number + block hash + txid/vout at point of iterator: %address%_info_%key%_%iterator%  */
bool DeltaDB:: writeInfoWithIterator(UniversalAddress address,		valtype key, uint64_t iterator, unsigned int blk_num, uint256 blk_hash, uint256 txid, unsigned int vout){
	std::vector<uint8_t> K;
	std::vector<uint8_t> V;
	CDataStream dsKey(SER_DISK,0);
	dsKey<<iterator;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), infoPre, infoPre + sizeof(infoPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	
	K.insert(K.end(), dsKey.begin(),dsKey.end());

	CDataStream dsValue(SER_DISK,0);
	dsValue<<blk_num;	
	dsValue<<blk_hash;	
	dsValue<<txid;	
	dsValue<<vout;	
	V.insert(V.end(),dsValue.begin(),dsValue.end());
	return Write(K, V);
}

bool DeltaDB:: readInfoWithIterator(UniversalAddress address, 	   valtype key, uint64_t iterator, unsigned int &blk_num, uint256 &blk_hash, uint256 &txid, unsigned int &vout){
	std::vector<uint8_t> K;
	std::vector<uint8_t> V;
	CDataStream dsKey(SER_DISK,0);
	dsKey<<iterator;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), infoPre, infoPre + sizeof(infoPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());
	K.insert(K.end(), dsKey.begin(),dsKey.end());
	if(Read(K, V)){
		CDataStream dsValue(V,SER_DISK,0);
		dsValue>>blk_num;
		dsValue>>blk_hash;
		dsValue>>txid;	
		dsValue>>vout;	
		return true;
	}else{
        return false;
	}
}

/* Oldest iterator that exists in the changelog database: %address%_old_%key% */
bool DeltaDB:: writeOldestIterator(UniversalAddress address,		  valtype key, uint64_t iterator, unsigned int blk_num, uint256 blk_hash){
	std::vector<uint8_t> K;
	std::vector<uint8_t> V;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), oldPre, oldPre + sizeof(oldPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());	

	CDataStream dsValue(SER_DISK,0);
	dsValue<<iterator;	
	dsValue<<blk_num;	
	dsValue<<blk_hash;	
	V.insert(V.end(),dsValue.begin(),dsValue.end());
	return Write(K, V);
}

bool DeltaDB:: readOldestIterator(UniversalAddress address,		 valtype key, uint64_t &iterator, unsigned int &blk_num, uint256 &blk_hash){
	std::vector<uint8_t> K;
	std::vector<uint8_t> V;
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());	
	K.insert(K.end(), '_');
	K.insert(K.end(), oldPre, oldPre + sizeof(oldPre)/sizeof(uint8_t));
	K.insert(K.end(), key.begin(),key.end());
	if(Read(K, V)){
		CDataStream dsValue(V,SER_DISK,0);
		dsValue>>iterator;	
		dsValue>>blk_num;
		dsValue>>blk_hash;
		return true;
	}else{
        return false;
	}
}




