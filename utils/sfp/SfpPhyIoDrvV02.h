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
/*!\file    SfpPhyIoDrvV02.h
   \brief   SFP PHY I/O driver

   This file contains the SFP PHY I/O driver class definition
*/
// ------------------------------------------------------------------------------------------------
#ifndef __SFPPHYIODRVV02_H__
#define __SFPPHYIODRVV02_H__

#include <accedian/acclib/BaseIoDrv.h>
#include <accedian/acclib/sys_defs.h>
#include "HalBitDef.h"

class Logger;
class I2cIoDrvV02;

// ------------------------------------------------------------------------------------------------
/*!@brief SFP PHY I/O driver

   This class implements I/O driver used to interact with the SFP PHY
*/
// ------------------------------------------------------------------------------------------------
class SfpPhyIoDrvV02 : public BaseIoDrv<acd_uint16_t>
{

public:
   SfpPhyIoDrvV02(const char* a_name, I2cIoDrvV02* a_pI2cIoDrv, BaseIoDrv<acd_uint64_t>* a_pIoBase,
                  acd_uint32_t a_i2cSelect, acd_uint32_t a_baseAddress);
   virtual ~SfpPhyIoDrvV02();
   virtual bool Read(acd_uint32_t a_reg, acd_uint16_t& a_data, bool a_bCheckState = true);
   virtual bool Write(acd_uint32_t a_reg, acd_uint16_t a_data, bool a_bCheckState = true);

private:

   static const acd_uint32_t SFP_PHY_MEM_REG = 0xAC;

   I2cIoDrvV02*               m_pI2cIoDrv;
   BaseIoDrv<acd_uint64_t>*   m_pIoBase;
   Logger*                    m_pLogger;
   acd_uint32_t               m_baseAddress;
};

#endif   // __SFPPHYIODRVV02_H__
