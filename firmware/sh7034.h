/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 by Alan Korr
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

#ifndef __SH7034_H__
#define __SH7034_H__

#define GBR           0x00000000

/* register address macros: */

#define SMR0_ADDR       0x05FFFEC0
#define BRR0_ADDR       0x05FFFEC1
#define SCR0_ADDR       0x05FFFEC2
#define TDR0_ADDR       0x05FFFEC3
#define SSR0_ADDR       0x05FFFEC4
#define RDR0_ADDR       0x05FFFEC5
#define SMR1_ADDR       0x05FFFEC8
#define BRR1_ADDR       0x05FFFEC9
#define SCR1_ADDR       0x05FFFECA
#define TDR1_ADDR       0x05FFFECB
#define SSR1_ADDR       0x05FFFECC
#define RDR1_ADDR       0x05FFFECD

#define ADDRA_ADDR      0x05FFFEE0
#define ADDRAH_ADDR     0x05FFFEE0
#define ADDRAL_ADDR     0x05FFFEE1
#define ADDRB_ADDR      0x05FFFEE2
#define ADDRBH_ADDR     0x05FFFEE2
#define ADDRBL_ADDR     0x05FFFEE3
#define ADDRC_ADDR      0x05FFFEE4
#define ADDRCH_ADDR     0x05FFFEE4
#define ADDRCL_ADDR     0x05FFFEE5
#define ADDRD_ADDR      0x05FFFEE6
#define ADDRDH_ADDR     0x05FFFEE6
#define ADDRDL_ADDR     0x05FFFEE6
#define ADCSR_ADDR      0x05FFFEE8
#define ADCR_ADDR       0x05FFFEE9

#define TSTR_ADDR       0x05FFFF00
#define TSNC_ADDR       0x05FFFF01
#define TMDR_ADDR       0x05FFFF02
#define TFCR_ADDR       0x05FFFF03
#define TCR0_ADDR       0x05FFFF04
#define TIOR0_ADDR      0x05FFFF05
#define TIER0_ADDR      0x05FFFF06
#define TSR0_ADDR       0x05FFFF07
#define TCNT0_ADDR      0x05FFFF08
#define GRA0_ADDR       0x05FFFF0A
#define GRB0_ADDR       0x05FFFF0C
#define TCR1_ADDR       0x05FFFF0E
#define TIOR1_ADDR      0x05FFFF0F
#define TIER1_ADDR      0x05FFFF10
#define TSR1_ADDR       0x05FFFF11
#define TCNT1_ADDR      0x05FFFF12
#define GRA_ADDR1       0x05FFFF14
#define GRB1_ADDR       0x05FFFF16
#define TCR2_ADDR       0x05FFFF18
#define TIOR2_ADDR      0x05FFFF19
#define TIER2_ADDR      0x05FFFF1A
#define TSR2_ADDR       0x05FFFF1B
#define TCNT2_ADDR      0x05FFFF1C
#define GRA2_ADDR       0x05FFFF1E
#define GRB2_ADDR       0x05FFFF20
#define TCR3_ADDR       0x05FFFF22
#define TIOR3_ADDR      0x05FFFF23
#define TIER3_ADDR      0x05FFFF24
#define TSR3_ADDR       0x05FFFF25
#define TCNT3_ADDR      0x05FFFF26
#define GRA3_ADDR       0x05FFFF28
#define GRB3_ADDR       0x05FFFF2A
#define BRA3_ADDR       0x05FFFF2C
#define BRB3_ADDR       0x05FFFF2E
#define TOCR_ADDR       0x05FFFF31
#define TCR4_ADDR       0x05FFFF32
#define TIOR4_ADDR      0x05FFFF33
#define TIER4_ADDR      0x05FFFF34
#define TSR4_ADDR       0x05FFFF35
#define TCNT4_ADDR      0x05FFFF36
#define GRA4_ADDR       0x05FFFF38
#define GRB4_ADDR       0x05FFFF3A
#define BRA4_ADDR       0x05FFFF3C
#define BRB4_ADDR       0x05FFFF3E

#define SAR0_ADDR       0x05FFFF40
#define DAR0_ADDR       0x05FFFF44
#define OR_ADDR         0x05FFFF48
#define TCR0_ADDR       0x05FFFF4A
#define CHCR0_ADDR      0x05FFFF4E
#define SAR1_ADDR       0x05FFFF50
#define DAR1_ADDR       0x05FFFF54
#define TCR1_ADDR       0x05FFFF5A
#define CHCR1_ADDR      0x05FFFF5E
#define SAR2_ADDR       0x05FFFF60
#define DAR2_ADDR       0x05FFFF64
#define TCR2_ADDR       0x05FFFF6A
#define CHCR2_ADDR      0x05FFFF6E
#define SAR3_ADDR       0x05FFFF70
#define DAR3_ADDR       0x05FFFF74
#define TCR3_ADDR       0x05FFFF7A
#define CHCR3_ADDR      0x05FFFF7E

#define IPRA_ADDR       0x05FFFF84
#define IPRB_ADDR       0x05FFFF86
#define IPRC_ADDR       0x05FFFF88
#define IPRD_ADDR       0x05FFFF8A
#define IPRE_ADDR       0x05FFFF8C
#define ICR_ADDR        0x05FFFF8E

#define BARH_ADDR       0x05FFFF90
#define BARL_ADDR       0x05FFFF92
#define BAMRH_ADDR      0x05FFFF94
#define BAMRL_ADDR      0x05FFFF96
#define BBR_ADDR        0x05FFFF98

#define BCR_ADDR        0x05FFFFA0
#define WCR1_ADDR       0x05FFFFA2
#define WCR2_ADDR       0x05FFFFA4
#define WCR3_ADDR       0x05FFFFA6
#define DCR_ADDR        0x05FFFFA8
#define PCR_ADDR        0x05FFFFAA
#define RCR_ADDR        0x05FFFFAC
#define RTCSR_ADDR      0x05FFFFAE
#define RTCNT_ADDR      0x05FFFFB0
#define RTCOR_ADDR      0x05FFFFB2

#define TCSR_ADDR       0x05FFFFB8
#define TCNT_ADDR       0x05FFFFB9
#define RSTCSR_ADDR     0x05FFFFBB

#define SBYCR_ADDR      0x05FFFFBC
			
#define PADR_ADDR       0x05FFFFC0
#define PBDR_ADDR       0x05FFFFC2
#define PAIOR_ADDR      0x05FFFFC4
#define PBIOR_ADDR      0x05FFFFC6
#define PACR1_ADDR      0x05FFFFC8
#define PACR2_ADDR      0x05FFFFCA
#define PBCR1_ADDR      0x05FFFFCC
#define PBCR2_ADDR      0x05FFFFCE
#define PCDR_ADDR       0x05FFFFD0
			
#define CASCR_ADDR      0x05FFFFEE


/* register macros for direct access: */

#define SMR0       (*((volatile unsigned char*)SMR0_ADDR))   
#define BRR0       (*((volatile unsigned char*)BRR0_ADDR))   
#define SCR0       (*((volatile unsigned char*)SCR0_ADDR))   
#define TDR0       (*((volatile unsigned char*)TDR0_ADDR))   
#define SSR0       (*((volatile unsigned char*)SSR0_ADDR))   
#define RDR0       (*((volatile unsigned char*)RDR0_ADDR))   
#define SMR1       (*((volatile unsigned char*)SMR1_ADDR))   
#define BRR1       (*((volatile unsigned char*)BRR1_ADDR))   
#define SCR1       (*((volatile unsigned char*)SCR1_ADDR))   
#define TDR1       (*((volatile unsigned char*)TDR1_ADDR))   
#define SSR1       (*((volatile unsigned char*)SSR1_ADDR))   
#define RDR1       (*((volatile unsigned char*)RDR1_ADDR))   

#define ADDRA      (*((volatile unsigned short*)ADDRA_ADDR)) 
#define ADDRAH     (*((volatile unsigned char*)ADDRAH_ADDR)) 
#define ADDRAL     (*((volatile unsigned char*)ADDRAL_ADDR)) 
#define ADDRB      (*((volatile unsigned short*)ADDRB_ADDR)) 
#define ADDRBH     (*((volatile unsigned char*)ADDRBH_ADDR)) 
#define ADDRBL     (*((volatile unsigned char*)ADDRBL_ADDR)) 
#define ADDRC      (*((volatile unsigned short*)ADDRC_ADDR)) 
#define ADDRCH     (*((volatile unsigned char*)ADDRCH_ADDR)) 
#define ADDRCL     (*((volatile unsigned char*)ADDRCL_ADDR)) 
#define ADDRD      (*((volatile unsigned short*)ADDRD_ADDR)) 
#define ADDRDH     (*((volatile unsigned char*)ADDRDH_ADDR)) 
#define ADDRDL     (*((volatile unsigned char*)ADDRDL_ADDR)) 
#define ADCSR      (*((volatile unsigned char*)ADCSR_ADDR))  
#define ADCR       (*((volatile unsigned char*)ADCR_ADDR))   

#define TSTR       (*((volatile unsigned char*)TSTR_ADDR))   
#define TSNC       (*((volatile unsigned char*)TSNC_ADDR))   
#define TMDR       (*((volatile unsigned char*)TMDR_ADDR))   
#define TFCR       (*((volatile unsigned char*)TFCR_ADDR))   
#define TCR0       (*((volatile unsigned char*)TCR0_ADDR))   
#define TIOR0      (*((volatile unsigned char*)TIOR0_ADDR))  
#define TIER0      (*((volatile unsigned char*)TIER0_ADDR))  
#define TSR0       (*((volatile unsigned char*)TSR0_ADDR))   
#define TCNT0      (*((volatile unsigned short*)TCNT0_ADDR)) 
#define GRA0       (*((volatile unsigned short*)GRA0_ADDR))  
#define GRB0       (*((volatile unsigned short*)GRB0_ADDR))  
#define TCR1       (*((volatile unsigned char*)TCR1_ADDR))   
#define TIOR1      (*((volatile unsigned char*)TIOR1_ADDR))  
#define TIER1      (*((volatile unsigned char*)TIER1_ADDR))  
#define TSR1       (*((volatile unsigned char*)TSR1_ADDR))   
#define TCNT1      (*((volatile unsigned short*)TCNT1_ADDR)) 
#define GRA1       (*((volatile unsigned short*)GRA_ADDR))1  
#define GRB1       (*((volatile unsigned short*)GRB1_ADDR))  
#define TCR2       (*((volatile unsigned char*)TCR2_ADDR))   
#define TIOR2      (*((volatile unsigned char*)TIOR2_ADDR))  
#define TIER2      (*((volatile unsigned char*)TIER2_ADDR))  
#define TSR2       (*((volatile unsigned char*)TSR2_ADDR))   
#define TCNT2      (*((volatile unsigned short*)TCNT2_ADDR)) 
#define GRA2       (*((volatile unsigned short*)GRA2_ADDR))  
#define GRB2       (*((volatile unsigned short*)GRB2_ADDR))  
#define TCR3       (*((volatile unsigned char*)TCR3_ADDR))   
#define TIOR3      (*((volatile unsigned char*)TIOR3_ADDR))  
#define TIER3      (*((volatile unsigned char*)TIER3_ADDR))  
#define TSR3       (*((volatile unsigned char*)TSR3_ADDR))   
#define TCNT3      (*((volatile unsigned short*)TCNT3_ADDR)) 
#define GRA3       (*((volatile unsigned short*)GRA3_ADDR))  
#define GRB3       (*((volatile unsigned short*)GRB3_ADDR))  
#define BRA3       (*((volatile unsigned short*)BRA3_ADDR))  
#define BRB3       (*((volatile unsigned short*)BRB3_ADDR))  
#define TOCR       (*((volatile unsigned char*)TOCR_ADDR))   
#define TCR4       (*((volatile unsigned char*)TCR4_ADDR))   
#define TIOR4      (*((volatile unsigned char*)TIOR4_ADDR))  
#define TIER4      (*((volatile unsigned char*)TIER4_ADDR))  
#define TSR4       (*((volatile unsigned char*)TSR4_ADDR))   
#define TCNT4      (*((volatile unsigned short*)TCNT4_ADDR)) 
#define GRA4       (*((volatile unsigned short*)GRA4_ADDR))  
#define GRB4       (*((volatile unsigned short*)GRB4_ADDR))  
#define BRA4       (*((volatile unsigned short*)BRA4_ADDR))  
#define BRB4       (*((volatile unsigned short*)BRB4_ADDR))  

#define SAR0       (*((volatile unsigned long*)SAR0_ADDR))   
#define DAR0       (*((volatile unsigned long*)DAR0_ADDR))   
#define DMAOR      (*((volatile unsigned long*)DMAOR_ADDR))  
#define TCR0       (*((volatile unsigned long*)TCR0_ADDR))   
#define CHCR0      (*((volatile unsigned short*)CHCR0_ADDR)) 
#define SAR1       (*((volatile unsigned long*)SAR1_ADDR))   
#define DAR1       (*((volatile unsigned long*)DAR1_ADDR))   
#define TCR1       (*((volatile unsigned long*)TCR1_ADDR))   
#define CHCR1      (*((volatile unsigned short*)CHCR1_ADDR)) 
#define SAR2       (*((volatile unsigned long*)SAR2_ADDR))   
#define DAR2       (*((volatile unsigned long*)DAR2_ADDR))   
#define TCR2       (*((volatile unsigned long*)TCR2_ADDR))   
#define HCR2       (*((volatile unsigned short*)CHCR2_ADDR)) 
#define SAR3       (*((volatile unsigned long*)SAR3_ADDR))   
#define DAR3       (*((volatile unsigned long*)DAR3_ADDR))   
#define TCR3       (*((volatile unsigned long*)TCR3_ADDR))   
#define CHCR3      (*((volatile unsigned short*)CHCR3_ADDR)) 

#define IPRA       (*((volatile unsigned short*)IPRA_ADDR))  
#define IPRB       (*((volatile unsigned short*)IPRB_ADDR))  
#define IPRC       (*((volatile unsigned short*)IPRC_ADDR))  
#define IPRD       (*((volatile unsigned short*)IPRD_ADDR))  
#define IPRE       (*((volatile unsigned short*)IPRE_ADDR))  
#define ICR        (*((volatile unsigned short*)ICR_ADDR))   

#define BARH       (*((volatile unsigned short*)BARH_ADDR))  
#define BARL       (*((volatile unsigned short*)BARL_ADDR))  
#define BAMRH      (*((volatile unsigned short*)BAMRH_ADDR)) 
#define BAMRL      (*((volatile unsigned short*)BAMRL_ADDR)) 
#define BBR        (*((volatile unsigned short*)BBR_ADDR))   

#define BCR        (*((volatile unsigned short*)BCR_ADDR))   
#define WCR1       (*((volatile unsigned short*)WCR1_ADDR))  
#define WCR2       (*((volatile unsigned short*)WCR2_ADDR))  
#define WCR3       (*((volatile unsigned short*)WCR3_ADDR))  
#define DCR        (*((volatile unsigned short*)DCR_ADDR))   
#define PCR        (*((volatile unsigned short*)PCR_ADDR))   
#define RCR        (*((volatile unsigned short*)RCR_ADDR))   
#define RTCSR      (*((volatile unsigned short*)RTCSR_ADDR)) 
#define RTCNT      (*((volatile unsigned short*)RTCNT_ADDR)) 
#define RTCOR      (*((volatile unsigned short*)RTCOR_ADDR)) 

#define TCSR       (*((volatile unsigned char*)TCSR_ADDR))   
#define TCNT       (*((volatile unsigned char*)TCNT_ADDR))   
#define RSTCSR     (*((volatile unsigned char*)RSTCSR_ADDR)) 

#define SBYCR      (*((volatile unsigned char*)SBYCR_ADDR))  

#define PADR       (*((volatile unsigned short*)PADR_ADDR))  
#define PBDR       (*((volatile unsigned short*)PBDR_ADDR))  
#define PAIOR      (*((volatile unsigned short*)PAIOR_ADDR)) 
#define PBIOR      (*((volatile unsigned short*)PBIOR_ADDR)) 
#define PACR1      (*((volatile unsigned short*)PACR1_ADDR)) 
#define PACR2      (*((volatile unsigned short*)PACR2_ADDR)) 
#define PBCR1      (*((volatile unsigned short*)PBCR1_ADDR)) 
#define PBCR2      (*((volatile unsigned short*)PBCR2_ADDR)) 
#define PCDR       (*((volatile unsigned short*)PCDR_ADDR))

#define CASCR      (*((volatile unsigned char*)CASCR_ADDR))  

#endif
