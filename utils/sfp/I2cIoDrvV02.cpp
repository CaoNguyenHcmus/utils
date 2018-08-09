// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2014 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    I2cIoDrvV02.cpp
   @brief   This file contains the I2C I/O driver version 2 implementation

*/
// ------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>

#include "I2cIoDrvV02.h"
#include <accedian/acclib/Logger.h>
#include <accedian/acclib/acd_utils.h>

pthread_mutex_t I2cIoDrvV02::s_mutex = PTHREAD_MUTEX_INITIALIZER;

// ================================================================================================
// ================================================================================================
//            PUBLIC CLASS SECTION
// ================================================================================================
// ================================================================================================
// ------------------------------------------------------------------------------------------------
/*!@brief Constructor

   @param [in]     a_name        : Driver instance name
   @param [in]     a_pIoBase     : I/O driver base instance
   @param [in]     a_i2cSelect   : I2C select
   @param [in]     a_baseAddress : I2C base address
*/
// ------------------------------------------------------------------------------------------------
I2cIoDrvV02::I2cIoDrvV02(const char* a_name, BaseIoDrv<acd_uint64_t>* a_pIoBase,
                         acd_uint32_t a_i2cSelect, acd_uint32_t a_baseAddress) :
BaseIoDrv<acd_uint8_t>(a_i2cSelect),
m_pIoBase(a_pIoBase),
m_pLogger(NULL),
m_baseAddress(a_baseAddress)
{
   m_pLogger = new Logger(a_name);
   m_pLogger->SetDebug(false);
}

// ------------------------------------------------------------------------------------------------
/*!@brief Destructor

*/
// ------------------------------------------------------------------------------------------------
I2cIoDrvV02::~I2cIoDrvV02()
{
   delete m_pLogger;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Read a set of contiguous registers

   @param [in]     a_reg         : Register offset
   @param [in]     a_nbr         : Number of registers to read
   @param [out]    a_data        : Register(s) content
   @param [in]     a_bCheckState : Flag to check the driver state before performing the access

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool I2cIoDrvV02::Read(acd_uint32_t a_reg, acd_uint32_t a_nbr, acd_uint8_t* a_data, bool a_bCheckState)
{
   I2cControlReg_t   control;
   acd_uint64_t      data[I2C_DATA_SIZE];

   //m_pLogger->LogDebug("Read(%08xh, %d)", a_reg, a_nbr);
   if ( a_nbr > (I2C_DATA_SIZE * sizeof(acd_uint64_t)) )
   {
      return false;
   }

   lock();

   // Select I2C device & memory region to address
   if ( !select(a_reg, 0) )
   {
      unlock();
      return false;
   }

   // Read actual memory region
   control.value    = 0;
   control.command  = eI2C_CMD_RD;
   control.start    = 1;
   control.stop     = 1;
   control.length   = a_nbr - 1;
   control.address  = a_reg>>1;

   // Send read command
   if ( !m_pIoBase->Write(m_baseAddress + I2C_CONTROL_REG, 1, &control.value, true) )
   {
      unlock();
      return false;
   }

   // Pool for read completion, error or timeout
   if ( !waitbusy(100) )
   {
      unlock();
      return false;
   }
   // Read device data
   if ( !m_pIoBase->Read(m_baseAddress + I2C_DATA_REG, I2C_DATA_SIZE, data) )
   {
      m_pLogger->LogDebug("I2C read I/O error");
      unlock();
      return false;
   }
   unlock();

   // Copy data to caller's array
   memcpy(a_data, data, a_nbr);

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Read the content of a register and return its value

   @param [in]     a_reg         : Register offset
   @param [out]    a_data        : Register content
   @param [in]     a_bCheckState : Flag to check the driver state before performing the access

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool I2cIoDrvV02::Read(acd_uint32_t a_reg, acd_uint8_t& a_data, bool a_bCheckState)
{
   return Read(a_reg, 1, &a_data, a_bCheckState);
}

// ------------------------------------------------------------------------------------------------
/*!@brief Write a set of contiguous registers

   @param [in]     a_reg         : Register offset
   @param [in]     a_nbr         : Number of registers to write
   @param [in]     a_data        : Values to write
   @param [in]     a_bCheckState : Flag to check the driver state before performing the access

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool I2cIoDrvV02::Write(
    acd_uint32_t    a_reg,
    acd_uint32_t    a_nbr,
    acd_uint8_t*    a_data,
    bool            a_bCheckState)
{
   // Write 1 byte at a time
   for(acd_uint32_t i = 0 ; i < a_nbr ; i++)
   {
      if ( !Write(a_reg + i, a_data[i], a_bCheckState) )
      {
         return false;
      }
   }
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Write a value to a register

   @param [in]     a_reg         : Register offset
   @param [in]     a_data        : Value to write
   @param [in]     a_bCheckState : Flag to check the driver state before performing the access

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool I2cIoDrvV02::Write(
    acd_uint32_t    a_reg,
    acd_uint8_t     a_data,
    bool            a_bCheckState)
{
   I2cControlReg_t   control;

   lock();

   // Select I2C device & memory region to address
   if ( !select(a_reg, 0) )
   {
      unlock();
      return false;
   }

   control.value    = 0;
   control.command  = eI2C_CMD_WR;
   control.start    = 1;
   control.stop     = 1;
   control.length   = 0;   // 1 byte
   control.address  = a_reg;
   control.wrdata   = a_data << 24;

   // Send write command
   if ( m_pIoBase->Write(m_baseAddress + I2C_CONTROL_REG, 1, &control.value, true) )
   {
      // Pool for read completion, error or timeout
      if ( !waitbusy(10) )
      {
         unlock();
         return false;
      }
   }

   unlock();
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Select the I2C region to address

   @param [in]     a_reg         : Memory region
   @param [in]     a_off         : Register offset

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool I2cIoDrvV02::select(acd_uint32_t a_reg, acd_uint32_t a_off)
{
   I2cSelectReg_t    sel;
   I2cControlReg_t   control;
   acd_uint64_t      data[2];

   if ( !m_pIoBase->IsReady() )
   {
      return false;
   }

   sel.value = 0;
   sel.i2c_sel = m_baseAdd;

   // Select the memory region to read
   control.value    = 0;
   control.command  = eI2C_CMD_WR;
   control.start    = 1;
   control.stop     = 0;
   control.length   = 0;  // 1 bytes
   control.address  = a_reg >> 1;
   control.wrdata   = a_off << 24;

   // Use burst write
   data[0] = sel.value;
   data[1] = control.value;
   if ( !m_pIoBase->Write(m_baseAddress + I2C_SELECT_REG, 2, data, true) )
   {
      //m_pLogger->LogDebug("Failed to select memory region %02xh", a_reg);
      return false;
   }
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Wait while controller is busy or until timeout

   @param [in]     a_timeoutMs : Timeout value in msec

   @return     true if successful, false on timeout or error
*/
// ------------------------------------------------------------------------------------------------
bool I2cIoDrvV02::waitbusy(acd_uint32_t a_timeoutMs)
{
   I2cStatusReg_t    status;
   acd_uint32_t      timeout = 0;
   acd_uint32_t      sleep_usec = 100 * a_timeoutMs; // 10 retry
   // Pool for read completion, error or timeout
   do
   {
      acd_usleep(sleep_usec);
      m_pIoBase->Read(m_baseAddress + I2C_STATUS_REG, 1, &status.value);
      //m_pLogger->LogDebug("Status = %d  Bytes left = %d pending %d", (int)status.status, (int)status.bytes_left, (int)status.pending_cmd);
      if ( status.status > 1 )
      {
         m_pLogger->LogDebug("I2C error status %d", (int)status.status);
         return false;
      }
   }while( (status.status == 1) && (timeout++ < 10) );
   if ( timeout == 10 )
   {
      m_pLogger->LogDebug("I2C read timeout");
      return false;
   }
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Lock controller for exclusive access

   @return     true if successful, false on error
*/
// ------------------------------------------------------------------------------------------------
bool I2cIoDrvV02::lock()
{
   pthread_mutex_lock(&s_mutex);
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Unlock controller exclusive access

   @return     true if successful, false on error
*/
// ------------------------------------------------------------------------------------------------
bool I2cIoDrvV02::unlock()
{
   pthread_mutex_unlock(&s_mutex);
   return true;
}
