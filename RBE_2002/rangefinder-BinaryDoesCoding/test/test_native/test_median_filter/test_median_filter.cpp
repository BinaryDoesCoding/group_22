#include "unity.h"
#include "filter.h"

/**
 * WARNING: You must hard-code a buffer of 9 elements for this test to work!!!
 */

MedianFilter filter;

void setUp(void) 
{  
  // set stuff up here
}

void tearDown(void) 
{
  // clean stuff up here
}

void test_median_filter(void)
{
  TEST_ASSERT_EQUAL_FLOAT(0, filter.CalcFiltered(40));
  TEST_ASSERT_EQUAL_FLOAT(0, filter.CalcFiltered(41));
  TEST_ASSERT_EQUAL_FLOAT(0, filter.CalcFiltered(42));
  TEST_ASSERT_EQUAL_FLOAT(0, filter.CalcFiltered(43));
  TEST_ASSERT_EQUAL_FLOAT(40, filter.CalcFiltered(44));
  TEST_ASSERT_EQUAL_FLOAT(40, filter.CalcFiltered(39));
  TEST_ASSERT_EQUAL_FLOAT(40, filter.CalcFiltered(38));
  TEST_ASSERT_EQUAL_FLOAT(40, filter.CalcFiltered(37));
  TEST_ASSERT_EQUAL_FLOAT(41, filter.CalcFiltered(41));
  TEST_ASSERT_EQUAL_FLOAT(41, filter.CalcFiltered(42));
  TEST_ASSERT_EQUAL_FLOAT(42, filter.CalcFiltered(43));
  TEST_ASSERT_EQUAL_FLOAT(41, filter.CalcFiltered(35));
}

int runUnityTests(void) 
{
  UNITY_BEGIN();

  RUN_TEST(test_median_filter);

  return UNITY_END();
}

int main() 
{
  runUnityTests();
}
