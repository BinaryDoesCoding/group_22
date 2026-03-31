#include "unity.h"
#include "filter.h"

/**
 * WARNING: floats are hard to test (they can be off by a bit or two), so check any errors
 * carefully to see if it's just a rounding error.
 */

LowPassFilter filter(0.1);

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
  TEST_ASSERT_EQUAL_FLOAT(4.5, filter.CalcFiltered(45));
  TEST_ASSERT_EQUAL_FLOAT(8.15, filter.CalcFiltered(41));
  TEST_ASSERT_EQUAL_FLOAT(11.535, filter.CalcFiltered(42));
  TEST_ASSERT_EQUAL_FLOAT(14.6815, filter.CalcFiltered(43));
  TEST_ASSERT_EQUAL_FLOAT(17.61335, filter.CalcFiltered(44));
  TEST_ASSERT_EQUAL_FLOAT(19.75201, filter.CalcFiltered(39));
  TEST_ASSERT_EQUAL_FLOAT(21.57681, filter.CalcFiltered(38));
  TEST_ASSERT_EQUAL_FLOAT(23.11913, filter.CalcFiltered(37));
  TEST_ASSERT_EQUAL_FLOAT(24.90722, filter.CalcFiltered(41));
  TEST_ASSERT_EQUAL_FLOAT(26.61649, filter.CalcFiltered(42));
  TEST_ASSERT_EQUAL_FLOAT(28.25485, filter.CalcFiltered(43));
  TEST_ASSERT_EQUAL_FLOAT(28.92936, filter.CalcFiltered(35));
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
