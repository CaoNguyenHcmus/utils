// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2014 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    SfpPhyIoDrvV02.cpp
   @brief   This file contains the SFP PHY I/O driver implementation

*/
// ------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "SfpPhyIoDrvV02.h"
#include <accedian/acclib/Logger.h>
#include <accedian/acclib/acd_utils.h>
#include "I2cIoDrvV02.h"

// ================================================================================================
// ================================================================================================
//            PUBLIC CLASS SECTION
// ================================================================================================
// ================================================================================================
// ------------------------------------------------------------------------------------------------
/*!@brief Constructor

   @param [in]     a_name        : Driver instance name
   @param [in]     a_pI2cIoDrv   : I2C I/O driver base instance
   @param [in]     a_pIoBase     : I/O driver base instance
   @param [in]     a_i2cSelect   : I2C select
   @param [in]     a_baseAddress : I2C base address
*/
// ------------------------------------------------------------------------------------------------
SfpPhyIoDrvV02::SfpPhyIoDrvV02(const char* a_name, I2cIoDrvV02* a_pI2cIoDrv, BaseIoDrv<acd_uint64_t>* a_pIoBase,
                               acd_uint32_t a_i2cSelect, acd_uint32_t a_baseAddress) :
BaseIoDrv<acd_uint16_t>(a_i2cSelect),
m_pI2cIoDrv(a_pI2cIoDrv),
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
SfpPhyIoDrvV02::~SfpPhyIoDrvV02()
{
   delete m_pLogger;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Read the content of a register and return its value

   @param [in]     a_reg         : Register offset
   @param [out]    a_data        : Register content
   @param [in]     a_bCheckState : Flag to check the driver state before performing the access

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool SfpPhyIoDrvV02::Read(acd_uint32_t a_reg, acd_uint16_t& a_data, bool a_bCheckState)
{
   I2cIoDrvV02::I2cControlReg_t   control;
   I2cIoDrvV02::I2cRdDataReg_t    data;

   //m_pLogger->LogDebug("Read(%08xh)", a_reg);

   m_pI2cIoDrv->lock();

   // Select I2C device & memory region to address
   if ( !m_pI2cIoDrv->select(SFP_PHY_MEM_REG, a_reg) )
   {
      m_pI2cIoDrv->unlock();
      return false;
   }

   // Read actual memory region
   control.value    = 0;
   control.command  = I2cIoDrvV02::eI2C_CMD_RD;
   control.start    = 1;
   control.stop     = 1;
   control.length   = I2cIoDrvV02::eI2C_LEN_2BYTE;
   control.address  = SFP_PHY_MEM_REG>>1;

   // Send read command
   if ( !m_pIoBase->Write(m_baseAddress + I2cIoDrvV02::I2C_CONTROL_REG, 1, &control.value, true) )
   {
      m_pI2cIoDrv->unlock();
      return false;
   }

   // Pool for read completion, error or timeout
   if ( !m_pI2cIoDrv->waitbusy(10) )
   {
      m_pI2cIoDrv->unlock();
      return false;
   }

   // Read device data
   if ( !m_pIoBase->Read(m_baseAddress + I2cIoDrvV02::I2C_DATA_REG, data.value) )
   {
      m_pI2cIoDrv->unlock();
      m_pLogger->LogDebug("I2C read I/O error");
      return false;
   }

   a_data = data.byte8;
   a_data = a_data << 8;
   a_data |= data.byte7;

   m_pI2cIoDrv->unlock();
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
bool SfpPhyIoDrvV02::Write(
    acd_uint32_t    a_reg,
    acd_uint16_t    a_data,
    bool            a_bCheckState)
{
   I2cIoDrvV02::I2cControlReg_t   control;

   //m_pLogger->LogDebug("Write(%08xh, %02xh)", a_reg, a_data);

   m_pI2cIoDrv->lock();

   // Select I2C device & memory region to address
   if ( !m_pI2cIoDrv->select(SFP_PHY_MEM_REG, 0) )
   {
      m_pI2cIoDrv->unlock();
      return false;
   }

   control.value    = 0;
   control.command  = I2cIoDrvV02::eI2C_CMD_WR;
   control.start    = 1;
   control.stop     = 1;
   control.length   = I2cIoDrvV02::eI2C_LEN_3BYTE;
   control.address  = SFP_PHY_MEM_REG >> 1;
   control.wrdata   = ((a_data & 0x00ff) << 8) | ((a_data & 0xff00) << 8) | (a_reg << 24);

   // Send write command
   if ( m_pIoBase->Write(m_baseAddress + I2cIoDrvV02::I2C_CONTROL_REG, 1, &control.value, true) )
   {
      // Pool for read completion, error or timeout
      if ( !m_pI2cIoDrv->waitbusy(10) )
      {
         m_pI2cIoDrv->unlock();
         return false;
      }
   }
   m_pI2cIoDrv->unlock();
   return true;
}
