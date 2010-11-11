/****************************************************************************
 *                       Samsung Electronics Co. Ltd.                       *
 *                                                                          *
 *                           COPYRIGHT (C) 2006                             *
 *                          All Rights Reserved                             *
 *                                                                          *
 *                       Mobile Media Lab. R&D Center                       *
 *                 Digital Media & Communications Business                  *
 ****************************************************************************/

/*!
 *****************************************************************************
 * @file scmn_audb.h
 *
 * @brief Definition for Speech/Audio Buffer
 *
 * @date 2008-12-18 Kwang Pyo Choi creates code
 *
 * @author Kwang Pyo Choi at Samsung Electronics
 *****************************************************************************
 */
#ifndef __SCMN_AUDB_H__3902743892743828493028493028439028432__
#define __SCMN_AUDB_H__3902743892743828493028493028439028432__

#ifdef __cplusplus
extern "C"
{
#endif

/* max channel count *********************************************************/
#define SCMN_AUDB_MAX_CHAN               (7+1)

/* channel muxing type *******************************************************/
/* unpack type (each channel uses seperated buffer */
#define SCMN_AUDB_CMT_UNPACK             (0)
/* pack type (each channel's sample interleaved in a buffer) */
#define SCMN_AUDB_CMT_PACK               (1)

/* channel id ****************************************************************/
/* center */
#define SCMN_AUDB_CHAN_C                 (0)
/* left */
#define SCMN_AUDB_CHAN_L                 (1)
/* right */
#define SCMN_AUDB_CHAN_R                 (2)
/* surround */
#define SCMN_AUDB_CHAN_S                 (3)
/* left surround */
#define SCMN_AUDB_CHAN_LS                (4)
/* right surround */
#define SCMN_AUDB_CHAN_RS                (5)
/* LFE */
#define SCMN_AUDB_CHAN_LFE               (6)


/* audio buffer definition ***************************************************/
typedef struct
{
	int      s[SCMN_AUDB_MAX_CHAN]; /* sample count of each audo track */
	int      b[SCMN_AUDB_MAX_CHAN]; /* byte count of each audio track */
	void   * a[SCMN_AUDB_MAX_CHAN]; /* user space address of each audio track */
	void   * p[SCMN_AUDB_MAX_CHAN]; /* physical address of each audio track */
	int      chan;              /* channel count */
	int      bits;              /* bits per a sample */
	int      hz;                /* sampling rate (Hz) */
	int      cmt;               /* channel muxing type */
	int      data[16];          /* arbitrary data */
} SCMN_AUDB;

#ifdef __cplusplus
}
#endif

#endif /* __SCMN_AUDB_H__3902743892743828493028493028439028432__ */

