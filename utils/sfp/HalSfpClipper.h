// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2012 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    HalSfpClipper.h
   @brief   Clipper SFP Hardware Abstraction Layer

   This file contains the Clipper SFP HAL class implementation
*/
// ------------------------------------------------------------------------------------------------
#ifndef __HALSFPCLIPPER_H__
#define __HALSFPCLIPPER_H__

#include "HalSfp.h"
#include <accedian/acclib/BaseIoDrv.h>

// ------------------------------------------------------------------------------------------------
/*!@brief Clipper SFP Hardware Abstraction Layer

   This class implements the SFP HAL on Clipper
*/
// ------------------------------------------------------------------------------------------------
class HalSfpClipper : public HalSfp
{

public:
   HalSfpClipper(const char* a_name, HalPortId &a_portId,
                 BaseIoDrv<acd_uint64_t>* a_pIoDrv,
                 BaseIoDrv<acd_uint8_t>* a_pI2cIoDrv,
                 HalSfpSpeed             a_defaultSpeed);
   virtual ~HalSfpClipper();

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

   static acd_uint64_t s_status1;
   static acd_uint64_t s_status2;
};

#endif // #ifndef __HALSFPCLIPPER_H__
