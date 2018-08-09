// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2012 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    HalSfpClipper.cpp
   @brief   Clipper SFP Hardware Abstraction Layer base class

   This file contains the Clipper SFP HAL class implementation

*/
#include "HalSfpClipper.h"
#include <accedian/acclib/acd_utils.h>

static const acd_uint32_t SFP_CTRL_1 = 0x500;
static const acd_uint32_t SFP_STAT_1 = 0x502;

// HAL_SFP_NB_PORT seems to include a dummy port 0???
#ifdef CLIPPER2
#define HAL_SFP_NB_PORT HalPortId9
#define HAL_SFP_PORT_ID_MIN HalPortId1
#define HAL_SFP_PORT_ID_MAX HalPortId8
#else // CLIPPER
#define HAL_SFP_NB_PORT HalPortId13
#define HAL_SFP_PORT_ID_MIN HalPortId1
#define HAL_SFP_PORT_ID_MAX HalPortId12
static const acd_uint32_t SFP_CTRL_2 = 0x501;
static const acd_uint32_t SFP_STAT_2 = 0x503;
#endif

struct sfpRegMap
{
   acd_uint32_t   regOffset;
   acd_uint32_t   bitOffset;
};

static sfpRegMap  sfpEnMap[HAL_SFP_NB_PORT] =
{
   { SFP_CTRL_1,  0 },   // Port 0
   { SFP_CTRL_1,  0 },   // Port 1
   { SFP_CTRL_1,  4 },   // Port 2
   { SFP_CTRL_1,  8 },   // Port 3
   { SFP_CTRL_1, 12 },   // Port 4
   { SFP_CTRL_1, 16 },   // Port 5
   { SFP_CTRL_1, 20 },   // Port 6
   { SFP_CTRL_1, 24 },   // Port 7
   { SFP_CTRL_1, 28 },   // Port 8
#ifdef CLIPPER
   { SFP_CTRL_2,  0 },   // Port 9
   { SFP_CTRL_2,  4 },   // Port 10
   { SFP_CTRL_2,  8 },   // Port 11
   { SFP_CTRL_2, 12 }    // Port 12
#endif
};

static sfpRegMap  sfpTxDisMap[HAL_SFP_NB_PORT] =
{
   { SFP_CTRL_1,  1 },   // Port 0
   { SFP_CTRL_1,  1 },   // Port 1
   { SFP_CTRL_1,  5 },   // Port 2
   { SFP_CTRL_1,  9 },   // Port 3
   { SFP_CTRL_1, 13 },   // Port 4
   { SFP_CTRL_1, 17 },   // Port 5
   { SFP_CTRL_1, 21 },   // Port 6
   { SFP_CTRL_1, 25 },   // Port 7
   { SFP_CTRL_1, 29 },   // Port 8
#ifdef CLIPPER
   { SFP_CTRL_2,  1 },   // Port 9
   { SFP_CTRL_2,  5 },   // Port 10
   { SFP_CTRL_2,  9 },   // Port 11
   { SFP_CTRL_2, 13 }    // Port 12
#endif
};

static sfpRegMap  sfpDetectMap[HAL_SFP_NB_PORT] =
{
   { SFP_STAT_1,  0 },   // Port 0
   { SFP_STAT_1,  1 },   // Port 1
   { SFP_STAT_1,  5 },   // Port 2
   { SFP_STAT_1,  9 },   // Port 3
   { SFP_STAT_1, 13 },   // Port 4
   { SFP_STAT_1, 17 },   // Port 5
   { SFP_STAT_1, 21 },   // Port 6
   { SFP_STAT_1, 25 },   // Port 7
   { SFP_STAT_1, 29 },   // Port 8
#ifdef CLIPPER
   { SFP_STAT_2,  1 },   // Port 9
   { SFP_STAT_2,  5 },   // Port 10
   { SFP_STAT_2,  9 },   // Port 11
   { SFP_STAT_2, 13 }    // Port 12
#endif
};

acd_uint64_t HalSfpClipper::s_status1 = 0;
#ifndef CLIPPER2
acd_uint64_t HalSfpClipper::s_status2 = 0;
#endif

// ================================================================================================
// ================================================================================================
//            PUBLIC CLASS SECTION
// ================================================================================================
// ================================================================================================
// ------------------------------------------------------------------------------------------------
/*!@brief Constructor

   @param [in]     a_name           : Instance name
   @param [in]     a_portId         : Port identifier
   @param [in]     a_pIoDrv         : FPGA I/O driver
   @param [in]     a_pI2cIoDrv      : I2C I/O driver
   @param [in]     a_defaultSpeed   : Default SFP speed
*/
// ------------------------------------------------------------------------------------------------
HalSfpClipper::HalSfpClipper(const char*     a_name,
                   HalPortId&                a_portId,
                   BaseIoDrv<acd_uint64_t>*  a_pIoDrv,
                   BaseIoDrv<acd_uint8_t>*   a_pI2cIoDrv,
                   HalSfpSpeed               a_defaultSpeed) :
HalSfp(a_name, a_defaultSpeed),
m_portId(a_portId),
m_pIoDrv(a_pIoDrv),
m_pI2cIoDrv(a_pI2cIoDrv)
{
   HalSetDebug(false);

   if ( (a_portId < HAL_SFP_PORT_ID_MIN) || (a_portId > HAL_SFP_PORT_ID_MAX) )
   {
      HalError("Invalid port id %d for HalSfpClipper", a_portId);
      throw(0);
   }
}

// ------------------------------------------------------------------------------------------------
/*!@brief Destructor

*/
// ------------------------------------------------------------------------------------------------
HalSfpClipper::~HalSfpClipper()
{
}

// ------------------------------------------------------------------------------------------------
/*!@brief Enable the SFP

   Power-up the SFP

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpClipper::Enable()
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
         acd_usleep(5000);   // 5 msec delay to reduce the power supply demand at startup
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
bool HalSfpClipper::Disable()
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
bool HalSfpClipper::SetTxEnable(bool a_bEnable)
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
bool HalSfpClipper::IsPresent()
{
   acd_uint64_t   val64;
   acd_uint64_t   mask;

#ifdef CLIPPER2
   val64 = s_status1;
#else // CLIPPER
   if ( m_portId < HalPortId9 )
   {
      val64 = s_status1;
   }
   else
   {
      val64 = s_status2;
   }
#endif

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
bool HalSfpClipper::UpdateData()
{
   acd_uint8_t buffer[128];
   acd_uint8_t ff[128];
   acd_uint8_t zero[128];
   bool bRet = false;

   if ( !m_isPresent || !m_bEnable )
   {
      return false;
   }

   memset(ff, 0xff, sizeof(ff));
   memset(zero, 0x00, sizeof(zero));
   //HalDebug("UpdateData");
   if ( m_pI2cIoDrv->Read(0xA0, sizeof(buffer), buffer) )
   {
      if ( memcmp(buffer, zero, sizeof(buffer)) == 0 )
      {
         HalDebug("Invalid data (0x00) read from EEPROM 0xA0");
      }
      else
      {
         memcpy(m_interfaceData, buffer, sizeof (buffer));
         bRet = HalSfp::UpdateData();
         if (!bRet)
         {
            //HalError("HalSfp::UpdateData() failed");
         }
      }
   }
   else
   {
      if ( ++m_logErrorCount < SFP_LOG_ERROR_THRESHOLD )
      {
         HalError("EEPROM 0xA0 read failed");
      }
   }

   if (m_bIsCopper)
   {
      if ( m_pI2cIoDrv->Read(0xAC, sizeof(buffer)/2, buffer) )
      {
         if ( memcmp(buffer, ff, sizeof(buffer)/2) == 0 )
         {
            HalDebug("Invalid data (0xff) read from EEPROM 0xAC");
         }
         else
         {
            memcpy(m_phyData, buffer, sizeof(buffer)/2);
         }
      }
      else
      {
         if ( ++m_logErrorCount < SFP_LOG_ERROR_THRESHOLD )
         {
            HalError("EEPROM 0xAC read failed");
         }
      }
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Update SFP monitoring data

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfpClipper::UpdateMonitoringData()
{
   bool        bRet = false;
   acd_uint8_t buffer[128];
   acd_uint8_t zero[128];

   if ( !m_isPresent || !m_bEnable )
   {
      return false;
   }
   //HalDebug("UpdateMonitoringData");

   memset(zero, 0x00, sizeof(zero));
   if ( m_pI2cIoDrv->Read(0xA2, sizeof(buffer), buffer) )
   {
      if ( memcmp(buffer, zero, sizeof(buffer)) == 0 )
      {
         HalDebug("Invalid data (0x00) read from EEPROM 0xA2");
      }
      else
      {
         memcpy(m_monData, buffer, sizeof (buffer));
         bRet = HalSfp::UpdateMonitoringData();
         if (!bRet)
         {
            //HalError("HalSfp::UpdateMonitoringData() failed");
         }
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
bool HalSfpClipper::RefreshStatus()
{
   bool bRet;

   bRet = m_pIoDrv->Read(SFP_STAT_1, s_status1);
#ifdef CLIPPER
   bRet = m_pIoDrv->Read(SFP_STAT_2, s_status2);
#endif

   return bRet;
}
