/****************************************************************************
 *                       Samsung Electronics Co. Ltd.                       *
 *                                                                          *
 *                           COPYRIGHT (C) 2009                             *
 *                          All Rights Reserved                             *
 *                                                                          *
 *                       Mobile Media Lab. R&D Center                       *
 *                 Digital Media & Communications Business                  *
 ****************************************************************************/

/*!
 *****************************************************************************
 * @file scmn_base.h
 *
 * @brief Common include file for Samsung Audio/Video Solution
 *
 * @date 2009-02-23 Kwang Pyo Choi creates code
 *
 * @author Kwang Pyo Choi at Samsung Electronics
 *****************************************************************************
 */
#ifndef __SCMN_BASE_H__930824390247839028439204839024832__
#define __SCMN_BASE_H__930824390247839028439204839024832__

/* structure for representing variable data */
typedef struct
{
	int       type; /*type */
	int       size; /* byte size of data */
	void    * data; /* address of data */
	int       ___dummy; /* dummy data to make align. don't use this */
} SCMN_VDATA;

#include "scmn_mtype.h"
#include "scmn_bitb.h"
#include "scmn_imgb.h"
#include "scmn_audb.h"
#include "scmn_mfal.h"
#include "scmn_mtag.h"

char * scmn_version(void);


#endif /* __SCMN_BASE_H__930824390247839028439204839024832__ */

