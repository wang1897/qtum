#include <vector>
#include "qtumtransaction.h"

static const uint8_t byteCodePre[]={'b','y','t','e','c','o','d','e','_'};

bool DeltaDB:: writeByteCode(UniversalAddress address, valtype byteCode){
  if(address.version == AddressVersion::X86){
	std::vector<uint8_t> K(byteCodePre, byteCodePre + sizeof(byteCodePre)/sizeof(uint8_t));  
	K.insert(K.end(), address.data.begin(), address.data.end());  
	return Write(K, byteCode);;	
  }
  return false;
}

bool DeltaDB:: readByteCode(UniversalAddress address, valtype& byteCode){
  if(address.version == AddressVersion::X86){
    std::vector<uint8_t> K(byteCodePre, byteCodePre + sizeof(byteCodePre)/sizeof(uint8_t));	
    K.insert(K.end(), address.data.begin(), address.data.end());
    return Read(K, byteCode);   
  }
  return false;
}



