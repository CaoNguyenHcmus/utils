// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2012 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    HalSfpE4.cpp
   @brief   Etchell-4 SFP Hardware Abstraction Layer base class

   This file contains the Etchell-4 SFP HAL class implementation

*/
#include "HalSfpE4.h"
#include <accedian/acclib/acd_utils.h>

acd_uint64_t HalSfpE4::s_status = 0;

// ================================================================================================
// ================================================================================================
//            PUBLIC CLASS SECTION
// ================================================================================================
// ================================================================================================
// ------------------------------------------------------------------------------------------------
/*!@brief Constructor

   @param [in]     a_name        : Instance name
   @param [in]     a_portId      : Port identifier
   @param [in]     a_pIoDrv      : FPGA I/O driver
   @param [in]     a_pI2cIoDrv   : I2C I/O driver
*/
// ------------------------------------------------------------------------------------------------
HalSfpE4::HalSfpE4(const char*              a_name,
                   HalPortId&               a_portId,
                   BaseIoDrv<acd_uint64_t>* a_pIoDrv,
                   BaseIoDrv<acd_uint8_t>*  a_pI2cIoDrv) :
HalSfp(a_name, HalSfpSpeed1G),
m_portId(a_portId),
m_pIoDrv(a_pIoDrv),
m_pI2cIoDrv(a_pI2cIoDrv)
{
   HalSetDebug(false);
}

// ------------------------------------------------------------------------------------------------
/*!@brief Destructor

*/
// ------------------------------------------------------------------------------------------------
HalSfpE4::~HalSfpE4()
{
}

// ------------------------------------------------------------------------------------------------
/*!@brief Enable the SFP

   Power-up the SFP

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpE4::Enable()
{
   bool bRet = true;
   SfpControlReg_t   ctrl;
   acd_uint64_t      val64;

   HalDebug("Enable");
   if ( m_pIoDrv->Read(SFP_CONTROL_REG, val64) )
   {
      ctrl.value = val64;
      switch (m_portId)
      {
         case HalPortId1:
            ctrl.sfp1_enable_n = 0;
            break;
         case HalPortId2:
            ctrl.sfp2_enable_n = 0;
            break;
         case HalPortId3:
            ctrl.sfp3_enable_n = 0;
            break;
         case HalPortId4:
            ctrl.sfp4_enable_n = 0;
            break;
         default:
            bRet = false;
            break;
      }
      if ( ctrl.value != val64 )
      {
         bRet &= m_pIoDrv->Write(SFP_CONTROL_REG, ctrl.value);
      }
   }
   return bRet && HalSfp::Enable();
}

// ------------------------------------------------------------------------------------------------
/*!@brief Disable the SFP

   Power-down the SFP

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpE4::Disable()
{
   bool bRet = true;
   SfpControlReg_t   ctrl;
   acd_uint64_t      val64;

   HalDebug("Disable");
   if ( m_pIoDrv->Read(SFP_CONTROL_REG, val64) )
   {
      ctrl.value = val64;
      switch (m_portId)
      {
         case HalPortId1:
            ctrl.sfp1_enable_n = 1;
            break;
         case HalPortId2:
            ctrl.sfp2_enable_n = 1;
            break;
         case HalPortId3:
            ctrl.sfp3_enable_n = 1;
            break;
         case HalPortId4:
            ctrl.sfp4_enable_n = 1;
            break;
         default:
            bRet = false;
            break;
      }
      if ( ctrl.value != val64 )
      {
         bRet &= m_pIoDrv->Write(SFP_CONTROL_REG, ctrl.value);
      }
   }
   return bRet && HalSfp::Disable();
}

// ------------------------------------------------------------------------------------------------
/*!@brief Set the SFP Tx enable

   @param [in]     a_bEnable     : Flag to control the SFP Tx enable

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpE4::SetTxEnable(bool a_bEnable)
{
   bool bRet = true;
   SfpControlReg_t   ctrl;
   acd_uint64_t      val64;

   HalDebug("SetTxEnable(%d)", a_bEnable);
   if ( m_pIoDrv->Read(SFP_CONTROL_REG, val64) )
   {
      ctrl.value = val64;
      switch (m_portId)
      {
         case HalPortId1:
            ctrl.sfp1_txdisable = !a_bEnable;
            break;
         case HalPortId2:
            ctrl.sfp2_txdisable = !a_bEnable;
            break;
         case HalPortId3:
            ctrl.sfp3_txdisable = !a_bEnable;
            break;
         case HalPortId4:
            ctrl.sfp4_txdisable = !a_bEnable;
            break;
         default:
            bRet = false;
            break;
      }
      if ( ctrl.value != val64 )
      {
         bRet &= m_pIoDrv->Write(SFP_CONTROL_REG, ctrl.value);
      }
   }
   return bRet && HalSfp::SetTxEnable(a_bEnable);
}

// ------------------------------------------------------------------------------------------------
/*!@brief Check SFP presence

   It is assumed that the RefreshStatus method is called prior to call this method to get
   an up to date status

   @return     true if SFP is present
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpE4::IsPresent()
{
   SfpStatusReg_t    regVal;

   regVal.value = s_status;
   switch (m_portId)
   {
      case HalPortId1:
         m_isPresent = !regVal.sfp1_detect_n;
         break;
      case HalPortId2:
         m_isPresent = !regVal.sfp2_detect_n;
         break;
      case HalPortId3:
         m_isPresent = !regVal.sfp3_detect_n;
         break;
      case HalPortId4:
         m_isPresent = !regVal.sfp4_detect_n;
         break;
      default:
         m_isPresent = false;
         break;
   }
   if ( !m_isPresent )
   {
      m_logErrorCount = 0;
   }
   //HalDebug("SFP %d is %s", m_portId, m_isPresent ? "present" : "not present");
   return m_isPresent;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Update SFP data

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpE4::UpdateData()
{
   acd_uint8_t buffer[128];
   bool bRet = false;

   if ( !m_isPresent || !m_bEnable )
   {
      return false;
   }

   //HalDebug("UpdateData");
   if ( m_pI2cIoDrv->Read(0xA0, sizeof(buffer), buffer) )
   {
      memcpy(m_interfaceData, buffer, sizeof (buffer));
      bRet = HalSfp::UpdateData();
      if (!bRet)
      {
         //HalError("HalSfp::UpdateData() failed");
      }
   }
   else
   {
      if ( ++m_logErrorCount < SFP_LOG_ERROR_THRESHOLD )
      {
         HalError("0xA0 EEPROM read failed");
      }
   }

   m_speedCap[HalSfpSpeed10G] = false;

   if (m_bIsCopper)
   {
      if ( m_pI2cIoDrv->Read(0xAC, sizeof(buffer)/2, buffer) )
      {
         memcpy(m_phyData, buffer, sizeof(buffer)/2);
      }
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Update SFP monitoring data

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpE4::UpdateMonitoringData()
{
   bool        bRet = false;
   acd_uint8_t buffer[128];

   if ( !m_isPresent || !m_bEnable )
   {
      return false;
   }
   //HalDebug("UpdateMonitoringData");

   if ( m_pI2cIoDrv->Read(0xA2, sizeof(buffer), buffer) )
   {
      memcpy(m_monData, buffer, sizeof (buffer));
      bRet = HalSfp::UpdateMonitoringData();
      if (!bRet)
      {
         //HalError("HalSfp::UpdateMonitoringData() failed");
      }
   }
   else
   {
      if ( ++m_logErrorCount < SFP_LOG_ERROR_THRESHOLD )
      {
         HalError("0xA2 EEPROM read failed");
      }
   }

   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Refresh status

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpE4::RefreshStatus()
{
   return m_pIoDrv->Read(SFP_STATUS_REG, s_status);
}
