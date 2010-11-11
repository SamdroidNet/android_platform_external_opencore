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
 * @file scmn_mfal.h
 *
 * @brief Declarations of media file I/O abstraction layer
 *
 * @date 2008-10-29 Kwang Pyo Choi creates code
 *
 * @author Kwang Pyo Choi at Samsung Electronics
 *****************************************************************************
 */
#ifndef __SCMN_MFAL_H__9382493743892439028493028493024__
#define __SCMN_MFAL_H__9382493743892439028493028493024__

#ifdef __cplusplus
extern "C"
{
#endif

/* file descriptor */
typedef void *                 SCMN_MFAL_FD;

/* file position */
#if defined(WM6) || defined(WIN32)
typedef __int64                SCMN_MFAL_FPOS;
#else
typedef long long              SCMN_MFAL_FPOS;
#endif

#define SCMN_MFAL_OMODE_WRITABLE            (1<<0)
#define SCMN_MFAL_OMODE_CACHABLE            (1<<1)

typedef struct SCMN_MFAL SCMN_MFAL;

/* function prototypes */
typedef SCMN_MFAL_FD (*SCMN_MFAL_OPEN)(SCMN_MFAL * mfal, unsigned long omode);
typedef void (*SCMN_MFAL_CLOSE)(SCMN_MFAL_FD fd);
typedef int (*SCMN_MFAL_READ)(SCMN_MFAL_FD fd, void * buf, int size);
typedef int (*SCMN_MFAL_WRITE)(SCMN_MFAL_FD fd, void * buf, int size);
typedef int (*SCMN_MFAL_GETPOS)(SCMN_MFAL_FD fd, SCMN_MFAL_FPOS * fpos);
typedef int (*SCMN_MFAL_SETPOS)(SCMN_MFAL_FD fd, SCMN_MFAL_FPOS * fpos);
typedef int (*SCMN_MFAL_SKIP)(SCMN_MFAL_FD fd, SCMN_MFAL_FPOS * fpos);
typedef int (*SCMN_MFAL_GETSIZE)(SCMN_MFAL_FD fd, SCMN_MFAL_FPOS * fpos);

/* file I/O interface structure */
struct SCMN_MFAL
{
	/* function address table */
	SCMN_MFAL_OPEN          open;
	SCMN_MFAL_CLOSE         close;
	SCMN_MFAL_READ          read;
	SCMN_MFAL_WRITE         write;
	SCMN_MFAL_GETPOS        getpos;
	SCMN_MFAL_SETPOS        setpos;
	SCMN_MFAL_SKIP          skip;
	SCMN_MFAL_GETSIZE       getsize;
	void                  * data;
};

#define SCMN_MFAL_BT_MDATA       0
#define SCMN_MFAL_BT_HEADER      1
#define SCMN_MFAL_BT_INDEX       2

typedef struct
{
	int                     type;
	SCMN_MFAL_FPOS          pos;
	SCMN_MFAL_FPOS          size;
} SCMN_MFAL_BLK;

#ifdef __cplusplus
}
#endif

#endif /* __SCMN_MFAL_H__9382493743892439028493028493024__ */

