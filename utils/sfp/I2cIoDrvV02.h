// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2014 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO
   PART OF THIS PROGRAM OR PUBLICATION MAY BE REPRODUCED, TRANSMITTED,
   TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM, OR TRANSLATED INTO ANY LANGUAGE
   OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC, MECHANICAL,
   MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR
   WRITTEN PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!\file    I2cIoDrvV02.h
   \brief   I2C I/O driver version 2

   This file contains the I2C I/O driver class definition
   The version 2 supports I2C page read first introduced on the Ethell-5
*/
// ------------------------------------------------------------------------------------------------
#ifndef __I2CIODRVV02_H__
#define __I2CIODRVV02_H__

#include <pthread.h>

#include <accedian/acclib/BaseIoDrv.h>
#include <accedian/acclib/sys_defs.h>
#include "HalBitDef.h"

class Logger;

// ------------------------------------------------------------------------------------------------
/*!@brief I2C I/O driver

   This class implements I/O driver used by the HalSfp classes
*/
// ------------------------------------------------------------------------------------------------
class I2cIoDrvV02 : public BaseIoDrv<acd_uint8_t>
{

public:
   I2cIoDrvV02(const char* a_name, BaseIoDrv<acd_uint64_t>* a_pIoBase, acd_uint32_t a_i2cSelec, acd_uint32_t a_baseAddress);
   virtual ~I2cIoDrvV02();
   virtual bool Read(acd_uint32_t a_reg, acd_uint8_t& a_data, bool a_bCheckState = true);
   virtual bool Read(acd_uint32_t a_reg, acd_uint32_t a_nbr, acd_uint8_t* a_data, bool a_bCheckState = true);
   virtual bool Write(acd_uint32_t a_reg, acd_uint8_t a_data, bool a_bCheckState = true);
   virtual bool Write(acd_uint32_t a_reg, acd_uint32_t a_count, acd_uint8_t* a_data, bool a_bCheckState = true);

   bool select(acd_uint32_t a_reg, acd_uint32_t a_off);
   bool waitbusy(acd_uint32_t a_timeoutMs);
   bool lock();
   bool unlock();

   static const acd_uint32_t I2C_SELECT_REG    = 0x00;
   static const acd_uint32_t I2C_CONTROL_REG   = 0x01;
   static const acd_uint32_t I2C_STATUS_REG    = 0x02;
   static const acd_uint32_t I2C_DATA_REG      = 0x10;
   static const acd_uint32_t I2C_DATA_SIZE     = 0x10;

   enum I2cLen
   {
      eI2C_LEN_1BYTE = 0,
      eI2C_LEN_2BYTE,
      eI2C_LEN_3BYTE,
      eI2C_LEN_4BYTE
   };

   enum I2cCmd
   {
      eI2C_CMD_WR = 0,
      eI2C_CMD_RD
   };

   union I2cSelectReg_t   // I2C select register
   {
      struct {
         BITFIELD_2(acd_uint64_t,
         Unused            ,  60,
         i2c_sel           ,   4)   // Controls whether the I2C line driver goes to SFP1 (0), SFP2 (1), SFP3 (2) or SFP4 (3)
      };
      acd_uint64_t value;
   };

   union I2cControlReg_t   // Indexed addresses for I2C input access of the SFPs
   {
      struct {
         BITFIELD_9(acd_uint64_t,
         unused3     ,   9,
         length      ,   7,   // Nbr of bytes = length + 1 :  0=1, 1=2, 2=3, ... In write, the maximum is 3 (4 bytes = 32 bits) due to wdata is a 32 bits only
         unused2     ,   4,
         command     ,   1,   // Command Rd = 1, Wr = 0
         start       ,   1,   // Start
         stop        ,   1,   // Stop
         unused1     ,   2,
         address     ,   7,   // Device address
         wrdata      ,  32)   // Write data
      };
      acd_uint64_t value;
   };

   union I2cStatusReg_t   // I2C controller status
   {
      struct {
         BITFIELD_6(acd_uint64_t,
         unused3     ,  49,
         bytes_left  ,   7,   // Remaining number of bytes - 1 to write or read
         unused2     ,   3,
         pending_cmd ,   1,
         unused1     ,   2,
         status      ,   2)   // Status 00: no error,  01: i2c_module_busy,  1X: error
      };
      acd_uint64_t value;
   };

   union I2cRdDataReg_t   // I2C read data
   {
      struct {
         BITFIELD_8(acd_uint64_t,
         byte8       ,   8,
         byte7       ,   8,
         byte6       ,   8,
         byte5       ,   8,
         byte4       ,   8,
         byte3       ,   8,
         byte2       ,   8,
         byte1       ,   8)
      };
      acd_uint64_t value;
   };

private:

   BaseIoDrv<acd_uint64_t>*   m_pIoBase;
   Logger*                    m_pLogger;
   acd_uint32_t               m_baseAddress;
   static pthread_mutex_t     s_mutex;
};

#endif   // __I2CIODRVV02_H__
