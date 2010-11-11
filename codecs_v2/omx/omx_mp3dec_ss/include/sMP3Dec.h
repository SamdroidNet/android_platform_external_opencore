#ifndef _SMP3_DEC_H_
#define  _SMP3_DEC_H_

/* Default Configuration */
#define FB_SIZE		1440 //Do not change this

/* SMP3_DEC identifier */
typedef void *                       sMP3D;

/* return values */ 
//Not Use Yet
#define SMP3_OK			(0)
#define SMP3_ERR_INVALID_ARG    (-1)
#define SMP3_ERR_INVALID_BS     (-2)
#define SMP3_ERR_INVALID_MEM     (-3)
#define SMP3_ERR_MEM_ALLOC     	(-4)
#define SMP3_ERR_UNKNOWN        (-100)

#define SMP3_IS_OK(ret)          ((ret)>=0)
#define SMP3_IS_ERR(ret)         ((ret)<0)

/* codec specific structure */ 
typedef struct{
	unsigned char 	*p_in;
	short		*p_out;
	int		read_bytes;
	int		n_ch;
	int		samplerate;
} mp3_dec_struct;

/* API functions */
/*
void sMP3CreateDec(mp3_dec_struct *dec_struct);
int sMP3DecFrame(mp3_dec_struct *dec_struct);
void sMP3DeleteDec(mp3_dec_struct *dec_struct);
void sMP3ResetDec(mp3_dec_struct *dec_struct);
*/

extern "C" {
extern void sMP3CreateDec(mp3_dec_struct *dec_struct);
} 
extern "C" {
extern int sMP3DecFrame(mp3_dec_struct *dec_struct);
} 
extern "C" {
extern void sMP3DeleteDec(mp3_dec_struct *dec_struct);
} 
extern "C" {
extern void sMP3ResetDec(mp3_dec_struct *dec_struct);
} 

#endif	/* _SMP3_DEC_H_ */
