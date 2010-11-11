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
 * @file scmn_mtype.h
 *
 * @brief Declarations of various media (audio/video) types
 *
 * @date 2008-10-06 Kwang Pyo Choi creates code
 *
 * @author Kwang Pyo Choi at Samsung Electronics
 *****************************************************************************
 */
#ifndef __SCMN_MTYPE_H__902840328748732894839028439204832__
#define __SCMN_MTYPE_H__902840328748732894839028439204832__

#ifdef __cplusplus
extern "C"
{
#endif

/* video types **************************************************************/
/*! MPEG-1 video */
#define SCMN_MT_VID_MPG1             100
/*! MPEG-2 video */
#define SCMN_MT_VID_MPG2             101
/*! MPEG-4 part 2 video */
#define SCMN_MT_VID_MPG4             102
/*! H.263 */
#define SCMN_MT_VID_H263             103
/*! H.264/MPEG-4 part 10 */
#define SCMN_MT_VID_H264             104
/*! Microsoft WMV7 */
#define SCMN_MT_VID_WMV7             105
/*! Microsoft WMV8 */
#define SCMN_MT_VID_WMV8             106
/*! Microsoft WMV9 */
#define SCMN_MT_VID_WMV9             107
/*! VC-1 */
#define SCMN_MT_VID_VC1              SCMN_MT_VID_WMV9
/*! On2 VP6 */
#define SCMN_MT_VID_VP6              108
/*! On2 VP7 */
#define SCMN_MT_VID_VP7              109
/*! Divx 3.11 */
#define SCMN_MT_VID_DIV3             110
/*! Sorenson Spark (H.263) */
#define SCMN_MT_VID_SORENSON_SPARK   111
#define SCMN_MT_VID_SORENSON_H263    SCMN_MT_VID_SORENSON_SPARK
/*! Theora */
#define SCMN_MT_VID_THEORA           112

/*! Unknown video type */
#define SCMN_MT_VID_UNKNOWN          299

/* audio types **************************************************************/
/*! Linear PCM */
#define SCMN_MT_AUD_PCM              300
/*! A-Law PCM */
#define SCMN_MT_AUD_PCM_A            301
/*! Mu-Law PCM */
#define SCMN_MT_AUD_PCM_M            302
/*! MPEG-1 audio layer 1 */
#define SCMN_MT_AUD_MP1              303
/*! MPEG-1 audio layer 2 */
#define SCMN_MT_AUD_MP2              304
/*! MPEG-1 audio layer 3 */
#define SCMN_MT_AUD_MP3              305
/*! MPEG-4 AAC-LC & MAIN */
#define SCMN_MT_AUD_AAC              306
/*! Dolby Digital AC-3 */
#define SCMN_MT_AUD_AC3              307
/*! DTS Inc. DTS */
#define SCMN_MT_AUD_DTS              308
/*! AMR-NB */
#define SCMN_MT_AUD_AMR_NB           309
/*! AMR-WB */
#define SCMN_MT_AUD_AMR_WB           310
/*! QCELP */
#define SCMN_MT_AUD_QCP              311
/*! Microsoft WMA2 (WMA7/8/9) */
#define SCMN_MT_AUD_WMA              312
/*! Microsoft WMA9 Professional */
#define SCMN_MT_AUD_WMA9_PRO         313
/*! Microsoft WMA9 Lossless */
#define SCMN_MT_AUD_WMA9_LS          314
/*! Microsoft WMA9 Voice */
#define SCMN_MT_AUD_WMA9_VOICE       315
/*! Microsoft WMA10 Professional */
#define SCMN_MT_AUD_WMA10_PRO        316
/*! MPEG-4 BSAC */
#define SCMN_MT_AUD_BSAC             320
/*! EVRC */
#define SCMN_MT_AUD_EVRC             321
/*! FLAC */
#define SCMN_MT_AUD_FLAC             322
/*! Vorbis */
#define SCMN_MT_AUD_VORBIS           323
/*! Unknown audio type */
#define SCMN_MT_AUD_UNKNOWN          499

/* image types **************************************************************/
#define SCMN_MT_IMG_BMP              500
#define SCMN_MT_IMG_JPG              501
#define SCMN_MT_IMG_GIF              502
#define SCMN_MT_IMG_PNG              503
#define SCMN_MT_IMG_UNKNOWN          699

/* text types ***************************************************************/
#define SCMN_MT_TXT_ASCII            700
#define SCMN_MT_TXT_UTF16            701
#define SCMN_MT_TXT_UTF16BE          702
#define SCMN_MT_TXT_UTF8             703
#define SCMN_MT_TXT_UNICODE          SCMN_MT_TXT_UTF16
#define SCMN_MT_TXT_UNKNOWN          899

/* tag types ****************************************************************/
#define SCMN_MT_TAG_ID3              900
#define SCMN_MT_TAG_OGG              901
#define SCMN_MT_TAG_APE              902
#define SCMN_MT_TAG_FLAC             903
#define SCMN_MT_TAG_WMV              904
#define SCMN_MT_TAG_WMA              SCMN_MT_TAG_WMV
#define SCMN_MT_TAG_MP4              905
#define SCMN_MT_TAG_MKV              906
#define SCMN_MT_TAG_MKA              SCMN_MT_TAG_MKV

/* file types ***************************************************************/
/*! ISO base video file format: MP4, 3GP... */
#define SCMN_MT_FLE_ISO              2000
/*! AVI video file format */
#define SCMN_MT_FLE_AVI              2001
/*! WMV video file format */
#define SCMN_MT_FLE_WMV              2002
/*! MKV video file format */
#define SCMN_MT_FLE_MKV              2003
/*! WMA audio file format */
#define SCMN_MT_FLE_WMA              SCMN_MT_FLE_WMV
/*! MKA audio file format */
#define SCMN_MT_FLE_MKA              SCMN_MT_FLE_MKV
/*! FLV video file format */
#define SCMN_MT_FLE_FLV              2004
/*! MP1 audio file format */
#define SCMN_MT_FLE_MP1              SCMN_MT_AUD_MP1
/*! MP2 audio file format */
#define SCMN_MT_FLE_MP2              SCMN_MT_AUD_MP2
/*! MP3 audio file format */
#define SCMN_MT_FLE_MP3              SCMN_MT_AUD_MP3
/*! FLAC audio file */
#define SCMN_MT_FLE_FLAC             SCMN_MT_AUD_FLAC
/*! BMP imag file */
#define SCMN_MT_FLE_BMP              SCMN_MT_IMG_BMP
/*! JPEG image file */
#define SCMN_MT_FLE_JPG              SCMN_MT_IMG_JPG
/*! GIF image file */
#define SCMN_MT_FLE_GIF              SCMN_MT_IMG_GIF
/*! PNG image file */
#define SCMN_MT_FLE_PNG              SCMN_MT_IMG_PNG
/*! OGG file format */
#define SCMN_MT_FLE_OGG              2005
/*! WAV audio file format */
#define SCMN_MT_FLE_WAV              2006
/*! Unknown file format */
#define SCMN_MT_FLE_UNKNOWN          2199


/* macro to check media types ***********************************************/
#define SCMN_MT_IS_VID(t)           ((t)>=100 && (t)<300)
#define SCMN_MT_IS_AUD(t)           ((t)>=300 && (t)<500)
#define SCMN_MT_IS_IMG(t)           ((t)>=500 && (t)<700)
#define SCMN_MT_IS_TXT(t)           ((t)>=700 && (t)<900)

#ifdef __cplusplus
}
#endif

#endif /* __SCMN_MTYPE_H__902840328748732894839028439204832__ */

