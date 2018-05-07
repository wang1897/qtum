#include <vector>
#include "qtumtransaction.h"

static const uint8_t byteCodePre[]={'b','y','t','e','c','o','d','e','_'};

bool DeltaDB:: writeByteCode(UniversalAddress address, valtype byteCode){
	std::vector<uint8_t> K(byteCodePre, byteCodePre + sizeof(byteCodePre)/sizeof(uint8_t));
	K.insert(K.end(), address.version);
	K.insert(K.end(), address.data.begin(), address.data.end());
	return Write(K, byteCode);;	
}

bool DeltaDB:: readByteCode(UniversalAddress address, valtype& byteCode){
    std::vector<uint8_t> K(byteCodePre, byteCodePre + sizeof(byteCodePre)/sizeof(uint8_t));	
	K.insert(K.end(), address.version);
    K.insert(K.end(), address.data.begin(), address.data.end());
    return Read(K, byteCode);   
}



