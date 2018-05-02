#include <boost/test/unit_test.hpp>
#include <qtumtests/test_utils.h>
#include <script/standard.h>
#include <qtum/qtumtransaction.h>


namespace deltaDBTest{


BOOST_FIXTURE_TEST_SUITE(deltaDB_tests, TestingSetup)

BOOST_AUTO_TEST_CASE(byteCode_read_write_test){
	size_t nCacheSize=8;
	bool fWipe=false,ret;
	DeltaDB* pDeltaDB= new DeltaDB(nCacheSize,false,fWipe);
	UniversalAddress addr(X86,valtype(ParseHex("0c4c1d7375918557df2ef8f1d1f0b2329cb248a1")));
	valtype byteCode = valtype(ParseHex("bf5f1e83000000000000000000000000c4c1d7375918557df2ef8f1d1f0b2329cb248a150000000000000000000000000000000000000000000000000000000000000002"));
	valtype byteCode2;
	
	ret = pDeltaDB->writeByteCode(addr, byteCode);
	BOOST_CHECK(ret);
	ret = pDeltaDB->readByteCode(addr, byteCode2);	
    BOOST_CHECK(byteCode2==byteCode);
	
    delete pDeltaDB;	
}


BOOST_AUTO_TEST_SUITE_END()


}

