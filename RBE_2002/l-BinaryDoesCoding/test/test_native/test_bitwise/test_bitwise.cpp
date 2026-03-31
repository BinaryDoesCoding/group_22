#include "unity.h"
#include <stdint.h>

#ifndef TCCR4A
volatile uint8_t TCCR4A = 0;
#endif

unsigned char SetBit(unsigned char byte, unsigned char bit)
{
    return byte | (1 << bit);
}

unsigned char ClearBit(unsigned char byte, unsigned char bit)
{
    return byte & ~(1 << bit);
}

unsigned int Concatenate(unsigned char highbyte, unsigned char lowbyte)
{
    return ((unsigned int)highbyte << 8) | lowbyte;
}

void setUp(void)
{
    TCCR4A |= (1 << 6);
}

void tearDown(void)
{
    TCCR4A &= ~(1 << 6);
}

unsigned char Xor(unsigned char byte1, unsigned char byte2)
{
    return byte1 ^ byte2;
}

void test_bitset(void)
{
    TEST_ASSERT_EQUAL(0xC4, SetBit(0x44, 7));
    TEST_ASSERT_EQUAL(0xE7, SetBit(0xA7, 6));
}

void test_bitclear(void)
{
    TEST_ASSERT_EQUAL(0x04, ClearBit(0x44, 6));
    TEST_ASSERT_EQUAL(0xF1, ClearBit(0xF3, 1));
}

void test_concatenate(void)
{
    TEST_ASSERT_EQUAL(0x3357, Concatenate(0x33, 0x57));
    TEST_ASSERT_EQUAL(0xA514, Concatenate(0xA5, 0x14));
}

void test_xor(void)
{
    TEST_ASSERT_EQUAL(0x00, Xor(0xAA, 0xAA));
    TEST_ASSERT_EQUAL(0xFF, Xor(0xAA, 0x55));
}

void test_setup_sets_bit_6(void)
{
    TCCR4A = 0x00;
    setUp();
    TEST_ASSERT_BITS_HIGH(1 << 6, TCCR4A);
}

void test_teardown_clears_bit_6(void)
{
    TCCR4A = 0xFF;
    tearDown();
    TEST_ASSERT_BITS_LOW(1 << 6, TCCR4A);
}

int runUnityTests(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_bitset);
    RUN_TEST(test_bitclear);
    RUN_TEST(test_concatenate);
    RUN_TEST(test_xor);
    RUN_TEST(test_setup_sets_bit_6);
    RUN_TEST(test_teardown_clears_bit_6);

    return UNITY_END();
}

int main(void)
{
    return runUnityTests();
}
