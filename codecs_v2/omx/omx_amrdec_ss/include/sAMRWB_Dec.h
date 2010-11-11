#ifndef _SAMRWB_DEC_H_
#define  _SAMRWB_DEC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/* return values */ 
#define SAMRWB_OK			(0)
#define SAMRWB_ERROR        (-1)

typedef struct
{
    unsigned char *p_in;
    short *p_out;
    void *st;
    short mode;
    short frame_type;
    short bitstreamformat;
    void *rx_state;
} sAMRWD_INFO;

typedef struct {
	short prms_in[477];
	short reset_flag;
	short reset_flag_old;
} sAMR_WB_Global;

void *sAMRWBCreateDec(sAMRWD_INFO* sAMRWDec);
short sAMRWBDecFrame(void*, sAMRWD_INFO* sAMRWDec);
short sAMRWBDeleteDec(void*, sAMRWD_INFO* sAMRWDec);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif	/* _SAMRWB_DEC_H_ */