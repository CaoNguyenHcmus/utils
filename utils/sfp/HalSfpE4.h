// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2012 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    HalSfpE4.h
   @brief   Etchell-4 SFP Hardware Abstraction Layer

   This file contains the Etchell-4 SFP HAL class implementation
*/
// ------------------------------------------------------------------------------------------------
#ifndef __HALSFPE4_H__
#define __HALSFPE4_H__

#include "HalSfp.h"
#include <accedian/acclib/BaseIoDrv.h>

// ------------------------------------------------------------------------------------------------
/*!@brief Etchell-4 SFP Hardware Abstraction Layer

   This class implements the SFP HAL on Etchell-4
*/
// ------------------------------------------------------------------------------------------------
class HalSfpE4 : public HalSfp
{

public:
   HalSfpE4(const char* a_name, HalPortId &a_portId, BaseIoDrv<acd_uint64_t>* a_pIoDrv, BaseIoDrv<acd_uint8_t>* a_pI2cIoDrv);
   virtual ~HalSfpE4();

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

   static const acd_uint32_t  SFP_CONTROL_REG = 0x01;
   static const acd_uint32_t  SFP_STATUS_REG  = 0x86;

   union SfpControlReg_t
   {
      struct
      {
#if BYTE_ORDER == BIG_ENDIAN
         acd_uint64_t   Unused1              :  32;
         acd_uint64_t   Unused2              :  20;
         acd_uint64_t   sfp4_txdisable       :  1;
         acd_uint64_t   sfp4_ratesel         :  1;
         acd_uint64_t   sfp4_enable_n        :  1;
         acd_uint64_t   sfp3_txdisable       :  1;
         acd_uint64_t   sfp3_ratesel         :  1;
         acd_uint64_t   sfp3_enable_n        :  1;
         acd_uint64_t   sfp2_txdisable       :  1;
         acd_uint64_t   sfp2_ratesel         :  1;
         acd_uint64_t   sfp2_enable_n        :  1;
         acd_uint64_t   sfp1_txdisable       :  1;
         acd_uint64_t   sfp1_ratesel         :  1;
         acd_uint64_t   sfp1_enable_n        :  1;
#else
         acd_uint64_t   sfp1_enable_n        :  1;
         acd_uint64_t   sfp1_ratesel         :  1;
         acd_uint64_t   sfp1_txdisable       :  1;
         acd_uint64_t   sfp2_enable_n        :  1;
         acd_uint64_t   sfp2_ratesel         :  1;
         acd_uint64_t   sfp2_txdisable       :  1;
         acd_uint64_t   sfp3_enable_n        :  1;
         acd_uint64_t   sfp3_ratesel         :  1;
         acd_uint64_t   sfp3_txdisable       :  1;
         acd_uint64_t   sfp4_enable_n        :  1;
         acd_uint64_t   sfp4_ratesel         :  1;
         acd_uint64_t   sfp4_txdisable       :  1;
         acd_uint64_t   Unused2              :  20;
         acd_uint64_t   Unused1              :  32;
#endif
      };
      acd_uint64_t value;
   }__attribute__((__packed__));

   union SfpStatusReg_t
   {
      struct
      {
#if BYTE_ORDER == BIG_ENDIAN
         acd_uint64_t   Unused1              :  32;
         acd_uint64_t   Unused2              :  16;
         acd_uint64_t   sfp4_txfault         :  1;
         acd_uint64_t   sfp4_sdet_n          :  1;    // 0x00 signal detected from sfp, 0x01 no signal detected from sfp
         acd_uint64_t   sfp4_detect_n        :  1;    // 0x00 sfp detected, 0x01 no sfp detected
         acd_uint64_t   sfp4_enable_n_status :  1;    // actual status of the sfp4_enable_n pin
         acd_uint64_t   sfp3_txfault         :  1;
         acd_uint64_t   sfp3_sdet_n          :  1;    // 0x00 signal detected from sfp, 0x01 no signal detected from sfp
         acd_uint64_t   sfp3_detect_n        :  1;    // 0x00 sfp detected, 0x01 no sfp detected
         acd_uint64_t   sfp3_enable_n_status :  1;    // actual status of the sfp3_enable_n pin
         acd_uint64_t   sfp2_txfault         :  1;
         acd_uint64_t   sfp2_sdet_n          :  1;    // 0x00 signal detected from sfp, 0x01 no signal detected from sfp
         acd_uint64_t   sfp2_detect_n        :  1;    // 0x00 sfp detected, 0x01 no sfp detected
         acd_uint64_t   sfp2_enable_n_status :  1;    // actual status of the sfp2_enable_n pin
         acd_uint64_t   sfp1_txfault         :  1;
         acd_uint64_t   sfp1_sdet_n          :  1;    // 0x00 signal detected from sfp, 0x01 no signal detected from sfp
         acd_uint64_t   sfp1_detect_n        :  1;    // 0x00 sfp detected, 0x01 no sfp detected
         acd_uint64_t   sfp1_enable_n_status :  1;    // actual status of the sfp1_enable_n pin
#else
         acd_uint64_t   sfp1_enable_n_status :  1;    // actual status of the sfp1_enable_n pin
         acd_uint64_t   sfp1_detect_n        :  1;    // 0x00 sfp detected, 0x01 no sfp detected
         acd_uint64_t   sfp1_sdet_n          :  1;    // 0x00 signal detected from sfp, 0x01 no signal detected from sfp
         acd_uint64_t   sfp1_txfault         :  1;
         acd_uint64_t   sfp2_enable_n_status :  1;    // actual status of the sfp2_enable_n pin
         acd_uint64_t   sfp2_detect_n        :  1;    // 0x00 sfp detected, 0x01 no sfp detected
         acd_uint64_t   sfp2_sdet_n          :  1;    // 0x00 signal detected from sfp, 0x01 no signal detected from sfp
         acd_uint64_t   sfp2_txfault         :  1;
         acd_uint64_t   sfp3_enable_n_status :  1;    // actual status of the sfp3_enable_n pin
         acd_uint64_t   sfp3_detect_n        :  1;    // 0x00 sfp detected, 0x01 no sfp detected
         acd_uint64_t   sfp3_sdet_n          :  1;    // 0x00 signal detected from sfp, 0x01 no signal detected from sfp
         acd_uint64_t   sfp3_txfault         :  1;
         acd_uint64_t   sfp4_enable_n_status :  1;    // actual status of the sfp4_enable_n pin
         acd_uint64_t   sfp4_detect_n        :  1;    // 0x00 sfp detected, 0x01 no sfp detected
         acd_uint64_t   sfp4_sdet_n          :  1;    // 0x00 signal detected from sfp, 0x01 no signal detected from sfp
         acd_uint64_t   sfp4_txfault         :  1;
         acd_uint64_t   Unused2              :  16;
         acd_uint64_t   Unused1              :  32;
#endif
      };
      acd_uint64_t value;
   }__attribute__((__packed__));

   static acd_uint64_t s_status;

};
#endif // #ifndef __HALSFPE4_H__
