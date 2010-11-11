/****************************************************************************
 *                       Samsung Electronics Co. Ltd.                       *
 *                                                                          *
 *                           COPYRIGHT (C) 2008                             *
 *                          All Rights Reserved                             *
 *                                                                          *
 *                        A/V Codec Lab. R&D Center                         *
 *                 Telecommunication & Network Business                     *
 ****************************************************************************/

/*!
 *****************************************************************************
 * @file sthmb.h
 *
 * @brief API Declarations for Video Thumbnail Extractor
 *
 * @date 2008-10-03 Kwang Pyo Choi creates the code.
 *
 * @author Kwang Pyo Choi at Samsung Electronics
 *****************************************************************************
 */

#ifndef __STHMB_H__39208493024730284903284903284930243824739204839204__
#define __STHMB_H__39208493024730284903284903284930243824739204839204__

#ifdef __cplusplus
extern "C"
{
#endif

#include "scmn_base.h"

#if defined(WIN32) || defined(WM6) || defined(WCE6)
#define STHMB_API                    __declspec(dllexport)
#else
#define STHMB_API
#endif

/*! thumbnail extractor instance identifier */
typedef void *                        STHMB;
/*! time unit (100 nanosecond) */
typedef long long                     STHMB_TIME;

/*! maximum video stream count */
#define STHMB_MAX_SCNT_VID            4
/*! maximum image stream count */
#define STHMB_MAX_SCNT_IMG            4
/*! maximum audio stream count */
#define STHMB_MAX_SCNT_AUD            8

/* return values ************************************************************/
/*! operation success */
#define STHMB_OK                      (0)
/*! invalid argument */
#define STHMB_ERR_INVALID_ARG         (-1)
/*! not sufficient memory */
#define STHMB_ERR_NOT_SUFF_MEM        (-2)
/*! not supported yet */
#define STHMB_ERR_NOT_SUPPORTED       (-3)
/*! cannot open file */
#define STHMB_ERR_NOT_OPEN_FILE       (-4)
/*! invalid file type */
#define STHMB_ERR_INVALID_FILE_TYPE   (-5)
/*! invalid data (bitstream) */
#define STHMB_ERR_INVALID_DATA        (-6)
/*! there is no thumbnail image */
#define STHMB_ERR_NO_THUMB_IMG        (-7)
/*! not expected operation */
#define STHMB_ERR_NOT_EXPECTED        (-8)
/*! unknown error */
#define STHMB_ERR_UNKNOWN             (-100)

/* return value checking macro **********************************************/
#define STHMB_IS_OK(ret)              ((ret) >= 0)
#define STHMB_IS_ERR(ret)             ((ret) < 0)

/* thumbnail extracting option values ***************************************/
/*! search highlighting thumbnail instead of exact position */
#define STHMB_PARAM_OPT_SEARCH_HIGHLIGHT    (1<<0)
/*! stretch image into expected thumbnail size */
#define STHMB_PARAM_OPT_FIT_FULL            (1<<1)
/*! keep image ratio */
#define STHMB_PARAM_OPT_FIT_KEEP_RATIO      (1<<2)
/*! position of available image */
#define STHMB_PARAM_OPT_POS_VER_TOP         (1<<8)
#define STHMB_PARAM_OPT_POS_VER_CENTER      (1<<9)
#define STHMB_PARAM_OPT_POS_VER_BOTTOM      (1<<10)
#define STHMB_PARAM_OPT_POS_HOR_LEFT        (1<<11)
#define STHMB_PARAM_OPT_POS_HOR_CENTER      (1<<12)
#define STHMB_PARAM_OPT_POS_HOR_RIGHT       (1<<13)
#define STHMB_PARAM_OPT_LINEAR_WRITE        (1<<14)

/*! dither output image */
#define STHMB_PARAM_OPT_DITHER              (1<<31)

/* DRM types ****************************************************************/
#define STHMB_DRM_NONE                      (0) /* No-DRM */
#define STHMB_DRM_DIVX                      (1) /* DivX DRM */
#define STHMB_DRM_WM                        (2) /* Windows Media DRM */

/* detail information for each media ****************************************/
/* for H.264 */
#define STHMB_DETAIL_VID_H264_LEVEL(detail)            ((detail) & 0xFF)
#define STHMB_DETAIL_VID_H264_PROFILE_BASELINE         (1<<8)
#define STHMB_DETAIL_VID_H264_PROFILE_MAIN             (1<<9)
#define STHMB_DETAIL_VID_H264_PROFILE_EXTENDED         (1<<10)
#define STHMB_DETAIL_VID_H264_PROFILE_HIGH             (1<<11)
#define STHMB_DETAIL_VID_H264_PROFILE_HIGH10           (1<<12)
#define STHMB_DETAIL_VID_H264_PROFILE_HIGH422          (1<<13)
#define STHMB_DETAIL_VID_H264_PROFILE_HIGH444          (1<<14)

/* for MPEG-4 */
#define STHMB_DETAIL_VID_MPG4_LEVEL(detail)            ((detail) & 0xF)
#define STHMB_DETAIL_VID_MPG4_PROFILE_SIMPLE           (1<<4)
#define STHMB_DETAIL_VID_MPG4_PROFILE_ADVANCED_SIMPLE  (1<<5)
#define STHMB_DETAIL_VID_MPG4_PROFILE_ETC              (1<<15)
#define STHMB_DETAIL_VID_MPG4_SVH                      (1<<16)
#define STHMB_DETAIL_VID_MPG4_LOWDELAY                 (1<<17)
#define STHMB_DETAIL_VID_MPG4_INTERLACED               (1<<18)
#define STHMB_DETAIL_VID_MPG4_OBMC                     (1<<19)
#define STHMB_DETAIL_VID_MPG4_MPEGQUANT                (1<<20)
#define STHMB_DETAIL_VID_MPG4_QPEL                     (1<<21)
#define STHMB_DETAIL_VID_MPG4_GMC                      (1<<22)


/* for MP1, MP2, MP3 audio */
#define STHMB_DETAIL_AUD_MP123_VBR                     (1<<0)

/* for JPEG image */
#define STHMB_DETAIL_IMG_JPG_PROGRESSIVE               (1<<0)

/* for PNG image */
#define STHMB_DETAIL_IMG_PNG_ANIMATED                  (1<<0)

/*! video information *******************************************************/
typedef struct
{
	/*! media type (video codec type) */
	int              mt;
	/*! width of video */
	int              w;
	/*! height of video */
	int              h;
	/*! frame per sec */
	int              fps;
	/*! total frame count */
	unsigned long    frames;
	/*! average bits per sec (bit) */
	int              bitrate;
	/*! detail media information */
	unsigned long    details;
} STHMB_VINFO;

/*! image information *******************************************************/
typedef struct
{
	/*! media type (image codec type) */
	int              mt;
	/*! width of image */
	int              w;
	/*! height of image */
	int              h;
	/*! detail media information */
	unsigned long    details;
} STHMB_IINFO;

/*! audio information *******************************************************/
typedef struct
{
	/*! media type (audio codec type) */
	int              mt;
	/*! sampling frequency (Hz) */
	int              hz;
	/*! audio channel count */
	int              chans;
	/*! bits per sample (bit) */
	int              bits;
	/*! average bits per sec (bit) */
	int              bitrate;
	/*! detail media information */
	unsigned long    details;
} STHMB_AINFO;

/*! media information *******************************************************/
typedef struct
{
	/*! video stream count */
	int              scnt_vid;
	/*! image stream count */
	int              scnt_img;
	/*! audio stream count */
	int              scnt_aud;
	/*! playback time (100 nanosecond unit) */
	STHMB_TIME       duration;
	/*! video informations */
	STHMB_VINFO      vinfo[STHMB_MAX_SCNT_VID];
	/*! image informations */
	STHMB_IINFO      iinfo[STHMB_MAX_SCNT_IMG];
	/*! audio informations */
	STHMB_AINFO      ainfo[STHMB_MAX_SCNT_AUD];
	/*! media file type */
	int              mt;
	/*! tag information, if exists */
	SCMN_MTAG      * mtag;
	/*! DRM type */
	int              drm;
} STHMB_MINFO;

/*! thumbnail extracting parameter ******************************************/
typedef struct
{
	/* stream index */
	int              sidx;
	/* expected extracting position in time (100 nanosecond unit) */
	STHMB_TIME       time;
	/* extracting options */
	unsigned long    opt;
} STHMB_PARAM;

/*! thumbnail extracting status *********************************************/
typedef struct
{
	/* time of the actual extracted postion (100 nanosecond unit) */
	STHMB_TIME       time;
} STHMB_STAT;

/*!
 *****************************************************************************
 * @brief Open a media file
 *
 * NOTE: This function should be called before calling the other functions.
 *
 * @param fname [in] media file path name.
 * @param minfo [out] media information in the file.
 * @param err [out] If function returns error, this variable is set to the error
 *            value. It can be NULL if no needs.
 * @return If success, return an identifier. Else, returns NULL.
 *****************************************************************************
 */
STHMB_API STHMB sthmb_open(void * fname, STHMB_MINFO * minfo, int * err);

/*!
 *****************************************************************************
 * @brief Open a media file by using SCMN_MFAL
 *
 * NOTE: This function should be called before calling the other functions.
 *
 * @param mfal [in] SCMN_MFAL structure.
 * @param minfo [out] media information in the file.
 * @param err [out] If function returns error, this variable is set to the error
 *            value. It can be NULL if no needs.
 * @return If success, return an identifier. Else, returns NULL.
 *****************************************************************************
 */
STHMB_API STHMB sthmb_open_mfal(SCMN_MFAL * mfal, STHMB_MINFO * minfo, int * err);

/*!
 *****************************************************************************
 * @brief Open a media file by using SCMN_BITB
 *
 * NOTE: This function should be called before calling the other functions.
 *
 * @param bitb [in] address of SCMN_BITB
 * @param minfo [out] media information in the file.
 * @param err [out] If function returns error, this variable is set to the error
 *            value. It can be NULL if no needs.
 * @return If success, return an identifier. Else, returns NULL.
 *****************************************************************************
 */
STHMB_API STHMB sthmb_open_bitb(SCMN_BITB * bitb, STHMB_MINFO * minfo, int * err);

/*!
 *****************************************************************************
 * @brief Extract a thumbnail image
 *
 * NOTE:
 *
 * @param id [in] STHMB identifier.
 * @param param [in] extracting parameter.
 * @param imgb [in, out] image buffer to contain decoded image.
 *           The expected thumbnail size (width, height, stride) and color space
 *           should be set in the structure before calling this function.
 *           And image buffer should be allocated before calling this function.
 * @param stat [out] status values after extracting. It can be NULL.
 *
 * @return If success, return STHMB_OK. Else, returns error code.
 *****************************************************************************
 */
STHMB_API int sthmb_extract(STHMB id, STHMB_PARAM * param, SCMN_IMGB * imgb,
	STHMB_STAT * stat);

/*!
 *****************************************************************************
 * @brief Close media file
 *
 * NOTE: After calling this function, the identifier cannot be used anymore.
 *
 * @param id [in] STHMB identifier.
 *****************************************************************************
 */
STHMB_API void sthmb_close(STHMB id);

/*!
 *****************************************************************************
 * @brief Get version string
 *
 * @param None
 *****************************************************************************
 */
STHMB_API char * sthmb_version(void);

#ifdef __cplusplus
}
#endif

#endif /* __STHMB_H__39208493024730284903284903284930243824739204839204__ */

