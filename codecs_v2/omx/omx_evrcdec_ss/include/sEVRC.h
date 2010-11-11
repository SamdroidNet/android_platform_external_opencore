/*======================================================================*/
/*         ..Defines.                                                   */
/*----------------------------------------------------------------------*/
#ifndef __SEVRC_CODEC_H__			/* TEMPLATE : Protection multiple Include */
#define __SEVRC_CODEC_H__			/* TEMPLATE : Protection multiple Include */
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
#define  SPEECH_BUFFER_LEN        160
#define  BITSTREAM_BUFFER_LEN      12	/*  data + 1 word for rate  */

/* return values */ 
#define SEVRC_OK			(0)
#define SEVRC_ERROR			(-1)


typedef struct{
    short *p_in;
    short *p_out;
	void *enc_s;
	void *dec_s;
	void *rate_mem;
	short rate;
	short beta;
	short ibuf_len;
	short obuf_len;
	short max_rate;
	short min_rate;
	short noise_suppression;
	short post_filter;
	long R[17];
} sEVRC_INFO;

void sEVRCCreateDec(sEVRC_INFO* sEVRCD);
short sEVRCDecFrame(sEVRC_INFO* sEVRCD);
short sEVRCDeleteDec(sEVRC_INFO* sEVRCD);

void sEVRCCreateEnc(sEVRC_INFO* sEVRCE);
short sEVRCEncFrame(sEVRC_INFO* sEVRCE);
short sEVRCDeleteEnc(sEVRC_INFO* sEVRCE);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif /* __SEVRC_CODEC_H__ */ 
