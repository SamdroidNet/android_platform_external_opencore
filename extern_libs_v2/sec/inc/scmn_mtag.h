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
 * @file scmn_mtag.h
 *
 * @brief Declarations of media TAG information
 *
 * @date 2009-07-13 Minseok Choi creates code
 *
 * @author Minseok Choi at Samsung Electronics
 *****************************************************************************
 */
#ifndef __SCMN_MTAG_H__9302473892473928493920473280483920483920__
#define __SCMN_MTAG_H__9302473892473928493920473280483920483920__

#ifdef __cplusplus
extern "C"
{
#endif

/*! maximum number of attached pictures */
#define SCMN_MTAG_MAX_PIC_CNT             (16)

/*! structure for media informations described in a tag */
typedef struct
{
	SCMN_VDATA         title;      /* title */
	SCMN_VDATA         artist;     /* artist */
	SCMN_VDATA         album;      /* album */
	SCMN_VDATA         genre;      /* genre */
	int                year;       /* year */
	int                track_num;  /* track number */
	int                track_total; /* total track number */
	int                pic_cnt;    /* the number of pictures embeded */
	SCMN_BITB          pic[SCMN_MTAG_MAX_PIC_CNT]; /* picture informations */
} SCMN_MTAG;

#ifdef __cplusplus
}
#endif

#endif /* __SCMN_MTAG_H__9302473892473928493920473280483920483920__ */

