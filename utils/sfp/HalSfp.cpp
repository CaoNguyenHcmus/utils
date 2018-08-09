// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2012 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    HalSfp.cpp
   @brief   SFP Hardware Abstraction Layer base class

   This file contains the SFP HAL base class
   It regroups all the methods common to all derived HalSfp classes

*/
// ------------------------------------------------------------------------------------------------

#include "HalSfp.h"
#include <stdlib.h>
#include <arpa/inet.h>
#include <math.h>
#include "SfpDb.h"

//#define SFP_DEBUG

// ================================================================================================
// ================================================================================================
//            PUBLIC CLASS SECTION
// ================================================================================================
// ================================================================================================
// ------------------------------------------------------------------------------------------------
/*!@brief Constructor

   @param [in]     a_name           : Instance name
   @param [in]     a_defaultSpeed   : Default SFP speed
*/
// ------------------------------------------------------------------------------------------------
HalSfp::HalSfp(const char* a_name, HalSfpSpeed a_defaultSpeed) :
Hal(a_name),
m_bEnable(true),
m_bTxEnable(true),
m_isPresent(false),
m_mode(HalSfpModeUndefined),
m_defaultSpeed(a_defaultSpeed),
m_bIsCopper(false),
m_logErrorCount(0)
{
   HalSetDebug(false);
   memset(m_monData, 0, HAL_SFP_PAGE_SIZE);
   memset(m_interfaceData, 0, HAL_SFP_PAGE_SIZE);
   memset(m_phyData, 0, HAL_SFP_PAGE_SIZE);

   m_pHdr = (sfp_hdr_type*)m_interfaceData;
   m_pMon = (sfp_mon_type*)m_monData;

   memset(m_speedCap, 0, sizeof(m_speedCap));
}

// ------------------------------------------------------------------------------------------------
/*!@brief Destructor

*/
// ------------------------------------------------------------------------------------------------
HalSfp::~HalSfp()
{
}

// ------------------------------------------------------------------------------------------------
/*!@brief Enable the SFP

   Should put the SFP in a working condition ex: power on
   Implementation may vary depending on platform

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::Enable()
{
   m_bEnable = true;
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Disable the SFP

   Should put the SFP in a non-working condition ex: power off
   Implementation may vary depending on platform

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::Disable()
{
   m_bEnable = false;
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Qwery the SFP enable

   @return     true if SFP is enabled
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::IsEnabled()
{
   return m_bEnable;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Set the SFP Tx enable

   @param [in]     a_bEnable     : Flag to control the SFP Tx enable

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::SetTxEnable(bool a_bEnable)
{
   m_bTxEnable = a_bEnable;
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Qwery the SFP Tx enable

   @return     true if SFP Tx is enabled
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::IsTxEnabled()
{
   return m_bTxEnable;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Qwery the SFP type copper

   @return     true if SFP is a copper type
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::IsCopper()
{
   return m_bIsCopper;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Qwery the SFP type fiber

   @return     true if SFP is a fiber type
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::IsFiber()
{
   return !m_bIsCopper;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the connector field

   @param [out]     a_connector : Connector field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetConnector(acd_uint8_t& a_connector)
{
   if (m_bIsCopper)
   {
      a_connector = SFP_CONN_RJ45;
   }
   else
   {
      a_connector = m_pHdr->connector;
   }

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the wavelength field

   @param [out]     a_wavelength : Wavelength field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetWaveLength(acd_uint16_t& a_wavelength)
{
   a_wavelength = m_pHdr->wave_length;

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the vendor name field

   @param [in]     a_name : Vendor name field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetVendorName(char* a_name)
{
   memcpy(a_name, m_pHdr->vendor, sizeof(m_pHdr->vendor));

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the vendor OUI field

   @param [out]     a_oui : Vendor OUI field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetVendorOui(char* a_oui)
{
   memcpy(a_oui, m_pHdr->vendor_oui, sizeof(m_pHdr->vendor_oui));

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the vendor part number field

   @param [out]     a_pn : Vendor part number field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetVendorPartNumber(acd_uchar8_t* a_pn)
{
   memcpy(a_pn, m_pHdr->vendor_pn, sizeof(m_pHdr->vendor_pn));

   // Replace all spaces by NULL
   for(acd_uint32_t i = 0 ; i < sizeof(m_pHdr->vendor_pn) ; i++)
   {
      if ( a_pn[i] == ' ' )
      {
         a_pn[i] = '\0';
      }
   }
   // Add NULL caracter for safety
   *(a_pn + sizeof(m_pHdr->vendor_pn)) = '\0';
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the vendor revision field

   @param [out]     a_rev : Vendor revision field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetVendorRevision(char* a_rev)
{
   memcpy(a_rev, m_pHdr->vendor_rev, sizeof(m_pHdr->vendor_rev));

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the serial field

   @param [out]     a_serial : Serial field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetSerial(char* a_serial)
{
   memcpy(a_serial, m_pHdr->serial, sizeof(m_pHdr->serial));

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the date code field

   @param [out]     a_year    : Year
   @param [out]     a_month   : Month
   @param [out]     a_day     : Day
   @param [out]     a_lot     : Lot

   @return     <return-value>
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetDateCode(acd_uint16_t& a_year, acd_uint16_t& a_month, acd_uint16_t& a_day, acd_uint16_t& a_lot)
{
   char  str[3] = {0,0,0};

   memcpy(str, &m_pHdr->year, 2);
   a_year = atoi(str) + 2000;

   memcpy(str, &m_pHdr->month, 2);
   a_month = atoi(str);

   memcpy(str, &m_pHdr->day, 2);
   a_day = atoi(str);

   memcpy(str, &m_pHdr->lot, 2);
   a_lot = atoi(str);

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the extended type field

   @param [out]     a_type : Extended type field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetExtendedType(acd_uint8_t& a_type)
{
   a_type = m_pHdr->ext_id;

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the type field

   @param [out]     a_type : Type field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetType(acd_uint8_t& a_type)
{
   a_type = m_pHdr->id;

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the tranceiver code field

   @param [out]     a_code : Tranceiver code field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetTranceiverCode(char* a_code)
{
   memcpy(a_code, &m_pHdr->trans3, 8);

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Check if the SFP support the diagnostics

   @return     true if diagnostic is supported
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::IsDiagCapable()
{
   return (m_pHdr->diag & 0x40) != 0;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Check if the SFP is internally calibrated

   @return     true if the SFP is internally calibrated
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::IsInternallyCalibrated()
{
   return (m_pHdr->diag & 0x20) != 0 ;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Check if the SFP supports the alarms

   @return     true if the SFP supports the alarms
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::IsAlarmCapable()
{
   return (m_pHdr->enhance & 0x80) != 0;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the diagnostic monitoring revision field

   @param [out]     a_rev : Diagnostic monitoring revision field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetDiagMonRev(acd_uint8_t& a_rev)
{
   a_rev = m_pHdr->rev_8472;

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the bias field

   @param [out]     a_bias : Bias field

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetBias(acd_uint32_t& a_bias)
{
   a_bias = convertBias( ntohs(m_pMon->bias) );

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief This function is used to provide the raw data form the EEPROM

        – Address A0h: Interface ID memory.
        – Address A2h: Diagnostic information memory.
        – Address ACh: Copper PHY config memory.

   @param [in]     a_region   : Memory region
   @param [out]    a_memory   : Output buffer
   @param [in]     a_size     : Number of bytes to get

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetMemory(acd_uint32_t a_region, acd_uint8_t* a_memory, acd_uint32_t a_size)
{
   bool bRet = false;

   if (a_region == 0xA0)
   {
      memcpy(a_memory, m_interfaceData, a_size);
      bRet = true;
   }
   else if (a_region == 0xA2)
   {
      memcpy(a_memory, m_monData, a_size);
      bRet = true;
   }
   else if (a_region == 0xAC)
   {
      memcpy(a_memory, m_phyData, a_size);
      bRet = true;
   }
   else
   {
      memset(a_memory, 0xEE, a_size);
   }

   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the rx power

   @param [out]     a_pwr : Rx power

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetRxPower(acd_uint32_t& a_pwr)
{
   a_pwr = convertRxPower( ntohs(m_pMon->rx_pwr) );
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the temperature

   @param [out]     a_temp : Temperature

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetTemperature(acd_int16_t& a_temp)
{
   a_temp = convertTemp( ntohs(m_pMon->temp) );
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the tx power

   @param [out]     a_pwr : Tx power

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetTxPower(acd_uint32_t& a_pwr)
{
   a_pwr = convertTxPower( ntohs(m_pMon->tx_pwr) );
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the voltage

   @param [out]     a_vcc : Voltage

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetVoltage(acd_uint16_t& a_vcc)
{
   a_vcc = convertVoltage( ntohs(m_pMon->vcc) );
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the temperature threshold

   @param [in]    a_id     : Temperature threshold identifier
   @param [out]   a_temp   : Temperature threshold

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetTemperatureThreshold(HalSfpThresholdId a_id, acd_int16_t& a_temp)
{
   bool bRet = true;

   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         a_temp = convertTemp( ntohs(m_pMon->ts_high_alm) );
         break;
      case HalSfpThresholdLowAlarm:
         a_temp = convertTemp( ntohs(m_pMon->ts_low_alm) );
         break;
      case HalSfpThresholdHighWarning:
         a_temp = convertTemp( ntohs(m_pMon->ts_high_warn) );
         break;
      case HalSfpThresholdLowWarning:
         a_temp = convertTemp( ntohs(m_pMon->ts_low_warn) );
         break;
      default:
         bRet = false;
         break;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the rx power threshold

   @param [in]    a_id  : Rx power threshold identifier
   @param [out]   a_pwr : Rx power threshold

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetRxPowerThreshold(HalSfpThresholdId a_id, acd_uint32_t& a_pwr)
{
   bool bRet = true;

   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         a_pwr = convertRxPower( ntohs(m_pMon->rx_pwr_high_alm) );
         break;
      case HalSfpThresholdLowAlarm:
         a_pwr = convertRxPower( ntohs(m_pMon->rx_pwr_low_alm) );
         break;
      case HalSfpThresholdHighWarning:
         a_pwr = convertRxPower( ntohs(m_pMon->rx_pwr_high_warn) );
         break;
      case HalSfpThresholdLowWarning:
         a_pwr = convertRxPower( ntohs(m_pMon->rx_pwr_low_warn) );
         break;
      default:
         bRet = false;
         break;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the tx power threshold

   @param [in]    a_id  : Tx power threshold identifier
   @param [out]   a_pwr : Tx power threshold

   @return     <return-value>
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetTxPowerThreshold(HalSfpThresholdId a_id, acd_uint32_t& a_pwr)
{
   bool bRet = true;

   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         a_pwr = convertTxPower( ntohs(m_pMon->tx_pwr_high_alm) );
         break;
      case HalSfpThresholdLowAlarm:
         a_pwr = convertTxPower( ntohs(m_pMon->tx_pwr_low_alm) );
         break;
      case HalSfpThresholdHighWarning:
         a_pwr = convertTxPower( ntohs(m_pMon->tx_pwr_high_warn) );
         break;
      case HalSfpThresholdLowWarning:
         a_pwr = convertTxPower( ntohs(m_pMon->tx_pwr_low_warn) );
         break;
      default:
         bRet = false;
         break;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the voltage threshold

   @param [in]    a_id  : Voltage threshold identifier
   @param [out]   a_vcc : Voltage threshold

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetVoltageThreshold(HalSfpThresholdId a_id, acd_uint16_t& a_vcc)
{
   bool bRet = true;

   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         a_vcc = convertVoltage( ntohs(m_pMon->vcc_high_alm) );
         break;
      case HalSfpThresholdLowAlarm:
         a_vcc = convertVoltage( ntohs(m_pMon->vcc_low_alm) );
         break;
      case HalSfpThresholdHighWarning:
         a_vcc = convertVoltage( ntohs(m_pMon->vcc_high_warn) );
         break;
      case HalSfpThresholdLowWarning:
         a_vcc = convertVoltage( ntohs(m_pMon->vcc_low_warn) );
         break;
      default:
         bRet = false;
         break;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the bias threshold

   @param [in]    a_id     : Bias threshold identifier
   @param [out]   a_bias   : Bias threshold

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetBiasThreshold(HalSfpThresholdId a_id, acd_uint32_t& a_bias)
{
   bool bRet = true;

   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         a_bias = convertBias( ntohs(m_pMon->lbc_high_alm) );
         break;
      case HalSfpThresholdLowAlarm:
         a_bias = convertBias( ntohs(m_pMon->lbc_low_alm) );
         break;
      case HalSfpThresholdHighWarning:
         a_bias = convertBias( ntohs(m_pMon->lbc_high_warn) );
         break;
      case HalSfpThresholdLowWarning:
         a_bias = convertBias( ntohs(m_pMon->lbc_low_warn) );
         break;
      default:
         bRet = false;
         break;
   }
   //HalDebug("GetBiasThreshold(%d, %d) = %d", id, bias, bRet);
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Set the temperature threshold

   @param [in]    a_id     : Temperature threshold identifier
   @param [in]    a_temp   : Temperature threshold

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::SetTemperatureThreshold(HalSfpThresholdId a_id, acd_int16_t& a_temp)
{
   bool bRet = true;

   // TODO: Save temperature threshold
   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         break;
      case HalSfpThresholdLowAlarm:
         break;
      case HalSfpThresholdHighWarning:
         break;
      case HalSfpThresholdLowWarning:
         break;
      default:
         bRet = false;
         break;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Set the rx power threshold

   @param [in]    a_id  : Rx power threshold identifier
   @param [in]    a_pwr : Rx power threshold

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::SetRxPowerThreshold(HalSfpThresholdId a_id, acd_uint32_t& a_pwr)
{
   bool bRet = true;

   // TODO: Save rx power threshold
   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         break;
      case HalSfpThresholdLowAlarm:
         break;
      case HalSfpThresholdHighWarning:
         break;
      case HalSfpThresholdLowWarning:
         break;
      default:
         bRet = false;
         break;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Set the tx power threshold

   @param [in]     a_id  : Tx power threshold identifier
   @param [in]     a_pwr : Tx power threshold

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::SetTxPowerThreshold(HalSfpThresholdId a_id, acd_uint32_t& a_pwr)
{
   bool bRet = true;

   // TODO: Save tx power threshold
   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         break;
      case HalSfpThresholdLowAlarm:
         break;
      case HalSfpThresholdHighWarning:
         break;
      case HalSfpThresholdLowWarning:
         break;
      default:
         bRet = false;
         break;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Set the voltage threshold

   @param [in]     a_id    : Voltage threshold identifier
   @param [in]     a_vcc   : Voltage threshold

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::SetVoltageThreshold(HalSfpThresholdId a_id, acd_uint16_t& a_vcc)
{
   bool bRet = true;

   // TODO: Save voltage threshold
   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         break;
      case HalSfpThresholdLowAlarm:
         break;
      case HalSfpThresholdHighWarning:
         break;
      case HalSfpThresholdLowWarning:
         break;
      default:
         bRet = false;
         break;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Set the bias threshold

   @param [in]     a_id    : Bias threshold identifier
   @param [in]     a_bias  : Bias threshold

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::SetBiasThreshold(HalSfpThresholdId a_id, acd_uint32_t& a_bias)
{
   bool bRet = true;

   // TODO: Save bias threshold
   switch(a_id)
   {
      case HalSfpThresholdHighAlarm:
         break;
      case HalSfpThresholdLowAlarm:
         break;
      case HalSfpThresholdHighWarning:
         break;
      case HalSfpThresholdLowWarning:
         break;
      default:
         bRet = false;
         break;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Check SFP presence

   @return     true if SFP is present
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::IsPresent()
{
   return m_isPresent;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the length in meters

   @param [out] a_length : Length in meters

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetLength(acd_uint32_t& a_length)
{
   bool bRet = true;

   if (m_pHdr->length9mkm != 0)
   {
      // Link length supported for 9/125 mm fiber, units of km
      a_length = m_pHdr->length9mkm * 1000;
   }
   else if (m_pHdr->length9m != 0)
   {
      // Length (9m) Link length supported for 9/125 mm fiber, units of 100 m
      a_length = m_pHdr->length9m * 100;
   }
   else if (m_pHdr->length50 != 0)
   {
      // Link length supported for 50/125 mm fiber, units of 10 m
      a_length = m_pHdr->length50 * 10;
   }
   else if (m_pHdr->length625 != 0)
   {
      // Link length supported for 62.5/125 mm fiber, units of 10 m
      a_length = m_pHdr->length625 * 10;
   }
   else if (m_pHdr->length_copper != 0)
   {
      // Length (Copper) Link length supported for copper, units of meters
      a_length = m_pHdr->length_copper;
   }
   else
   {
      bRet = false;
      a_length = 0;
   }

   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Update SFP data

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::UpdateData()
{
   if ( !checkCodeBase(m_interfaceData) || !checkCodeExt(m_interfaceData) )
   {
      m_pLogger->LogDebug("SFP 0xA0 checksum failed");
      return false;
   }

   // Shall be implemented in a derived class
   return updateSpeedCap();
}

// ------------------------------------------------------------------------------------------------
/*!@brief Update SFP monitoring data

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::UpdateMonitoringData()
{
   if ( !checkCodeDmi(m_monData) )
   {
      m_pLogger->LogDebug("SFP 0xA2 checksum failed");
      return false;
   }
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Convert the temperature

   @param [in]     a_tsAd : Analog to digital converter value

   @return     Temperature
*/
// ------------------------------------------------------------------------------------------------
acd_int16_t HalSfp::convertTemp(acd_int16_t a_tsAd)
{
   acd_int16_t    ts_offset;
   acd_uint16_t   ts_slope;
   acd_int16_t    ts;
   float          slope;

   if ( IsInternallyCalibrated() )
   {
      ts = (a_tsAd/256);
      //HalDebug("internal ts: %d", ts);
   }
   else
   {
      ts_slope = ntohs(m_pMon->ts_slope);
      ts_offset = ntohs(m_pMon->ts_offset);

      slope = (ts_slope&0x00FF);
      slope = slope/256;
      slope += ts_slope>>8;

      ts = (acd_int16_t)((a_tsAd*slope + ts_offset)/256);
      //HalDebug("external ts_slope: %f ts_offset: %d", slope, ts_offset);
      //HalDebug("external ts: %d", ts);
   }
   return ts;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Convert the voltage

   @param [in]     a_vccAd : Voltage analog to digital converter value

   @return     Voltage
*/
// ------------------------------------------------------------------------------------------------
acd_uint16_t HalSfp::convertVoltage(acd_uint16_t a_vccAd)
{
   acd_int16_t    vcc_offset;
   acd_uint16_t   vcc_slope;
   acd_uint16_t   vcc;
   float          slope;

   if ( IsInternallyCalibrated() )
   {
      vcc = a_vccAd/10;
      //HalDebug("internal vcc: %d", vcc);
   }
   else
   {
      vcc_slope = ntohs(m_pMon->vcc_slope);
      vcc_offset = ntohs(m_pMon->vcc_offset);
      slope = (vcc_slope&0x00FF)/256 + (vcc_slope>>8);
      vcc = (acd_uint16_t)((a_vccAd*slope + vcc_offset)/10);
      //HalDebug("external vcc_slope: %f vcc_offset: %d vcc:", slope, vcc_offset, vcc);
   }

   return vcc;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Convert the bias

   @param [in]     a_lbcAd : Bias analog to digital converter value

   @return     Bias
*/
// ------------------------------------------------------------------------------------------------
acd_uint32_t HalSfp::convertBias(acd_uint16_t a_lbcAd)
{
   acd_int16_t    lbc_offset;
   acd_uint16_t   lbc_slope;
   acd_uint32_t   lbc;
   float          slope;

   if ( IsInternallyCalibrated() )
   {
      lbc = (a_lbcAd*2);
      //HalDebug("internal lbc: %d", lbc);
   }
   else
   {
      lbc_slope = ntohs(m_pMon->lbc_slope);
      lbc_offset = ntohs(m_pMon->lbc_offset);
      slope = ((float)(lbc_slope&0x00FF) / 256.0) + (float)(lbc_slope>>8);
      lbc = (acd_uint32_t)((a_lbcAd*slope + lbc_offset)*2);
      //HalDebug("external lbc_slope: %f lbc_offset: %d lbc: %d", slope, lbc_offset, lbc);
   }

   return lbc;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Convert the tx power

   @param [in]     a_txPwrAd : Tx power analog to digital converter value

   @return     Tx power
*/
// ------------------------------------------------------------------------------------------------
acd_uint32_t HalSfp::convertTxPower(acd_uint16_t a_txPwrAd)
{
   acd_int16_t    tx_offset;
   acd_uint16_t   tx_slope;
   acd_uint32_t   tx_pwr;
   float          slope;

   if ( IsInternallyCalibrated() )
   {
        //HalDebug("internal rx_pwr: %d", a_txPwrAd);
        tx_pwr = a_txPwrAd;
   }
   else
   {
      tx_slope = ntohs(m_pMon->tx_pwr_slope);
      tx_offset = ntohs(m_pMon->tx_pwr_offset);
      slope = ((float)(tx_slope&0x00FF) / 256.0) + (float)(tx_slope>>8);
      tx_pwr = (acd_uint32_t)(a_txPwrAd*slope + tx_offset);
      //HalDebug("external tx_slope: %f tx_offset: %d tx_pwr: %d", slope, tx_offset, tx_pwr);
   }

   return tx_pwr;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Convert the rx power

   @param [in]     a_rxPwrAd : Rx power analog to digital converter value

   @return     Rx power
*/
// ------------------------------------------------------------------------------------------------
acd_uint32_t HalSfp::convertRxPower(acd_uint16_t a_rxPwrAd)
{
   acd_uint16_t   rx_pwr;
   float          rx_pwr4;
   float          rx_pwr3;
   float          rx_pwr2;
   float          rx_pwr1;
   float          rx_pwr0;
   float          rx_ad;
   float          mantissa;
   acd_int32_t    exp;
   acd_uint32_t   pwr;

   if ( IsInternallyCalibrated() )
   {
      //HalDebug("internal rx_pwr: %d", a_rxPwrAd);
      rx_pwr = a_rxPwrAd;
   }
   else
   {
      /* Transform each IEEE-754 float in C float */
      pwr = ntohl(m_pMon->rx_pwr4);
      mantissa = pwr&0x7FFFFF;
      mantissa = (mantissa/0x7FFFFF)+1;
      exp = ((pwr&0x7F800000)>>23)-127;
      rx_pwr4 = ldexpf(mantissa, exp);
      if ((pwr&0x80000000) != 0) rx_pwr4 = -rx_pwr4;

      pwr = ntohl(m_pMon->rx_pwr3);
      mantissa = pwr&0x7FFFFF;
      mantissa = (mantissa/0x7FFFFF)+1;
      exp = ((pwr&0x7F800000)>>23)-127;
      rx_pwr3 = ldexpf(mantissa, exp);
      if ((pwr&0x80000000) != 0) rx_pwr3 = -rx_pwr3;

      pwr = ntohl(m_pMon->rx_pwr2);
      mantissa = pwr&0x7FFFFF;
      mantissa = (mantissa/0x7FFFFF)+1;
      exp = ((pwr&0x7F800000)>>23)-127;
      rx_pwr2 = ldexpf(mantissa, exp);
      if ((pwr&0x80000000) != 0) rx_pwr2 = -rx_pwr2;

      pwr = ntohl(m_pMon->rx_pwr1);
      mantissa = pwr&0x7FFFFF;
      mantissa = (mantissa/0x7FFFFF)+1;
      exp = ((pwr&0x7F800000)>>23)-127;
      rx_pwr1 = ldexpf(mantissa, exp);
      if ((pwr&0x80000000) != 0) rx_pwr4 = -rx_pwr1;

      pwr = ntohl(m_pMon->rx_pwr0);
      mantissa = pwr&0x7FFFFF;
      mantissa = (mantissa/0x7FFFFF)+1;
      exp = ((pwr&0x7F800000)>>23)-127;
      rx_pwr0 = ldexpf(mantissa, exp);
      if ((pwr&0x80000000) != 0) rx_pwr0 = -rx_pwr0;

#ifdef SFP_DEBUG
      HalDebug("rx_pwr4: %f", rx_pwr4);
      HalDebug("rx_pwr3: %f", rx_pwr3);
      HalDebug("rx_pwr2: %f", rx_pwr2);
      HalDebug("rx_pwr1: %f", rx_pwr1);
      HalDebug("rx_pwr0: %f", rx_pwr0);
#endif
      rx_ad = a_rxPwrAd;
#ifdef SFP_DEBUG
      HalDebug("rx_pwr_ad: %d (0x%04x)", rx_pwr_ad, a_rxPwrAd);
      HalDebug("rx_ad: %f", rx_ad);
#endif
      pwr = (acd_uint32_t)(rx_pwr4*powf(rx_ad, 4) + \
                           rx_pwr3*powf(rx_ad, 3) + \
                           rx_pwr2*powf(rx_ad, 2) + \
                           rx_pwr1*rx_ad + \
                           rx_pwr0);
#ifdef SFP_DEBUG
      HalDebug("pwr: %d, built with:\n", pwr);
      HalDebug("rx_pwr4*powf(rx_ad, 4): %f", rx_pwr4*powf(rx_ad, 4));
      HalDebug("rx_pwr3*powf(rx_ad, 3): %f", rx_pwr3*powf(rx_ad, 3));
      HalDebug("rx_pwr2*powf(rx_ad, 2): %f", rx_pwr2*powf(rx_ad, 2));
      HalDebug("rx_pwr1*powf(rx_ad, 2): %f", rx_pwr1*rx_ad);
      HalDebug("rx_pwr0: %f\n", rx_pwr0);
#endif
      // This check fixes ticket #1491
      if ((pwr&0x80000000) == 0)
      {
         rx_pwr = pwr;
      }
      else
      {
         rx_pwr = 0;
      }
      //HalDebug("external rx_pwr: %d", rx_pwr);
    }

   return rx_pwr;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the SFP speed capability

   @param [in]     a_speed : SFP speed capability

   @return     true if the given speed is supported
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetSpeedCapability(HalSfpSpeed a_speed)
{
   bool bRet = false;

   if ( a_speed < HalSfpSeedMax )
      bRet = m_speedCap[a_speed];

   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the SFP default speed

   @return     Default SFP speed
*/
// ------------------------------------------------------------------------------------------------
HalSfpSpeed HalSfp::GetDefaultSpeed()
{
   return m_defaultSpeed;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Update the SFP speed capabilities

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::updateSpeedCap()
{
   bool bRet = true;

   // Reset speed capabilities
   memset(m_speedCap, 0, sizeof(m_speedCap));
   m_bIsCopper = false;

#if 0
   // NOTE: Trash Debug - Some variables might not be well represented
   HalDebug("SFP HDR: id(%d)-ext_id(%d)-connector(%d)-trans3(%d)-trans4(%d)-trans5(%d)",
         m_pHdr->id, m_pHdr->ext_id, m_pHdr->connector, m_pHdr->trans3, m_pHdr->trans4, m_pHdr->trans5);
   HalDebug("SFP HDR: trans6(%d)-trans7(%d)-trans8(%d)-trans9(%d)-trans10(%d)-encoding(%d)-bit_rate(%d)",
         m_pHdr->trans6,m_pHdr->trans7,m_pHdr->trans8,m_pHdr->trans9,m_pHdr->trans10,m_pHdr->encoding,m_pHdr->bit_rate);
   HalDebug("SFP HDR: rsv1(%d)-length9mkm(%d)-length9m(%d)-length50(%d)-length625(%d)-length_copper(%d)",
         m_pHdr->rsv1, m_pHdr->length9mkm,m_pHdr->length9m,m_pHdr->length50,m_pHdr->length625, m_pHdr->length_copper);
   HalDebug("SFP HDR: rsv2(%d)-vendor[16](%s)-transceiver(%d)-vendor_oui[3](%d%d%d)-vendor_pn[16](%s)-vendor_rev[4](%s)",
         m_pHdr->rsv2, m_pHdr->vendor,m_pHdr->transceiver, m_pHdr->vendor_oui[2],m_pHdr->vendor_oui[1],
         m_pHdr->vendor_oui[0],m_pHdr->vendor_pn, m_pHdr->vendor_rev);
   HalDebug("SFP HDR: wave_length(%d)-rsv4(%d)-cc_base(%d)-option1(%d)-option2(%d)-br_max(%d)-br_min(%d)",
         m_pHdr->wave_length, m_pHdr->rsv4, m_pHdr->cc_base, m_pHdr->option1, m_pHdr->option2,
         m_pHdr->br_max,m_pHdr->br_min);
   HalDebug("SFP HDR: serial[16](%s)-year(%d)-month(%d)-day(%d)-lot(%d)-diag(%d)-enhance(%d)-rev_8472(%d)",
         m_pHdr->serial,m_pHdr->year,m_pHdr->month,m_pHdr->day,m_pHdr->lot,m_pHdr->diag,
         m_pHdr->enhance,m_pHdr->rev_8472);
#endif

   //
   // Try to find part number from the SFP database first
   //
   if ( updateSpeedCapFromDb() )
   {
      bRet = true;
   }
   else
   {
      // 10G bit rate
      if ( (m_pHdr->trans3 & ETH_10GBASE_XX) || (m_pHdr->bit_rate >= 0x64) )
      {
         m_speedCap[HalSfpSpeed10G] = true;
         bRet = true;
      }
      // 1G bit rate
      if (((m_pHdr->trans6&ETH_1000BASE_SX) != 0) ||
          ((m_pHdr->trans6&ETH_1000BASE_LX) != 0) ||
         (((m_pHdr->trans6&ETH_BASE_BX) != 0) && (m_pHdr->bit_rate == 0x0A)) ||
         (((m_pHdr->trans6&ETH_BASE_BX) != 0) && (m_pHdr->bit_rate == 0x0C)) ||
         (((m_pHdr->trans6&ETH_BASE_BX) != 0) && (m_pHdr->bit_rate == 0x0D)))
      {
         m_speedCap[HalSfpSpeed1G] = true;
         bRet = true;
      }
      if ((m_pHdr->trans6&ETH_1000BASE_T) != 0)
      {
         m_speedCap[HalSfpSpeed1G] = true;
         m_bIsCopper = true;
         bRet = true;
      }

      // Sonet/Ethernet Compliance codes
      if (((m_pHdr->trans5&SONET_OC3_SR) != 0) ||
          ((m_pHdr->trans5&SONET_OC3_IR) != 0) ||
          ((m_pHdr->trans5&SONET_OC3_LR) != 0) ||
          ((m_pHdr->trans6&ETH_100BASE_FX) != 0) ||
          ((m_pHdr->trans6&ETH_100BASE_LX) != 0) ||
          (((m_pHdr->trans6&ETH_BASE_BX) != 0) && (m_pHdr->bit_rate == 0x01)) ||
          (((m_pHdr->encoding&SFP_ENCODE_4B5B) != 0) && (m_pHdr->bit_rate == 0x01)))
      {
         m_speedCap[HalSfpSpeed100M] = true;
         bRet = true;
      }
   }

   if (!m_speedCap[HalSfpSpeed10M]  &&
       !m_speedCap[HalSfpSpeed100M] &&
       !m_speedCap[HalSfpSpeed1G]   &&
       !m_speedCap[HalSfpSpeed10G])
   {
      HalError("No speed detected on SFP");
   }
   else
   {
#if 0
      HalDebug("SFP speed detected is 10M(%d)-100M(%d)-1G(%d)-10G(%d)-Copper(%d)",
               m_speedCap[HalSfpSpeed10M],
               m_speedCap[HalSfpSpeed100M],
               m_speedCap[HalSfpSpeed1G],
               m_speedCap[HalSfpSpeed10G],
               m_bIsCopper);
#endif
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Update the speed capability from the SFP database

   @return     true if match found
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::updateSpeedCapFromDb()
{
   bool bRet = false;
   SfpDb* pSfpDb = SfpDb::GetInstance();

   if ( pSfpDb == NULL )
   {
      HalError("Sfp database not found");
      return false;
   }

   // Get valid vendor pn string
   acd_uchar8_t vendorPnStr[sizeof(m_pHdr->vendor_pn) + 1];
   GetVendorPartNumber(vendorPnStr);
   //HalDebug("Vendor PN: %s", vendorPnStr);

   SfpDb::SfpDesc sfpDesc;
   string sPn((const char*)vendorPnStr);
   if ( pSfpDb->Get(sPn, sfpDesc) )
   {
      //HalNotice("Found SFP %s in SFP database", sPn.c_str());
      m_speedCap[HalSfpSpeed10M]    = sfpDesc.Is10MCapable();
      m_speedCap[HalSfpSpeed100M]   = sfpDesc.Is100MCapable();
      m_speedCap[HalSfpSpeed1G]     = sfpDesc.Is1GCapable();
      m_speedCap[HalSfpSpeed10G]    = sfpDesc.Is10GCapable();
      m_bIsCopper                   = sfpDesc.IsTypeCopper();
      bRet = true;
   }
   else
   {
      //HalNotice("SFP %s was not found in SFP database", sPn.c_str());
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Set the SFP mode

   @param [in]     a_mode     : SFP mode

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::SetMode(HalSfpMode a_mode)
{
   m_mode = a_mode;
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the SFP mode

   @param [out]     a_mode     : SFP mode

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::GetMode(HalSfpMode& a_mode)
{
   m_mode = a_mode;
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Validate the base check code

   CC_BASE [Address A0h, Byte 63]
   0 to byte 62 inclusive

   @param [out]     a_pBuf     : SFP data 0xA0

   @return     true if base check code is valid
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::checkCodeBase(acd_uint8_t* a_pBuf)
{
   acd_uint8_t cc = 0;

   for(acd_uint32_t i = 0 ; i < HAL_SFP_CC_BASE ; i++)
   {
      cc += a_pBuf[i];
   }
   if ( cc != a_pBuf[HAL_SFP_CC_BASE] )
   {
      //m_pLogger->LogDebug("SFP base check code failed");
   }
   return (cc == a_pBuf[HAL_SFP_CC_BASE]);
}

// ------------------------------------------------------------------------------------------------
/*!@brief Validate the extended check code

   CC_EXT [Address A0h, Byte 95]
   64 to byte 94 inclusive

   @param [out]     a_pBuf     : SFP data 0xA0

   @return     true if extended check code is valid
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::checkCodeExt(acd_uint8_t* a_pBuf)
{
   acd_uint8_t cc = 0;

   for(acd_uint32_t i = HAL_SFP_CC_BASE+1 ; i < HAL_SFP_CC_EXT ; i++)
   {
      cc += a_pBuf[i];
   }
   if ( cc != a_pBuf[HAL_SFP_CC_EXT] )
   {
      //m_pLogger->LogDebug("SFP extended check code failed");
   }
   return (cc == a_pBuf[HAL_SFP_CC_EXT]);
}

// ------------------------------------------------------------------------------------------------
/*!@brief Validate the diagnostic management interface check code

   CC_DMI [Address A2h, Byte 95]
   0 to byte 94, inclusive.

   @param [out]     a_pBuf     : SFP data 0xA0

   @return     true if diagnostic management interface check code is valid
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::checkCodeDmi(acd_uint8_t* a_pBuf)
{
   acd_uint8_t cc = 0;

   for(acd_uint32_t i = 0 ; i < HAL_SFP_CC_DMI ; i++)
   {
      cc += a_pBuf[i];
   }
   if ( cc != a_pBuf[HAL_SFP_CC_DMI] )
   {
      //m_pLogger->LogDebug("SFP DMI check code failed -- Computed %02xh  Expected %02xh", cc, a_pBuf[HAL_SFP_CC_DMI]);
   }
   return (cc == a_pBuf[HAL_SFP_CC_DMI]);
}

// ------------------------------------------------------------------------------------------------
/*!@brief Refresh status

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool HalSfp::RefreshStatus()
{
   return true;
}
