
#ifndef __SG711_CODEC_H__			/* TEMPLATE : Protection multiple Include */
#define __SG711_CODEC_H__			/* TEMPLATE : Protection multiple Include */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//#include <stdio.h>		// DO Not Use Standard IO Library

/* sG711 Return Value */
#define SG711_OK               			0
#define SG711_OK_END               		1
#define SG711_ERR_LAWMODE         		-1


typedef struct { /* TEMPLATE : Do not include codec internal only except future reserve */

	short *inp_buf;
	unsigned char   *out_buf;	

	short block_size;
	char lawmode;


}sG711E_INFO;

typedef struct { /* TEMPLATE : Do not include codec internal only except future reserve */

	unsigned char   *inp_buf;	
	short *out_buf;

	short block_size;
	char lawmode;


}sG711D_INFO;


void  ulaw_compress (long lseg, short *linbuf, unsigned char *logbuf);
void  alaw_compress (long lseg, short *linbuf, unsigned char *logbuf);
void  ulaw_expand (long lseg, unsigned char *logbuf, short *linbuf);
void  alaw_expand (long lseg, unsigned char *logbuf, short *linbuf);

/* New API */
int sG711CreateDec(sG711D_INFO *sG711D);
void sG711DecFrame(sG711D_INFO *sG711D);
void sG711DeleteDec(sG711D_INFO *sG711D);

int sG711CreateEnc(sG711E_INFO *sG711E);
void sG711EncFrame(sG711E_INFO *sG711E);
void sG711DeleteEnc(sG711E_INFO *sG711E);

#ifdef __cplusplus
}
#endif // __cplusplus
#endif /* __SG711_CODEC_H__ */ 
