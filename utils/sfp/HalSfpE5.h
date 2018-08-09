// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2012 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    HalSfpE5.h
   @brief   Etchell-5 SFP Hardware Abstraction Layer

   This file contains the Etchell-5 SFP HAL class implementation
*/
// ------------------------------------------------------------------------------------------------
#ifndef __HALSFPE5_H__
#define __HALSFPE5_H__

#include "HalSfp.h"
#include <accedian/acclib/BaseIoDrv.h>

// ------------------------------------------------------------------------------------------------
/*!@brief Etchell-5 SFP Hardware Abstraction Layer

   This class implements the SFP HAL on Etchell-5
*/
// ------------------------------------------------------------------------------------------------
class HalSfpE5 : public HalSfp
{

public:
   HalSfpE5(const char* a_name, HalPortId &a_portId, BaseIoDrv<acd_uint64_t>* a_pIoDrv, BaseIoDrv<acd_uint8_t>* a_pI2cIoDrv);
   virtual ~HalSfpE5();

   virtual bool Enable();
   virtual bool Disable();
   virtual bool SetTxEnable(bool a_bEnable);

   virtual bool IsPresent();
   virtual bool UpdateData();
   virtual bool UpdateMonitoringData();

   virtual bool RefreshStatus();

private:
   HalPortId   m_portId;
   BaseIoDrv<acd_uint64_t>* m_pIoDrv;     // The I/O driver used to access the FPGA registers
   BaseIoDrv<acd_uint8_t>*  m_pI2cIoDrv;  // The I/O driver used to access the I2C registers

   static acd_uint64_t s_status;
};
#endif // #ifndef __HALSFPE5_H__
