/*
 * messaging.cpp
 *
 *  Created on: Jan 30, 2018
 *      Author: Brian
 */
#include "messaging.hpp"

TEST(CheckSumUnitTest, ZeroCase)
{
    EXPECT_EQ(0xffffffff, dmcf_calculate_checksum(NULL, 0));
}
