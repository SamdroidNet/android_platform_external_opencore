#ifndef _SAC3_DEC_H_
#define  _SAC3_DEC_H_


/* Default Configuration */


/* SWMA_DEC identifier */
typedef void *                       sAC3D;

/* sAC3DecFrame() Return Value */

#define AC3_NO_ERROR	 0
#define AC3_ERROR		 1
#define AC3_BUFFER_END	2		// modified by LKS @ 20080401
#define AC3_CRC_ERROR	3		// modified by LKS @ 20080401
#define AC3_STREAM_FAIL	4		// modified by LKS @ 20080401
#define AC3_CRC_ERR_HALF 5		// modified by LKS @ 20080401
#define AC3_BITSTREM_ERR 6 		// yesjoon 080730

/* codec specific structure */

typedef struct{
	unsigned char 		*pInBuff;
	unsigned char			*pInTempBuff;		// modified by LKS @ 20080403
	short				*pOutBuff;
	int					framesz;
	int					bound;			// yesjoon 080725
	int					InbuffSize;		// modified by LKS @ 20080402
	int					OutbuffSize;	// modified by LKS @ 20080402
	int					nReadByte;	// modified by LKS @ 20080401
	int					nOutPCM;	// modified by LKS @ 20080401
	int					stoutacmod; // yesjoon 080408
	int					flush_set;
}sAC3D_INFO;

/* API functions */
extern "C" {
extern void sAC3CreateDec(void);
}
extern "C" {
extern int sAC3DecFrame(sAC3D_INFO *);
}
extern "C" {
extern void sAC3DeleteDec(void);
}

#endif   /*  _SAC3_DEC_H_ */

