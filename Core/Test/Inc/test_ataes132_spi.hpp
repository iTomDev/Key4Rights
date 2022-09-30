/*
 * test_ataes132_spi.hpp
 *
 *  Created on: Sep 22, 2022
 *      Author: Tom
 */

#ifndef TEST_INC_TEST_ATAES132_SPI_HPP_
#define TEST_INC_TEST_ATAES132_SPI_HPP_

#include "../Inc/ataes132_spi.hpp"

class ATAES132_SPI_Test {
protected:
	ATAES132_SPI test_at132;
public:
	int8_t setUp();
	void tearDown();
};


#endif /* TEST_INC_TEST_ATAES132_SPI_HPP_ */
