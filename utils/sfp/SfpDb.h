// ------------------------------------------------------------------------------------------------
/* ACCEDIAN PROPRIETARY - www.accedian.com
   COPYRIGHT (c) 2004-2012 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO PART OF THIS PROGRAM OR
   PUBLICATION MAY BE REPRODUCED, TRANSMITTED, TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM,
   OR TRANSLATED INTO ANY LANGUAGE OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC,
   MECHANICAL, MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR WRITTEN
   PERMISSION OF ACCEDIAN INC.
*/
// ------------------------------------------------------------------------------------------------
/*!@file    SfpDb.h
   @brief   SFP database

   This file contains the SFP database class definition
*/
// ------------------------------------------------------------------------------------------------
#ifndef __SFPDB_H__
#define __SFPDB_H__

#include <map>
#include <string>
using namespace std;

#include <global/acd_types.h>

// ------------------------------------------------------------------------------------------------
/*!@brief SFP database class

   This class defines the SFP database interface
*/
// ------------------------------------------------------------------------------------------------
class SfpDb
{
   public:

      static SfpDb* GetInstance();

      class SfpDesc
      {
         public:
            SfpDesc(bool a_bSpeed10M,
                    bool a_bSpeed100M,
                    bool a_bSpeed1G,
                    bool a_bSpeed10G,
                    bool a_bIsFiber,
                    acd_uint32_t a_txWavelength,
                    acd_uint32_t a_rxWavelength)
            {
               bSpeed10M      = a_bSpeed10M;
               bSpeed100M     = a_bSpeed100M;
               bSpeed1G       = a_bSpeed1G;
               bSpeed10G      = a_bSpeed10G;
               bIsFiber       = a_bIsFiber;
               txWavelength   = a_txWavelength;
               rxWavelength   = a_rxWavelength;
            }
            SfpDesc()
            {
               bSpeed10M      = false;
               bSpeed100M     = false;
               bSpeed1G       = false;
               bSpeed10G      = false;
               bIsFiber       = false;
               txWavelength   = 0;
               rxWavelength   = 0;
            }
            bool Is10MCapable()  { return bSpeed10M;  }
            bool Is100MCapable() { return bSpeed100M; }
            bool Is1GCapable()   { return bSpeed1G;   }
            bool Is10GCapable()  { return bSpeed10G;  }
            bool IsTypeCopper()  { return !bIsFiber;}
            bool IsTypeFiber()   { return bIsFiber; }
            acd_uint32_t GetTxWavelength() { return txWavelength; }
            acd_uint32_t GetRxWavelength() { return rxWavelength; }
         private:
            bool           bIsFiber;
            bool           bSpeed10M;
            bool           bSpeed100M;
            bool           bSpeed1G;
            bool           bSpeed10G;
            acd_uint32_t   txWavelength;
            acd_uint32_t   rxWavelength;
      };

      bool Get(string& a_partNumber, SfpDesc& a_sfpDesc);
      bool Load();
      bool Save();
      bool Insert(string& a_partNumber, SfpDesc& a_sfpDesc);
      bool Remove(string& a_partNumber);

      bool Show();

private:
      SfpDb();
      virtual ~SfpDb();

      typedef std::map<string, SfpDesc> SfpMapType;
      SfpMapType  m_sfpMap;
      static SfpDb*  s_pTheInstance;
};

#endif   // __SFPDB_H__
