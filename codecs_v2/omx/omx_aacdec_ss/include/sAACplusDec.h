#ifndef _SAACPLUS_DEC_H_
#define  _SAACPLUS_DEC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define _IMPORT_AACPLUS_DEC_

#ifdef _IMPORT_AACPLUS_DEC_
#define _EXT		extern
#else
#define _EXT		 
#endif

#include "scmn_audb.h"
#include "scmn_bitb.h"
#include "scmn_mtype.h"
#include "sAACD_err.h"


/* Codec Specific Pre-define Value */
#define SAACD_MAX_INPUT_BUF_SIZE	 (1024<<2)
#define SAACD_MIN_OUTPUT_BUF_SIZE    (2*2048*2)


//#include "sAACplusDec2.h" // yesjoon 090616
/* identifier */
typedef void *                       SAACD;

/*! Decoder configure information structure */
typedef struct
{
	/*! 0: no channel packing, 1: every channels are packed into channel 0 */
	int               use_pack;

	/*! channel normalizing.\n
	0: no normalizing \n
	1: 1 channel normalizing \n
	2: 2 channel normalizing
	*/
	int               use_normchan;

	/*! bits per sample normalizing.\n
	0: no normalizing \n
	1: 8-bit normalizing \n
	2: 16-bit normalizing \n
	3: 24-bit normalizing
	*/
	int               use_normbits;

	/*! 0: No acceleration (S/W), 1: Uses Acceleration (H/W) */
	int               use_accel;

	/* 1: only LC profile supported, 0: other tools (like SBR/PS) supported */
	int               use_lc_only;

	/* default sampling frequency */
	int               hz;

	/* default number of channels */
	int               chan;
} SAACD_INIT_DSC;

/*! Decoder output status information structure */
typedef struct
{
	/*! a decoded frame is available? */
	int               fa;

	/*! byte size of decoded bitstream (read size of bitstream) */
	int               read;

	/*! frame number increased whenever decoding a frame. */
	unsigned long     fn;

	/*! number of channel. this value is not influenced by normalizing */
	int               chan;

	/*! bit per sample. this value is not influenced by normalizing 
		Note: set by the application, not by the decoder 
	*/
	int               bits;

	/*! sampling frequency */
	int               hz;

	int				  sbr_on;

	int				  ps_on;
} SAACD_STAT;


/* common functions */
_EXT int saac_init(void);
_EXT void saac_deinit(void);

_EXT int saacd_reset(SAACD id);
_EXT SAACD saacd_create(SAACD_INIT_DSC* init_dsc, int *err);
_EXT int saacd_decode(SAACD id, SCMN_BITB * bitb, SCMN_AUDB * audb, SAACD_STAT * stat);
_EXT int saacd_info(SAACD id, SCMN_BITB * bitb, SCMN_AUDB * audb, SAACD_STAT * stat);
_EXT void saacd_delete(SAACD id);


_EXT void AllocDecBuf(int);
_EXT void FreeDecBuf(int);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

