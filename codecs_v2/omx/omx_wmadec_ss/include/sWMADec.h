#ifndef _SWMA_DEC_H_
#define  _SWMA_DEC_H_


/* Default Configuration */

#define WMA_MAX_DATA 128
#define MAX_SAMPLES 16384
#define DATA_OBJECT_SIZE 50

/* SWMA_DEC identifier */
typedef void *                       sWMAD;

/* sWMADecode() Return Value */

#define SWMAD_OK               			0
#define SWMAD_OK_DECODED_END			1	
#define SWMAD_ERR_BAD_ARG				2
#define SWMAD_ERR_BAD_ASF_HEADER		3
#define SWMAD_ERR_BAD_PACKET_HEADER		4
#define SWMAD_ERR_BROKEN_FRAME			5
#define SWMAD_OK_NO_MORE_FRAMES			6
#define SWMAD_ERR_BAD_SAMPLINGRATE		7
#define SWMAD_ERR_BAD_NUM_OF_CH			8
#define SWMAD_ERR_BAD_VER_NUM			9	
#define SWMAD_ERR_BAD_WEIGHTING_MODE	10
#define SWMAD_ERR_BAD_PACKETIZATIOIN	11
#define SWMAD_ERR_BAD_DRM_TYPE			12
#define SWMAD_ERR_DRM_FAILED			13
#define SWMAD_ERR_DRM_UNSUPPORTED		14
#define SWMAD_ERR_DEMO_EXPIRED			15
#define SWMAD_ERR_BAD_STATE				16
#define SWMAD_ERR_INTERNAL           	17          
#define SWMAD_ERR_NOMORE_DATA			18
#define SWMAD_ERR_BAD_FORMAT			19
#define SWMAD_ERR_DIVIDEDBYZERO			20		

/* SWMAD_INFO flag Value */

#define SWMAD_STATUS_NORMAL0			0
#define SWMAD_STATUS_NORMAL1			1
#define SWMAD_STATUS_NEED_MORE_DECODING	2
#define SWMAD_STATUS_NORMAL3			3
#define SWMAD_STATUS_NORMAL4			4
#define SWMAD_STATUS_NORMAL5			5
#define PAYLOAD_DEC


/* codec specific structure */ 
#ifdef PAYLOAD_DEC
typedef struct{
	unsigned long int	pBuff_cnt;
	int				Ch_x_Byte_per_sample;
	int 				flag;
	unsigned long    	NumSmpls;	
	short				*pOutBuff;
	unsigned char 	*pInBuff;	

		/* ASF header */
	unsigned long int   	cbPacketSize;

		/* audio prop */
	unsigned short    	nVersion;			// don't need to care about
	unsigned short    	wFormatTag;
	unsigned short    	nChannels;
	unsigned long int   	nSamplesPerSec;
	unsigned long int   	nAvgBytesPerSec;
	unsigned long int   	nBlockAlign;
	unsigned short    	wBitsPerSample;      // container size

	unsigned long int   	nSamplesPerBlock;
	unsigned short    	nEncodeOpt;

	long int 			 	bHasDRM;    
	unsigned int 		NumDecodedSmpls;	

	void *				handle_MI;

}SWMAD_INFO;
#else
typedef struct{
	unsigned char 		*pInBuff;
	short				*pOutBuff;
	unsigned long int	pBuff_cnt;
	int		     		Ch_x_Byte_per_sample;
	int 				flag;
	unsigned long       NumSmpls;

	unsigned int 		NumDecodedSmpls;	
	int					BlockAlign;
	int					nChannel;
	int					Samplerate;
	int					AvgBytePerSec;
	int					BitsPerSample;	    

	void *				handle_MI;

}SWMAD_INFO;
#endif

/* API functions */
extern "C" {
extern int sWMACreateDec(SWMAD_INFO *dec_struct);
}
extern "C" {
extern int sWMAResetDec(SWMAD_INFO *dec_struct);
}
extern "C" {
extern int sWMADecode(SWMAD_INFO *dec_struct);
}
extern "C" {
extern void sWMADeleteDec(SWMAD_INFO *dec_struct); 
}

#endif   /*  _SWMA_DEC_H_ */

