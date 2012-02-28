#ifndef __BLOOM_FILTER_TEST__
#define __BLOOM_FILTER_TEST__

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class BloomFilterTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( BloomFilterTest );
	CPPUNIT_TEST( Test );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp()
	{
	}

	void tearDown()
	{
	}

	void Test();

};


#endif

