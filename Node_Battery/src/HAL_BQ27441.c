/* --COPYRIGHT--,BSD
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//****************************************************************************
//
// HAL_BQ27441.c - Hardware abstraction layer for interfacing BQ27441
//
//****************************************************************************

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "HAL_BQ27441.h"
#include "dmcf_debug.h"
#include <ti/drivers/I2C.h>
#include "Board.h"
#include <unistd.h>

I2C_Handle battpack_i2c;

static unsigned char computeCheckSum(unsigned char oldCheckSum, int oldData, int newData);
static int swapMSB_LSB(int data);

/* Configures BQ27441 device properties */
bool BQ27441_initConfig()
{
    I2C_Params masterParams;
    short result = 0;

    I2C_Params_init(&masterParams);
    masterParams.transferMode = I2C_MODE_BLOCKING;
    masterParams.bitRate = I2C_100kHz;
    battpack_i2c = I2C_open(BATTPACK_I2C, &masterParams);

    if(battpack_i2c == NULL)
        return 0;

    dmcf_debugprintf("*************************************");
	dmcf_debugprintf("Initializing BQ27441 Configuration");
	dmcf_debugprintf("*************************************");

	//Default Config, DesignCapacity = 1200mAh, DesignEnergy = 1200mAh*3.7V, Terminate Voltage = 3200mV, Taper Current = 120mA
	char str[64];
	sprintf(str, "DesignCapacity = %dmAh", CONF_DESIGN_CAPACITY);
	dmcf_debugprintf(str);
	sprintf(str, "DesignEnergy = %dmAh * 3.7 = %dJ", CONF_DESIGN_CAPACITY, CONF_DESIGN_ENERGY);
	dmcf_debugprintf(str);
	sprintf(str, "TerminateVoltage = %dmV", CONF_TERMINATE_VOLTAGE);
	dmcf_debugprintf(str);
	sprintf(str, "TaperRate = %dmAh/(0.1*%dmA) = %d", CONF_DESIGN_CAPACITY, CONF_TAPER_CURRENT, CONF_TAPER_RATE);
	dmcf_debugprintf(str);

	if (!BQ27441_read16(FLAGS, &result, 1000))
		return 0;

	/* Check if ITPOR bit is set in FLAGS */
	if (result & 0x0020)
	{
		/* Instructs fuel gauge to enter CONFIG UPDATE mode. */
		if (!BQ27441_control(SET_CFGUPDATE, 1000))
			return 0;

		usleep(400000);

		result = 0;
		/* Check if CFGUPMODE bit is set in FLAGS */
		while(!(result & 0x0010))
		{
			if (!BQ27441_read16(FLAGS, &result, 1000))
			{
			    dmcf_debugprintf("Flags %u", result);
			    return 0;
			}
		}

		/* Enable Block data memory control */
		if (!BQ27441_command(BLOCK_DATA_CONTROL, 0x00, 1000))
			return 0;

		/* Set the data class to be accessed */
		if (!BQ27441_command(DATA_CLASS, 0x52, 1000))
			return 0;

		/* Write the block offset loaction */
		if (!BQ27441_command(DATA_BLOCK, 0x00, 1000))
			return 0;

		usleep(400000);

		char old_chksum = 0;
		char new_chksum = 0;
		char tmp_chksum = 0;
		char chksum = 0;
		do
		{
			/* Read Block Data Checksum */
			if (!BQ27441_readChecksum(&old_chksum, 1000))
				return 0;

			usleep(400000);

			/* Checksum calculation */
			tmp_chksum = old_chksum;

			short old_designCapacity = 0;
			short old_designEnergy = 0;
			short old_terminateVoltage = 0;
			short old_taperRate = 0;

			/* Read old design capacity */
			if (!BQ27441_read16(0x4A, &old_designCapacity, 1000))
				return 0;

			tmp_chksum = computeCheckSum(tmp_chksum, old_designCapacity, CONF_DESIGN_CAPACITY);

			/* Read old design energy */
			if (!BQ27441_read16(0x4C, &old_designEnergy, 1000))
				return 0;

			tmp_chksum = computeCheckSum(tmp_chksum, old_designEnergy, CONF_DESIGN_ENERGY);

			/* Read old terminate voltage */
			if (!BQ27441_read16(0x50, &old_terminateVoltage, 1000))
				return 0;

			tmp_chksum = computeCheckSum(tmp_chksum, old_terminateVoltage, CONF_TERMINATE_VOLTAGE);

			/* Read old taper rate */
			if (!BQ27441_read16(0x5B, &old_taperRate, 1000))
				return 0;

			/* Checksum calculation */
			tmp_chksum = computeCheckSum(tmp_chksum, old_taperRate, CONF_TAPER_RATE);

			/* Write new design capacity */
			if (!BQ27441_write16(0x4A, swapMSB_LSB(CONF_DESIGN_CAPACITY), 1000))
				return 0;

			/* Write new design energy */
			if (!BQ27441_write16(0x4C, swapMSB_LSB(CONF_DESIGN_ENERGY), 1000))
				return 0;

			/* Write new terminate voltage */
			if (!BQ27441_write16(0x50, swapMSB_LSB(CONF_TERMINATE_VOLTAGE), 1000))
				return 0;

			/* Write new taper rate */
			if (!BQ27441_write16(0x5B, swapMSB_LSB(CONF_TAPER_RATE), 1000))
				return 0;

			/* Checksum calculation */
			new_chksum = tmp_chksum;

			/* Write new checksum */
			if (!BQ27441_command(BLOCK_DATA_CHECKSUM, new_chksum, 1000))
				return 0;

			usleep(400000);

			/* Read Block Data Checksum */
			if (!BQ27441_readChecksum(&chksum, 1000))
				return 0;

			usleep(400000);
		}
		while(new_chksum != chksum);

		/* Send SOFT_RESET control command */
		if (!BQ27441_control(SOFT_RESET, 1000))
			return 0;

		usleep(400000);

		result = 0;
		/* Check if CFGUPMODE bit is cleared in FLAGS */
		while(result & 0x0010)
		{
			if (!BQ27441_read16(FLAGS, &result, 1000))
				return 0;
		}

		dmcf_debugprintf("*************************************");
		dmcf_debugprintf("BQ27441 config inialized succesfully");
		dmcf_debugprintf("*************************************");
		return 1;
	}
	else
	{
		dmcf_debugprintf("*************************************");
		dmcf_debugprintf("BQ27441 config inialized already");
		dmcf_debugprintf("*************************************");
		return 1;
	}
}


/* Configures BQ27441 opconfig */
bool BQ27441_initOpConfig()
{
	short result = 0;

	/* Instructs fuel gauge to enter CONFIG UPDATE mode. */
	if (!BQ27441_control(SET_CFGUPDATE, 1000))
		return 0;

	usleep(400000);

	result = 0;
	/* Check if CFGUPMODE bit is set in FLAGS */
	while(!(result & 0x0010))
	{
		if (!BQ27441_read16(FLAGS, &result, 1000))
			return 0;
	}

	/* Enable Block data memory control */
	if (!BQ27441_command(BLOCK_DATA_CONTROL, 0x00, 1000))
		return 0;

	/* Set the data class to be accessed */
	if (!BQ27441_command(DATA_CLASS, 0x40, 1000))
		return 0;

	/* Write the block offset loaction */
	if (!BQ27441_command(DATA_BLOCK, 0x00, 1000))
		return 0;

	usleep(400000);

	char old_chksum = 0;
	char new_chksum = 0;
	char tmp_chksum = 0;
	char chksum = 0;
	do
	{
		/* Read Block Data Checksum */
		if (!BQ27441_readChecksum(&old_chksum, 1000))
			return 0;

		usleep(400000);

		/* Checksum calculation */
		tmp_chksum = old_chksum;

		short old_opconfig= 0;

		/* Read old opconfig */
		if (!BQ27441_read16(0x40, &old_opconfig, 1000))
			return 0;

		tmp_chksum = computeCheckSum(tmp_chksum, old_opconfig, 0x05F8);

		/* Write new opconfig */
		if (!BQ27441_write16(0x40, swapMSB_LSB(0x05F8), 1000))
			return 0;

		/* Checksum calculation */
		new_chksum = tmp_chksum;

		/* Write new checksum */
		if (!BQ27441_command(BLOCK_DATA_CHECKSUM, new_chksum, 1000))
			return 0;

		usleep(400000);

		/* Read Block Data Checksum */
		if (!BQ27441_readChecksum(&chksum, 1000))
			return 0;

		usleep(400000);
	}
	while(new_chksum != chksum);

	/* Send SOFT_RESET control command */
	if (!BQ27441_control(SOFT_RESET, 1000))
		return 0;

	usleep(400000);

	result = 0;
	/* Check if CFGUPMODE bit is cleared in FLAGS */
	while(result & 0x0010)
	{
		if (!BQ27441_read16(FLAGS, &result, 1000))
			return 0;
	}

	// Read the Operation Config
	short result16 = 0;
    if (!BQ27441_read16(OP_CONFIG, &result16, 1000))
        return 0;

    // Check if BIE is cleared in Operation Config
    if (result16 & 0x2000)
    	return 0;

    return 1;
}

/* Send control subcommand */
bool BQ27441_control(short subcommand, unsigned int timeout)
{
    bool ret;
    uint8_t buf[3];
    buf[0] = CONTROL;
    buf[1] = (subcommand & 0xFF);
    buf[2] = subcommand >> 8;

    I2C_Transaction trans;

    trans.readBuf = NULL;
    trans.readCount = 0;
    trans.slaveAddress = BQ27441_SLAVE_ADDRESS;
    trans.writeBuf = buf;
    trans.writeCount = 3;

    ret = I2C_transfer(battpack_i2c, &trans);

    return ret;
}


/* Send control subcommand then read from control command */
bool BQ27441_controlRead(short subcommand, short *result, unsigned int timeout)
{
    bool ret;
    uint8_t buf[3];

    I2C_Transaction trans;

    ret = BQ27441_control(subcommand, timeout);

    if(!ret)
        return ret;

    usleep(400000);

    buf[0] = CONTROL;

    trans.readBuf = result;
    trans.readCount = 2;
    trans.writeBuf = buf;
    trans.writeCount = 1;
    trans.slaveAddress = BQ27441_SLAVE_ADDRESS;

	ret = I2C_transfer(battpack_i2c, &trans);

	return ret;
}

/* Send command */
bool BQ27441_command(short command, char data, unsigned int timeout)
{
	bool ret;
	uint8_t buf[2];
	buf[0] = command;
	buf[1] = data;

    I2C_Transaction trans;

    trans.readBuf = NULL;
    trans.readCount = 0;
    trans.writeBuf = buf;
    trans.writeCount = 2;
    trans.slaveAddress = BQ27441_SLAVE_ADDRESS;

    ret = I2C_transfer(battpack_i2c, &trans);

	return ret;
}

/* Write word to address */
bool BQ27441_write16(short addr, short data, unsigned int timeout)
{
	bool ret;

	uint8_t buf[3];
	buf[0] = addr;
	buf[1] = (data & 0xFF);
	buf[2] = data >> 8;

    I2C_Transaction trans;

    trans.readBuf = NULL;
    trans.readCount = 0;
    trans.slaveAddress = BQ27441_SLAVE_ADDRESS;
    trans.writeBuf = buf;
    trans.writeCount = 3;

    ret = I2C_transfer(battpack_i2c, &trans);

	return ret;
}


/* Read from standard command*/
bool BQ27441_read16(short stdcommand, short *result, unsigned int timeout)
{
    bool ret;

    I2C_Transaction trans;

    trans.readBuf = result;
    trans.readCount = 2;
    trans.slaveAddress = BQ27441_SLAVE_ADDRESS;
    trans.writeBuf = &stdcommand;
    trans.writeCount = 1;

    ret = I2C_transfer(battpack_i2c, &trans);

    return ret;
}


/* Read block data checksum */
bool BQ27441_readChecksum(char *result, unsigned int timeout)
{
	bool ret;
	uint8_t buf = BLOCK_DATA_CHECKSUM;

	I2C_Transaction trans;

	trans.readBuf = result;
	trans.readCount = 1;
	trans.slaveAddress = BQ27441_SLAVE_ADDRESS;
	trans.writeBuf = &buf;
	trans.writeCount = 1;

	ret = I2C_transfer(battpack_i2c, &trans);

	return ret;
}


/* Computes checksum for fuel gauge */
static unsigned char computeCheckSum(unsigned char oldCheckSum, int oldData, int newData)
{
	unsigned char tmpCheckSum = 0xFF - oldCheckSum - ( unsigned char )oldData - ( unsigned char )( oldData >> 8 );
	unsigned char newCheckSum = 0xFF - (  tmpCheckSum + ( unsigned char )newData + ( unsigned char )( newData >> 8 ) );
	return newCheckSum;
}


/* Swaps the MSB and LSB of a word */
static int swapMSB_LSB(int data)
{
	int tmp = ( unsigned char )data;
	tmp = tmp << 8;
	tmp += ( unsigned char )( data >> 8 );
	return tmp;
}
