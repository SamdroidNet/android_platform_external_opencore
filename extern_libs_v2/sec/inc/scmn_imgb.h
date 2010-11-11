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
 * @file scmn_imgb.h
 *
 * @brief Definitions for Image/Video Buffer
 *
 * @date 2008-10-06 Kwang Pyo Choi creates code
 *
 * @author Kwang Pyo Choi at Samsung Electronics
 *****************************************************************************
 */
#ifndef __SCMN_IMGB_H__983284903289403824903284903289043__
#define __SCMN_IMGB_H__983284903289403824903284903289043__

#ifdef __cplusplus
extern "C"
{
#endif

/* color spaces **************************************************************/
/* YUV planar type */
#define SCMN_CS_YUV400              0 /* Y */
#define SCMN_CS_YUV420              1 /* Y:U:V 4:2:0 */
#define SCMN_CS_YUV422              2 /* Y:U:V 4:2:2 */
#define SCMN_CS_YUV444              3 /* Y:U:V 4:4:4 */
#define SCMN_CS_YV12                4 /* Y:V:U 4:2:0 */
#define SCMN_CS_I420                SCMN_CS_YUV420 /* Y:U:V */
#define SCMN_CS_IYUV                SCMN_CS_YUV420 /* Y:U:V */
#define SCMN_CS_YV16                5 /* Y:V:U 4:2:2 */
#define SCMN_CS_NV12                6
#define SCMN_CS_NV21                7
#define SCMN_CS_YUV422N             SCMN_CS_YUV422
#define SCMN_CS_YUV422W             8
#define SCMN_CS_GRAY                SCMN_CS_YUV400
#define SCMN_CS_Y800                SCMN_CS_YUV400
#define SCMN_CS_Y16                 9
#define SCMN_CS_GRAYA               10
#define SCMN_CS_NV12_T64X32         11 /* 64x32 Tiled NV12 type */

/* YUV pack type */
#define SCMN_CS_UYVY                100
#define SCMN_CS_YUYV                101
#define SCMN_CS_YUY2                SCMN_CS_YUYV
#define SCMN_CS_V422                SCMN_CS_YUYV
#define SCMN_CS_YUNV                SCMN_CS_YUYV
#define SCMN_CS_Y8A8                110

/* RGB pack type */
#define SCMN_CS_RGB565              200
#define SCMN_CS_BGR565              201

#define SCMN_CS_RGB555              210 /* 1 : R(5) : G(5) : R(5) */
#define SCMN_CS_BGR555              211

#define SCMN_CS_RGB666              300
#define SCMN_CS_BGR666              301

#define SCMN_CS_RGB888              400
#define SCMN_CS_BGR888              401

#define SCMN_CS_RGBA8888            500
#define SCMN_CS_BGRA8888            501
#define SCMN_CS_ARGB8888            502
#define SCMN_CS_ABGR8888            503

/* unknown color space */
#define SCMN_CS_UNKNOWN             1000

/* macro for color space *****************************************************/
#define SCMN_CS_IS_YUV(cs)          ((cs)>=0 && (cs)<200)
#define SCMN_CS_IS_YUV_PLANAR(cs)   ((cs)>=0 && (cs)<100)
#define SCMN_CS_IS_YUV_PACK(cs)     ((cs)>=100 && (cs)<200)
#define SCMN_CS_IS_RGB_PACK(cs)     ((cs)>=200 && (cs)<600)
#define SCMN_CS_IS_RGB16_PACK(cs)   ((cs)>=200 || (cs)<300)
#define SCMN_CS_IS_RGB18_PACK(cs)   ((cs)>=300 || (cs)<400)
#define SCMN_CS_IS_RGB24_PACK(cs)   ((cs)>=400 || (cs)<500)
#define SCMN_CS_IS_RGB32_PACK(cs)   ((cs)>=500 || (cs)<600)


/* max channel count *********************************************************/
#define SCMN_IMGB_MAX_PLANE         (4)

/* image buffer definition ***************************************************

    +------------------------------------------+ ---
    |                                          |  ^
    |     a[], p[]                             |  |
    |     +---------------------------+ ---    |  |
    |     |                           |  ^     |  |
    |     |<---------- w[] ---------->|  |     |  |
    |     |                           |  |     |  |
    |     |                           |        |
    |     |                           |  h[]   |  e[]
    |     |                           |        |
    |     |                           |  |     |  |
    |     |                           |  |     |  |
    |     |                           |  v     |  |
    |     +---------------------------+ ---    |  |
    |                                          |  v
    +------------------------------------------+ ---

    |<----------------- s[] ------------------>|
*/

typedef struct
{
	/* width of each image plane */
	int      w[SCMN_IMGB_MAX_PLANE];
	/* height of each image plane */
	int      h[SCMN_IMGB_MAX_PLANE];
	/* stride of each image plane */
	int      s[SCMN_IMGB_MAX_PLANE];
	/* elevation of each image plane */
	int      e[SCMN_IMGB_MAX_PLANE];
	/* user space address of each image plane */
	void   * a[SCMN_IMGB_MAX_PLANE];
	/* physical address of each image plane, if needs */
	void   * p[SCMN_IMGB_MAX_PLANE];
	/* color space type of image */
	int      cs;
	/* left postion, if needs */
	int      x;
	/* top position, if needs */
	int      y;
	/* to align memory */
	int      __dummy2;
	/* arbitrary data */
	int      data[16];
} SCMN_IMGB;

#ifdef __cplusplus
}
#endif

#endif /* __SCMN_IMGB_H__983284903289403824903284903289043__ */

