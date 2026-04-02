#include "unity.h"
#include "line_sensor.h"

LineSensor lineSensor(60, 100);

void setUp(void) 
{  
  // set stuff up here
}

void tearDown(void) 
{
  // clean stuff up here
}

void test_intersection(void) 
{
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(40, 40));
  TEST_ASSERT_TRUE(lineSensor.CheckIntersection(120, 120));
}

void test_event_intersection(void) 
{
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(40, 40));
  TEST_ASSERT_TRUE(lineSensor.CheckIntersection(120, 120));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(120, 120));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(40, 40));
}

void test_event_intersection_hysteresis(void)
{
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(40, 40));
  TEST_ASSERT_TRUE(lineSensor.CheckIntersection(100, 100));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(99, 99));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(100, 100));
}

void test_event_intersection_differential(void)
{
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(40, 40));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(40, 100));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(100, 40));
  TEST_ASSERT_TRUE(lineSensor.CheckIntersection(100, 100));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(99, 100));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(100, 100));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(40, 100));
  TEST_ASSERT_FALSE(lineSensor.CheckIntersection(100, 40));
  TEST_ASSERT_TRUE(lineSensor.CheckIntersection(100, 100));
}

int runUnityTests(void) 
{
  UNITY_BEGIN();

  RUN_TEST(test_intersection);
  RUN_TEST(test_event_intersection);
  RUN_TEST(test_event_intersection_hysteresis);
  RUN_TEST(test_event_intersection_differential);

  return UNITY_END();
}

int main(void)
{
  runUnityTests();
}
