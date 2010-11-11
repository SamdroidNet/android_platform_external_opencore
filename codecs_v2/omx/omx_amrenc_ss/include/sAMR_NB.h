#ifndef _SAMR_NB_H_
#define _SAMR_NB_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define 	MODE_MR475 0
#define     MODE_MR515 1            
#define		MODE_MR59  2
#define		MODE_MR67  3
#define		MODE_MR74  4
#define		MODE_MR795 5
#define		MODE_MR102 6
#define		MODE_MR122 7

#define		DTX_OFF 0
#define		DTX_ON  1

/* return values */ 
#define SAMR_NB_NO_ERROR		(0)
#define SAMR_NB_ERR_TOC_FLAG    (-1)

#ifdef LINUX
#define ALIGN  __attribute__((align(4)))
#else
#define ALIGN
#endif
typedef struct {
 short * p_in;
 ALIGN unsigned char * p_out;
 short	rate;
 short	dtx;
 short  output_size;
} sAMR_NB_Enc_Struct;

typedef struct {
 ALIGN unsigned char * p_in;
 short * p_out;
 short  input_size;
} sAMR_NB_Dec_Struct;

void *sAMR_NBCreateEnc(int*,sAMR_NB_Enc_Struct* enc_struct);
void sAMR_NBDeleteEnc(void*,sAMR_NB_Enc_Struct* enc_struct);
int sAMR_NBEncFrame(void*,sAMR_NB_Enc_Struct* enc_struct);

void *sAMR_NBCreateDec(int*,sAMR_NB_Dec_Struct* dec_struct);
void sAMR_NBDeleteDec(void*,sAMR_NB_Dec_Struct* dec_struct);
int sAMR_NBDecFrame(void*,sAMR_NB_Dec_Struct* dec_struct);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
