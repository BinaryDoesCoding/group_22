#include "unity.h"
#include "filter.h"

/**
 * WARNING: You must hard-code a buffer of 9 elements for this test to work!!!
 */

MovingAverageFilter filter;

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
  TEST_ASSERT_EQUAL_FLOAT(5, filter.CalcFiltered(45));
  TEST_ASSERT_EQUAL_FLOAT(9.555555, filter.CalcFiltered(41));
  TEST_ASSERT_EQUAL_FLOAT(14.222222, filter.CalcFiltered(42));
  TEST_ASSERT_EQUAL_FLOAT(19, filter.CalcFiltered(43));
  TEST_ASSERT_EQUAL_FLOAT(23.888888, filter.CalcFiltered(44));
  TEST_ASSERT_EQUAL_FLOAT(28.222222, filter.CalcFiltered(39));
  TEST_ASSERT_EQUAL_FLOAT(32.444444, filter.CalcFiltered(38));
  TEST_ASSERT_EQUAL_FLOAT(36.555555, filter.CalcFiltered(37));
  TEST_ASSERT_EQUAL_FLOAT(41.111111, filter.CalcFiltered(41));
  TEST_ASSERT_EQUAL_FLOAT(40.777777, filter.CalcFiltered(42));
  TEST_ASSERT_EQUAL_FLOAT(41, filter.CalcFiltered(43));
  TEST_ASSERT_EQUAL_FLOAT(40.222222, filter.CalcFiltered(35));
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
