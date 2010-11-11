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
 * @file scmn_bitb.h
 *
 * @brief Definitions for Bitstream Buffer for Media Data
 *
 * @date 2009-02-21 Kwang Pyo Choi creates code
 *
 * @author Kwang Pyo Choi at Samsung Electronics
 *****************************************************************************
 */
#ifndef __SCMN_BITB_H__93024738294683924783920473829473829__
#define __SCMN_BITB_H__93024738294683924783920473829473829__

#ifdef __cplusplus
extern "C"
{
#endif

/* bitstream buffer definition ***********************************************/
typedef struct
{
	/* user space address indicating buffer */
	void   * addr;
	/* physical address indicating buffer, if any */
	void   * pddr;
	/* byte size of buffer */
	int      size;
	/* bitstream has an error? */
	int      err;
	/* media type, if any */
	int      mt;
	/* to align memory */
	int      __dummy0;
	/* arbitrary data */
	int      data[16];
} SCMN_BITB;

#ifdef __cplusplus
}
#endif

#endif /* __SCMN_BITB_H__93024738294683924783920473829473829__ */

