// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2012 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    SfpSfp.cpp
   @brief   SFP database class

   This file contains the SFP database class

*/
// ------------------------------------------------------------------------------------------------

#include "SfpDb.h"
#include <stdio.h>

SfpDb* SfpDb::s_pTheInstance = NULL;

// ================================================================================================
// ================================================================================================
//            PUBLIC CLASS SECTION
// ================================================================================================
// ================================================================================================
// ------------------------------------------------------------------------------------------------
/*!@brief Constructor

*/
// ------------------------------------------------------------------------------------------------
SfpDb::SfpDb()
{
}

// ------------------------------------------------------------------------------------------------
/*!@brief Destructor

*/
// ------------------------------------------------------------------------------------------------
SfpDb::~SfpDb()
{
   m_sfpMap.clear();
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get the SFP database instance

*/
// ------------------------------------------------------------------------------------------------
SfpDb* SfpDb::GetInstance()
{
   if ( s_pTheInstance == NULL )
   {
      s_pTheInstance = new SfpDb;
   }
   return s_pTheInstance;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Get a SFP descriptor

   @param [in]      a_partNumber : SFP part number
   @param [out]     a_sfpDesc    : SFP descriptor

   @return     true if found
*/
// ------------------------------------------------------------------------------------------------
bool SfpDb::Get(string& a_partNumber, SfpDesc& a_sfpDesc)
{
   bool bRet = false;
   if ( m_sfpMap.find(a_partNumber) != m_sfpMap.end() )
   {
      a_sfpDesc = m_sfpMap[a_partNumber];
      bRet = true;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Load the SFP database from the persistent storage

   Current implementation does not use an external file
   This method inserts the known SFP into the database
   Future implementation would read the SFP data from a file

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool SfpDb::Load()
{
   string   partNum;
   SfpDesc  sfpDesc;

   // Agilent 1000Mbps SFP copper
   partNum = "HBCU-5710R";
   SfpDesc desc1(false, false, true, false, false, 0, 0);
   Insert(partNum, desc1);

   // Agilestar 1000Mbps SFP copper
   partNum = "AS-1TX";
   SfpDesc desc2(false, false, true, false, false, 0, 0);
   Insert(partNum, desc2);

   // Finisar 1000Mbps SFP copper
   partNum = "FCLF8521P2BTL";
   SfpDesc desc3(false, false, true, false, false, 0, 0);
   Insert(partNum, desc3);

   // Support "FWLF-1621-" serie
   partNum = "FWDM-16217D47";
   SfpDesc desc4(false, false, true, false, true, 1470, 1470);
   Insert(partNum, desc4);
   partNum = "FWDM-16217D49";
   SfpDesc desc5(false, false, true, false, true, 1490, 1490);
   Insert(partNum, desc5);
   partNum = "FWDM-16217D51";
   SfpDesc desc6(false, false, true, false, true, 1510, 1510);
   Insert(partNum, desc6);
   partNum = "FWDM-16217D53";
   SfpDesc desc7(false, false, true, false, true, 1530, 1530);
   Insert(partNum, desc7);
   partNum = "FWDM-16217D55";
   SfpDesc desc8(false, false, true, false, true, 1550, 1550);
   Insert(partNum, desc8);
   partNum = "FWDM-16217D57";
   SfpDesc desc9(false, false, true, false, true, 1570, 1570);
   Insert(partNum, desc9);
   partNum = "FWDM-16217D59";
   SfpDesc desc10(false, false, true, false, true, 1590, 1590);
   Insert(partNum, desc10);
   partNum = "FWDM-16217D61";
   SfpDesc desc11(false, false, true, false, true, 1610, 1610);
   Insert(partNum, desc11);

   // Support "FWLF-1631-" serie
   partNum = "FWLF-1631-";
   SfpDesc desc12(false, false, true, false, true, 0, 0);
   Insert(partNum, desc12);

   // Support FWLF163131-CY
   partNum = "FWLF163131-CY";
   SfpDesc desc12b(false, false, true, false, true, 1552, 1552);
   Insert(partNum, desc12b);

   partNum = "MiRICi-FE-T3";
   SfpDesc desc13(false, true, false, false, true, 0, 0);
   Insert(partNum, desc13);

   partNum = "MiRICi-GE-T3";
   SfpDesc desc14(false, false, true, false, true, 0, 0);
   Insert(partNum, desc14);

   partNum = "MiRIi-GET3";
   SfpDesc desc14b(false, false, true, false, true, 0, 0);
   Insert(partNum, desc14b);

   partNum = "MiTOP-FE-T1";
   SfpDesc desc15(false, true, false, false, true, 0, 0);
   Insert(partNum, desc15);

   partNum = "MiTOP-FE-E1";
   SfpDesc desc16(false, true, false, false, true, 0, 0);
   Insert(partNum, desc16);

   partNum = "MiTOP-FE-T3";
   SfpDesc desc17(false, true, false, false, true, 0, 0);
   Insert(partNum, desc17);

   partNum = "MiTOP-FE-E3";
   SfpDesc desc18(false, true, false, false, true, 0, 0);
   Insert(partNum, desc18);

   partNum = "MiTOP-GE-T3";
   SfpDesc desc19(false, false, true, false, true, 0, 0);
   Insert(partNum, desc19);

   partNum = "MiTOP-GE-E3";
   SfpDesc desc20(false, false, true, false, true, 0, 0);
   Insert(partNum, desc20);

   // Agilestar 1000Mbps SFP copper
   partNum = "7SV-001";
   SfpDesc desc21(false, false, true, false, false, 0, 0);
   Insert(partNum, desc21);

   // SFP-1000-COPPER, 0-85C  (35.4%)
   partNum = "7SV-000";
   SfpDesc desc22(true, true, true, false, false, 0, 0);
   Insert(partNum, desc22);

   // SFP-1000-COPPER, 0-85C  (35.4%)
   partNum = "7SV-000-AS";
   SfpDesc desc23(true, true, true, false, false, 0, 0);
   Insert(partNum, desc23);

   // SFP-1000-MM-7.5dB (550m) 850nm  (25.8%)
   partNum = "7SM-000";
   SfpDesc desc24(false, false, true, false, true, 850, 850);
   Insert(partNum, desc24);

   // SFP-1000-SM-11dB (10km) 1310nm   (10.6%)
   partNum = "7SN-000";
   SfpDesc desc25(false, false, true, false, true, 1310, 1310);
   Insert(partNum, desc25);

   // SFP-1000-BIDIR-SM-17dB (40km) 1490 - 1310nm  (2.2%)
   partNum = "7SZ-000";
   SfpDesc desc26(false, false, true, false, true, 1490, 1310);
   Insert(partNum, desc26);

   // SFP-1000-BIDIR-SM-17dB (40km) 1310 - 1490nm  (2.2%)
   partNum = "7SY-000";
   SfpDesc desc27(false, false, true, false, true, 1310, 1490);
   Insert(partNum, desc27);

   // SFP-100-SM-16dB (20km) 1310nm  (0.7%)
   partNum = "7SA-000";
   SfpDesc desc28(false, true, false, false, true, 1310, 1310);
   Insert(partNum, desc28);

   // Multi-rate SFP 100M/1G to be supported in a near future
   //partNum = "TRPCG1CLXIACEE2G";
   //SfpDesc desc29(false, true, true, false, true, 1310, 1310);
   //Insert(partNum, desc29);

   //
   // Copper pigtails from Siemon
   //
   // These SFP support any rate from 10Mbps to 10 Gbps
   // They are considered as optical SFP since they do not have a built in PHY
   //
   partNum = "SFPP30-01";
   SfpDesc desc30(true, true, true, true, true, 256, 256);
   Insert(partNum, desc30);
   //
   partNum = "SFPP30-001";
   SfpDesc desc31(true, true, true, true, true, 256, 256);
   Insert(partNum, desc31);
   //
   partNum = "SFPP30-002";
   SfpDesc desc32(true, true, true, true, true, 256, 256);
   Insert(partNum, desc32);

   // PROLABS GLC-BX-D-C
   partNum = "GLC-BX-D-C";
   SfpDesc desc33(false, false, true, false, true, 1490, 1310);
   Insert(partNum, desc33);

   // PROLABS GLC-BX-U-C
   partNum = "GLC-BX-U-C";
   SfpDesc desc34(false, false, true, false, true, 1310, 1490);
   Insert(partNum, desc34);

   // SOLID-OPTICS GLC-BX-D40+-SO
   partNum = "GLC-BX-D40+-SO";
   SfpDesc desc35(false, false, true, false, true, 1490, 1310);
   Insert(partNum, desc35);

   // SOLID-OPTICS GLC-BX-U40+-SO
   partNum = "GLC-BX-U40+-SO";
   SfpDesc desc36(false, false, true, false, true, 1310, 1490);
   Insert(partNum, desc36);

   // Ciena T3 with GE host interface
   partNum = "XCVR-TDM3GE";
   SfpDesc desc37(false, false, true, false, true, 0, 0);
   Insert(partNum, desc37);

   // OCP
   partNum = "TRP148KL2I00040G";
   SfpDesc desc38(false, false, true, false, true, 1531, 1531);
   Insert(partNum, desc38);

   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Save the SFP database to the persistent storage

   Current implementation does not use an external file
   This method does nothing for now
   Future implementation would write the SFP data to a file

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool SfpDb::Save()
{
   return true;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Insert a SFP descriptor into the database

   @param [in]      a_partNumber : SFP part number
   @param [in]      a_sfpDesc    : SFP descriptor

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool SfpDb::Insert(string& a_partNumber, SfpDesc& a_sfpDesc)
{
   bool bRet = true;
   if ( m_sfpMap.find(a_partNumber) != m_sfpMap.end() )
   {
      printf("\nERROR: >>>>>>>>>>>>>>  SFP %s is already present in the SFP database <<<<<<<<<<<<<<\n",
             a_partNumber.c_str());
      bRet = false;
   }
   else
   {
      m_sfpMap.insert(std::pair<string, SfpDesc>(a_partNumber, a_sfpDesc));
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Remove a SFP descriptor from the database

   @param [in]      a_partNumber : SFP part number

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool SfpDb::Remove(string& a_partNumber)
{
   bool bRet = false;
   if ( m_sfpMap.find(a_partNumber) != m_sfpMap.end() )
   {
      m_sfpMap.erase(a_partNumber);
      bRet = true;
   }
   return bRet;
}

// ------------------------------------------------------------------------------------------------
/*!@brief Show the SFP database content for debug purposes

   @return     true if successful
*/
// ------------------------------------------------------------------------------------------------
bool SfpDb::Show()
{
   SfpMapType::iterator i;

   printf("\n           Part          Type     10M  100M  1G  10G      Tx       Rx \n");
   printf("\n     -----------------   ----     ---  ----  --  ---     ----     ----\n");
   for(i = m_sfpMap.begin() ; i != m_sfpMap.end() ; i++)
   {
      printf("\t %-15s %s    %c    %c    %c    %c     %4d     %4d\n",
             (*i).first.c_str(),
             (*i).second.IsTypeFiber() ? "Fiber  " : "Copper ",
             (*i).second.Is10MCapable() ? 'Y' : 'N',
             (*i).second.Is100MCapable() ? 'Y' : 'N',
             (*i).second.Is1GCapable() ? 'Y' : 'N',
             (*i).second.Is10GCapable() ? 'Y' : 'N',
             (*i).second.GetTxWavelength(),
             (*i).second.GetRxWavelength()
             );
   }
   printf("\n");
   return true;
}
