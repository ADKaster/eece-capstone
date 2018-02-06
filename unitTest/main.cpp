/*
 * main.cpp
 *
 *  Created on: Jan 30, 2018
 *      Author: Brian
 */

#include "gtest.h"
#include "messaging_unit_test.hpp"

int main(int argc, char **args) {

    ::testing::InitGoogleTest(&argc, args);
    return RUN_ALL_TESTS();
}
