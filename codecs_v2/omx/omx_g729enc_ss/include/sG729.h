#ifndef __SG729_CODEC_H__			/* TEMPLATE : Protection multiple Include */
#define __SG729_CODEC_H__			/* TEMPLATE : Protection multiple Include */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* return values */ 
#define SG729_OK			(0)
#define SG729_ERROR			(-1)
#define __MI_USE__ 

typedef struct{
	unsigned char *p_in;
	short *p_out;
} sG729D_INFO;


typedef struct{
	short *p_in;
	unsigned char *p_out;
} sG729E_INFO;


#ifdef __MI_USE__ //Y.H.Won 090520 - multiple instance
void *sG729CreateDec(sG729D_INFO* sG729D);
short sG729DecFrame(void *sG729D_handler, sG729D_INFO* sG729D);
short sG729DeleteDec(void *sG729D_handler, sG729D_INFO* sG729D);

void *sG729CreateEnc(sG729E_INFO* sG729E);
short sG729EncFrame(void *sG729E_handler, sG729E_INFO* sG729E);
short sG729DeleteEnc(void *sG729E_handler, sG729E_INFO* sG729E);
#else
void sG729CreateDec(sG729D_INFO* sG729D);
short sG729DecFrame(sG729D_INFO* sG729D);
short sG729DeleteDec(sG729D_INFO* sG729D);

void sG729CreateEnc(sG729E_INFO* sG729E);
short sG729EncFrame(sG729E_INFO* sG729E);
short sG729DeleteEnc(sG729E_INFO* sG729E);
#endif
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SG729_CODEC_H__ */
