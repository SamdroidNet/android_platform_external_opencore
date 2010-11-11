/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * @Project/Module Name	: EVRC Encoder
 * @File Name			: AV Codec EVRC encoder header file.
 * @File Description	: File contains definitions of evrc encoder library functions.
 * @Integrated By		: Sudhir Vyas.				 
 * @Created Date		: 		     
 * @Modification History
 * Version:				Date:				By:				Change:		
 * -------------------------------------------------------------------
 */

#define  SPEECH_BUFFER_LEN        160
#define  BITSTREAM_BUFFER_LEN     12	/*  data + 1 word for rate  */

/* return values */ 
#define SEVRC_OK			(0)
#define SEVRC_ERROR			(-1)

extern "C" {

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

void	sEVRCCreateDec(sEVRC_INFO* sEVRCD);
short	sEVRCDecFrame(sEVRC_INFO* sEVRCD);
short	sEVRCDeleteDec(sEVRC_INFO* sEVRCD);

void	sEVRCCreateEnc(sEVRC_INFO* sEVRCE);
short	sEVRCEncFrame(sEVRC_INFO* sEVRCE);
short	sEVRCDeleteEnc(sEVRC_INFO* sEVRCE);
}