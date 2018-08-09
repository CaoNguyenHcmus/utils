// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2014 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    HalSfpE5.cpp
   @brief   Etchell-5 SFP Hardware Abstraction Layer base class

   This file contains the Etchell-5 SFP HAL class implementation

*/
#include "HalSfpE5.h"
#include <accedian/acclib/acd_utils.h>

static const acd_uint32_t  SFP_CONTROL_REG = 0x10000;
static const acd_uint32_t  SFP_STATUS_REG  = 0x10001;

struct sfpRegMap
{
   acd_uint32_t   regOffset;
   acd_uint32_t   bitOffset;
};
static sfpRegMap  sfpEnMap[HalPortId9] =
{
   { SFP_CONTROL_REG,  0 },   // Port 0
   { SFP_CONTROL_REG,  0 },   // Port 1
   { SFP_CONTROL_REG,  4 },   // Port 2
   { SFP_CONTROL_REG,  8 },   // Port 3
   { SFP_CONTROL_REG, 12 },   // Port 4
   { SFP_CONTROL_REG, 16 },   // Port 5
   { SFP_CONTROL_REG, 20 },   // Port 6
   { SFP_CONTROL_REG, 24 },   // Port 7
   { SFP_CONTROL_REG, 28 },   // Port 8
};

static sfpRegMap  sfpTxDisMap[HalPortId9] =
{
   { SFP_CONTROL_REG,  1 },   // Port 0
   { SFP_CONTROL_REG,  1 },   // Port 1
   { SFP_CONTROL_REG,  5 },   // Port 2
   { SFP_CONTROL_REG,  9 },   // Port 3
   { SFP_CONTROL_REG, 13 },   // Port 4
   { SFP_CONTROL_REG, 17 },   // Port 5
   { SFP_CONTROL_REG, 21 },   // Port 6
   { SFP_CONTROL_REG, 25 },   // Port 7
   { SFP_CONTROL_REG, 29 },   // Port 8
};

static sfpRegMap  sfpDetectMap[HalPortId9] =
{
   { SFP_STATUS_REG,  0 },   // Port 0
   { SFP_STATUS_REG,  0 },   // Port 1
   { SFP_STATUS_REG,  4 },   // Port 2
   { SFP_STATUS_REG,  8 },   // Port 3
   { SFP_STATUS_REG, 12 },   // Port 4
   { SFP_STATUS_REG, 16 },   // Port 5
   { SFP_STATUS_REG, 20 },   // Port 6
   { SFP_STATUS_REG, 24 },   // Port 7
   { SFP_STATUS_REG, 28 },   // Port 8
};

acd_uint64_t HalSfpE5::s_status = 0;

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
HalSfpE5::HalSfpE5(const char*              a_name,
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
HalSfpE5::~HalSfpE5()
{
}

// ------------------------------------------------------------------------------------------------
/*!@brief Enable the SFP

   Power-up the SFP

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpE5::Enable()
{
   bool           bRet = true;
   acd_uint64_t   val64;
   acd_uint64_t   newVal;
   acd_uint64_t   mask;

   //HalDebug("Enable");
   if ( m_pIoDrv->Read(sfpEnMap[m_portId].regOffset, val64) )
   {
      newVal = val64;
      mask = (1 << sfpEnMap[m_portId].bitOffset);
      newVal &= ~mask;
      if ( newVal != val64 )
      {
         bRet &= m_pIoDrv->Write(sfpEnMap[m_portId].regOffset, newVal);
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
bool HalSfpE5::Disable()
{
   bool           bRet = true;
   acd_uint64_t   val64;
   acd_uint64_t   newVal;
   acd_uint64_t   mask;

   //HalDebug("Disable");
   if ( m_pIoDrv->Read(sfpEnMap[m_portId].regOffset, val64) )
   {
      newVal = val64;
      mask = (1 << sfpEnMap[m_portId].bitOffset);
      newVal |= mask;
      if ( newVal != val64 )
      {
         bRet &= m_pIoDrv->Write(sfpEnMap[m_portId].regOffset, newVal);
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
bool HalSfpE5::SetTxEnable(bool a_bEnable)
{
   bool           bRet = true;
   acd_uint64_t   val64;
   acd_uint64_t   newVal;
   acd_uint64_t   mask;

   //HalDebug("SetTxEnable(%d)", a_bEnable);
   if ( m_pIoDrv->Read(sfpTxDisMap[m_portId].regOffset, val64) )
   {
      newVal = val64;
      mask = (1 << sfpTxDisMap[m_portId].bitOffset);
      if ( a_bEnable )
      {
         newVal &= ~mask;
      }
      else
      {
         newVal |= mask;
      }
      if ( newVal != val64 )
      {
         bRet &= m_pIoDrv->Write(sfpTxDisMap[m_portId].regOffset, newVal);
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
bool HalSfpE5::IsPresent()
{
   acd_uint64_t   val64 = s_status;
   acd_uint64_t   mask;

   mask = (1 << sfpDetectMap[m_portId].bitOffset);
   m_isPresent = ((val64 & mask) == 0) ? true : false;
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
bool HalSfpE5::UpdateData()
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
bool HalSfpE5::UpdateMonitoringData()
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
bool HalSfpE5::RefreshStatus()
{
   return m_pIoDrv->Read(SFP_STATUS_REG, s_status);
}
