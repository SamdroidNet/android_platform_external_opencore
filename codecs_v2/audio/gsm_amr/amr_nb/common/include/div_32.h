/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
/****************************************************************************************
Portions of this file are derived from the following 3GPP standard:

    3GPP TS 26.073
    ANSI-C code for the Adaptive Multi-Rate (AMR) speech codec
    Available from http://www.3gpp.org

(C) 2004, 3GPP Organizational Partners (ARIB, ATIS, CCSA, ETSI, TTA, TTC)
Permission to distribute, modify and use this file under the standard license
terms listed above has been obtained from the copyright holder.
****************************************************************************************/
/*

 Filename: /audio/gsm_amr/c/include/div_32.h


------------------------------------------------------------------------------
 REVISION HISTORY

 Description: Updated function prototype declaration to reflect new interface.
              A pointer to overflow flag is passed into the function. Updated
              template.

 Description: Moved _cplusplus #ifdef after Include section.

 Who:                       Date:
 Description:

------------------------------------------------------------------------------
 INCLUDE DESCRIPTION

 This file contains all the constant definitions and prototype definitions
 needed by the Div_32 function.

------------------------------------------------------------------------------
*/

/*----------------------------------------------------------------------------
; CONTINUE ONLY IF NOT ALREADY DEFINED
----------------------------------------------------------------------------*/
#ifndef DIV_32_H
#define DIV_32_H

/*----------------------------------------------------------------------------
; INCLUDES
----------------------------------------------------------------------------*/

#include        "basicop_malloc.h"

/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
extern "C"
{
#endif

    /*----------------------------------------------------------------------------
    ; MACROS
    ; Define module specific macros here
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; DEFINES
    ; Include all pre-processor statements here.
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; EXTERNAL VARIABLES REFERENCES
    ; Declare variables used in this module but defined elsewhere
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; SIMPLE TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; ENUMERATED TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; STRUCTURES TYPEDEF'S
    ----------------------------------------------------------------------------*/

    /*----------------------------------------------------------------------------
    ; GLOBAL FUNCTION DEFINITIONS
    ; Function Prototype declaration
    ----------------------------------------------------------------------------*/
    Word32 Div_32(Word32 L_num,
                  Word16 L_denom_hi,
                  Word16 L_denom_lo,
                  Flag   *pOverflow) ;

    /*----------------------------------------------------------------------------
    ; END
    ----------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif

#endif /* _DIV_32_H_ */
