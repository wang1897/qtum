#ifndef QTUMTRANSACTION_H
#define QTUMTRANSACTION_H

#include <libethcore/Transaction.h>
#include <libethereum/Transaction.h>
#include <dbwrapper.h>
#include <util.h>
#include <script/interpreter.h>


struct VersionVM{
    //this should be portable, see https://stackoverflow.com/questions/31726191/is-there-a-portable-alternative-to-c-bitfields
# if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t format : 2;
    uint8_t rootVM : 6;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint8_t rootVM : 6;
    uint8_t format : 2;
#endif
    uint8_t vmVersion;
    uint16_t flagOptions;
    // CONSENSUS CRITICAL!
    // Do not add any other fields to this struct

    uint32_t toRaw(){
        return *(uint32_t*)this;
    }
    static VersionVM fromRaw(uint32_t val){
        VersionVM x = *(VersionVM*)&val;
        return x;
    }
    static VersionVM GetNoExec(){
        VersionVM x;
        x.flagOptions=0;
        x.rootVM=0;
        x.format=0;
        x.vmVersion=0;
        return x;
    }
    static VersionVM GetEVMDefault(){
        VersionVM x;
        x.flagOptions=0;
        x.rootVM=1;
        x.format=0;
        x.vmVersion=0;
        return x;
    }
}__attribute__((__packed__));

enum AddressVersion{
    UNKNOWN = 0,
    //legacy is either pubkeyhash or EVM, depending on if the address already exists
    LEGACYEVM = 1,
    PUBKEYHASH = 2,
    EVM = 3,
    X86 = 4,
    SCRIPTHASH = 5,
};


struct UniversalAddress{
    UniversalAddress(){
        version = AddressVersion::UNKNOWN;
    }
    UniversalAddress(AddressVersion v, const std::vector<uint8_t> &d)
    : version(v), data(d) {}
    UniversalAddress(AddressVersion v, const unsigned char* begin, const unsigned char* end)
    : version(v), data(begin, end) {}
    AddressVersion version;
    std::vector<uint8_t> data;

    bool operator<(const UniversalAddress& a) const{
        return data < a.data;
    }
    bool operator==(const UniversalAddress& a) const{
        return version == a.version && data == a.data;
    }
    bool operator!=(const UniversalAddress& a) const{
        return !(a == *this);
    }

    static UniversalAddress FromScript(const CScript& script);
    static UniversalAddress FromOutput(AddressVersion v, uint256 txid, uint32_t vout);
};


class DeltaDB : public CDBWrapper
{

public:
	DeltaDB(size_t nCacheSize, bool fMemory, bool fWipe) : CDBWrapper(GetDataDir() / "DeltaDB", nCacheSize, fMemory, fWipe) { }
    bool writeState(valtype address, valtype key, valtype value);
    bool readState(valtype address, valtype key, valtype& value);
	bool writeByteCode(UniversalAddress address,valtype byteCode);
	bool readByteCode(UniversalAddress address,valtype& byteCode);


};

class QtumTransaction : public dev::eth::Transaction{

public:

    QtumTransaction() : nVout(0) {}

    QtumTransaction(dev::u256 const& _value, dev::u256 const& _gasPrice, dev::u256 const& _gas, dev::bytes const& _data, dev::u256 const& _nonce = dev::Invalid256):
		dev::eth::Transaction(_value, _gasPrice, _gas, _data, _nonce) {}

    QtumTransaction(dev::u256 const& _value, dev::u256 const& _gasPrice, dev::u256 const& _gas, dev::Address const& _dest, dev::bytes const& _data, dev::u256 const& _nonce = dev::Invalid256):
		dev::eth::Transaction(_value, _gasPrice, _gas, _dest, _data, _nonce) {}

    void setHashWith(const dev::h256 hash) { m_hashWith = hash; }

    dev::h256 getHashWith() const { return m_hashWith; }

    void setNVout(uint32_t vout) { nVout = vout; }

    uint32_t getNVout() const { return nVout; }

    void setVersion(VersionVM v){
        version=v;
    }
    VersionVM getVersion() const{
        return version;
    }
private:

    uint32_t nVout;
    VersionVM version;

};


#endif
