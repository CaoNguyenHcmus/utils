/*****************************************************************************/
/*                                                                           */
/*    ACCEDIAN                                                               */
/*    The Carrier Sense of Ethernet(TM)                                      */
/*                                                                           */
/*    Accedian Networks, Inc.                                                */
/*    4878 Levy, suite 202                                                   */
/*    Saint-Laurent, Quebec Canada H4R 2P1                                   */
/*    E-mail: support@accedian.com                                           */
/*                                                                           */
/*    www.accedian.com                                                       */
/*                                                                           */
/* ACCEDIAN PROPRIETARY                                                      */
/*                                                                           */
/* COPYRIGHT (c) 2004-2011 BY ACCEDIAN CORPORATION. ALL RIGHTS RESERVED. NO  */
/* PART OF THIS PROGRAM OR PUBLICATION MAY BE REPRODUCED, TRANSMITTED,       */
/* TRANSCRIBED, STORED IN A RETRIEVAL SYSTEM, OR TRANSLATED INTO ANY LANGUAGE*/
/* OR COMPUTER LANGUAGE IN ANY FORM OR BY ANY MEANS, ELECTRONIC, MECHANICAL, */
/* MAGNETIC, OPTICAL, CHEMICAL, MANUAL, OR OTHERWISE, WITHOUT THE PRIOR      */
/* WRITTEN PERMISSION OF ACCEDIAN INC.                                       */
/*                                                                           */
/*****************************************************************************/

#ifndef _SFP_MSA_H_
#define _SFP_MSA_H_

#include <global/acd_types.h>

/*
 * Identifier values
 */
enum sfp_id_enum
{
   SFP_ID_UNSPECIFIED      = 0x0,
   SFP_ID_GBIC             = 0x1,
   SFP_ID_SOLDERED         = 0x2,
   SFP_ID_SFP              = 0x3,
   SFP_ID_XBI              = 0x4,
   SFP_ID_XENPACK          = 0x5,
   SFP_ID_XFP              = 0x6,
   SFP_ID_XFFP             = 0x7,
   SFP_ID_XFPEE            = 0x8,
   SFP_ID_XPACK            = 0x9,
   SFP_ID_X2               = 0xA,
   SFP_ID_DWDM_SFP         = 0xB,
   SFP_ID_QSFP             = 0xC
};

/*
 * Connector values
 */
enum sfp_conn_id_enum
{
   SFP_CONN_ID_UNSPECIFIED    = 0x00,
   SFP_CONN_ID_SC             = 0x01,
   SFP_CONN_ID_FC1COPPER      = 0x02,
   SFP_CONN_ID_FC2COPPER      = 0x03,
   SFP_CONN_ID_BNC            = 0x04,
   SFP_CONN_ID_FCCOAX         = 0x05,
   SFP_CONN_ID_FIBERJACK      = 0x06,
   SFP_CONN_ID_LC             = 0x07,
   SFP_CONN_ID_MT_RJ          = 0x08,
   SFP_CONN_ID_MU             = 0x09,
   SFP_CONN_ID_SG             = 0x0a,
   SFP_CONN_ID_OPT_PIGTAIL    = 0x0b,
   SFP_CONN_ID_MPO_PAR_OPT    = 0x0c,
   SFP_CONN_ID_HSSDC_II       = 0x20,
   SFP_CONN_ID_COPPER_PIGTAIL = 0x21,
   SFP_CONN_ID_RJ45           = 0x22
};

/*
 * Identifier values
 */
enum sfp_extid_enum
{
   SFP_EXTID_UNSPECIFIED   = 0x0,
   SFP_EXTID_MOD_DEF1      = 0x1,
   SFP_EXTID_MOD_DEF2      = 0x2,
   SFP_EXTID_MOD_DEF3      = 0x3,
   SFP_EXTID_SFP           = 0x4,
   SFP_EXTID_MOD_DEF5      = 0x5,
   SFP_EXTID_MOD_DEF6      = 0x6,
   SFP_EXTID_MOD_DEF7      = 0x7
};

/*
 * Encoding values
 */
enum sfp_encoding_enum
{
   SFP_ENCODE_UNSPECIFIED  = 0x0,
   SFP_ENCODE_8B10B        = 0x1,
   SFP_ENCODE_4B5B         = 0x2,
   SFP_ENCODE_NRZ          = 0x3,
   SFP_ENCODE_MANCHESTER   = 0x4,
   SFP_ENCODE_SONET        = 0x5,
   SFP_ENCODE_6466B        = 0x6
};

/*
 * Transceiver codes
 */
static const acd_uint8_t SONET_OC12_RESV     = 0x80;
static const acd_uint8_t SONET_OC12_LR       = 0x40;
static const acd_uint8_t SONET_OC12_IR       = 0x20;
static const acd_uint8_t SONET_OC12_SR       = 0x10;
static const acd_uint8_t SONET_OC3_RESV      = 0x08;
static const acd_uint8_t SONET_OC3_LR        = 0x04;
static const acd_uint8_t SONET_OC3_IR        = 0x02;
static const acd_uint8_t SONET_OC3_SR        = 0x01;

static const acd_uint8_t ETH_BASE_PX         = 0x80;
static const acd_uint8_t ETH_BASE_BX         = 0x40;
static const acd_uint8_t ETH_100BASE_FX      = 0x20;
static const acd_uint8_t ETH_100BASE_LX      = 0x10;
static const acd_uint8_t ETH_1000BASE_T      = 0x08;
static const acd_uint8_t ETH_1000BASE_CX     = 0x04;
static const acd_uint8_t ETH_1000BASE_LX     = 0x02;
static const acd_uint8_t ETH_1000BASE_SX     = 0x01;

// Ethernet 10G compliance code: register 3 (0xA0h)
static const acd_uint8_t ETH_10GBASE_ER      = 0x80;
static const acd_uint8_t ETH_10GBASE_LRM     = 0x40;
static const acd_uint8_t ETH_10GBASE_LR      = 0x20;
static const acd_uint8_t ETH_10GBASE_SR      = 0x10;
static const acd_uint8_t ETH_10GBASE_XX      = 0xF0;

/*
 * Rate values (byte 13 in A0)
 */
enum sfp_rate_enum
{
   SFP_RATE_UNSPECIFIED_0  = 0x00,  // Unspecified
   SFP_RATE_4_2_1G         = 0x01,  // Defined for SFF-8079 (4/2/1G Rate_Select & AS0/AS1)
   SFP_RATE_8_4_2G_RX      = 0x02,  //  Defined for SFF-8431 (8/4/2G Rx Rate_Select only)
   SFP_RATE_UNSPECIFIED_3  = 0x03,  // Unspecified *
   SFP_RATE_8_4_2G_TX      = 0x04,  // Defined for SFF-8431 (8/4/2G Tx Rate_Select only)
   SFP_RATE_UNSPECIFIED_5  = 0x05,  // Unspecified *
   SFP_RATE_8_4_2G_RX_TX   = 0x06,  // Defined for SFF-8431 (8/4/2G Independent Rx & Tx Rate_select)
   SFP_RATE_UNSPECIFIED_7  = 0x07,  // Unspecified *
   SFP_RATE_16_8_4G_RX     = 0x08,  // Defined for FC-PI-5 (16/8/4G Rx Rate_select only) High=16G only, Low=8G/4G
   SFP_RATE_UNSPECIFIED_9  = 0x09,  // Unspecified *
   SFP_RATE_16_8_4G_RX_TX  = 0x0A   // Defined for FC-PI-5 (16/8/4G Independent Rx, Tx Rate_select) High=16G only, Low=8G/4G
};

/*
 * Serial ID data fields
 */
struct sfp_hdr_type
{
   acd_uint8_t         id;                 // 0 type of serial transceiver
   acd_uint8_t         ext_id;             // 1 extended identifier
   acd_uint8_t         connector;          // 2 extended identifier
   acd_uint8_t         trans3;             // 3 code for electronic compa1
   acd_uint8_t         trans4;             // 4 code for electronic compa2
   acd_uint8_t         trans5;             // 5 code for electronic compa3
   acd_uint8_t         trans6;             // 6 code for electronic compa4
   acd_uint8_t         trans7;             // 7 code for electronic compa5
   acd_uint8_t         trans8;             // 8 code for electronic compa6
   acd_uint8_t         trans9;             // 9 code for electronic compa7

   acd_uint8_t         trans10;            // 10 code for electronic compa8
   acd_uint8_t         encoding;           // 11
   acd_uint8_t         bit_rate;           // 12
   acd_uint8_t         rsv1;               // 13
   acd_uint8_t         length9mkm;         // 14
   acd_uint8_t         length9m;           // 15
   acd_uint8_t         length50;           // 16
   acd_uint8_t         length625;          // 17
   acd_uint8_t         length_copper;      // 18
   acd_uint8_t         rsv2;               // 19

   acd_uint8_t         vendor[16];         // Vendor name
   acd_uint8_t         transceiver;        // 36
   acd_uint8_t         vendor_oui[3];      // vendor Org. Unique ID
   acd_uint8_t         vendor_pn[16];      // part number in ASCII
   acd_uint8_t         vendor_rev[4];      // revision number in ASCII
   acd_uint16_t        wave_length;        // Laser wave length
   acd_uint8_t         rsv4;               // 62
   acd_uint8_t         cc_base;            //
   acd_uint8_t         option1;            //
   acd_uint8_t         option2;            //
   acd_uint8_t         br_max;             //
   acd_uint8_t         br_min;             //
   acd_uint8_t         serial[16];         //
   acd_uint16_t        year;               // Date Code: year
   acd_uint16_t        month;              // Date Code: 01=Jan
   acd_uint16_t        day;                // Date Code: 01-31 day of month
   acd_uint16_t        lot;                // Date Code: lot code
   acd_uint8_t         diag;               // 92 Diagnostic monitor
   acd_uint8_t         enhance;            // 93 Diagnostic monitor
   acd_uint8_t         rev_8472;           // 94 Diagnostic monitor
}  __attribute__((__packed__));

struct sfp_mon_type
{
   // Alarm and Warning Thresholds

   // SFP Internal Temperature Sensor
   acd_uint16_t        ts_high_alm;        // High Alarm 00-01
   acd_uint16_t        ts_low_alm;         // Low Alarm 02-03
   acd_uint16_t        ts_high_warn;       // High Warning 04-05
   acd_uint16_t        ts_low_warn;        // Low Warning 06-07

   // Supply Voltage
   acd_uint16_t        vcc_high_alm;       // High Alarm 08-09
   acd_uint16_t        vcc_low_alm;        // Low Alarm 10-11
   acd_uint16_t        vcc_high_warn;      // High Warning 12-13
   acd_uint16_t        vcc_low_warn;       // Low Warning 14-15

   // Laser Bias Current
   acd_uint16_t        lbc_high_alm;       // High Alarm 16-17
   acd_uint16_t        lbc_low_alm;        // Low Alarm 18-19
   acd_uint16_t        lbc_high_warn;      // High Warning 20-21
   acd_uint16_t        lbc_low_warn;       // Low Warning 22-23

   // Transmit Power
   acd_uint16_t        tx_pwr_high_alm;    // High Alarm 24-25
   acd_uint16_t        tx_pwr_low_alm;     // Low Alarm 26-27
   acd_uint16_t        tx_pwr_high_warn;   // High Warning 28-29
   acd_uint16_t        tx_pwr_low_warn;    // Low Warning 30-31

   // Receive Power
   acd_uint16_t        rx_pwr_high_alm;    // High Alarm 32-33
   acd_uint16_t        rx_pwr_low_alm;     // Low Alarm 34-35
   acd_uint16_t        rx_pwr_high_warn;   // High Warning 36-37
   acd_uint16_t        rx_pwr_low_warn;    // Low Warning 38-39

   acd_uint8_t         resv1[16];          // Reserved 40-55

   // Calibration constants for external Calibration option

   acd_uint32_t        rx_pwr4;            // 56-59
   acd_uint32_t        rx_pwr3;            // 60-63
   acd_uint32_t        rx_pwr2;            // 64-67
   acd_uint32_t        rx_pwr1;            // 68-71
   acd_uint32_t        rx_pwr0;            // 72-75
   acd_uint16_t        lbc_slope;          // 76-77
   acd_int16_t         lbc_offset;         // 78-79
   acd_uint16_t        tx_pwr_slope;       // 80-81
   acd_int16_t         tx_pwr_offset;      // 82-83
   acd_uint16_t        ts_slope;           // 84-85
   acd_int16_t         ts_offset;          // 86-87
   acd_uint16_t        vcc_slope;          // 88-89
   acd_int16_t         vcc_offset;         // 90-91

   acd_uint8_t         resv2[3];           // Reserved 92-94
   acd_uint8_t         checsum;            // Checksum 95

   acd_uint16_t        temp;
   acd_uint16_t        vcc;
   acd_uint16_t        bias;
   acd_uint16_t        tx_pwr;
   acd_uint16_t        rx_pwr;

}  __attribute__((__packed__));

#endif

