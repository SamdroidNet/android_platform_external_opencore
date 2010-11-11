/* --------------------------------------------------------------------------------
 * Copyright (C) 2009 Samsung
 *
 * @Project/Module Name    : EVRC Encoder
 * @File Name              : AV Codec MP3 encoder header file.
 * @File Description       : File contains definitions of mp3 encoder library functions.
 * @Author                 : Sudhir Vyas, Renuka V
 * @Created Date           : 05-06-2009
 * @Modification History
 * Version:                Date:                By:                Change:
 * ----------------------------------------------------------------------------------
 */

#ifndef        _SMP3_ENC_H_
#define        _SMP3_ENC_H_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#define MAXMP3BUFFER            1988

// SWMA_DEC identifier
typedef void *                  sMP3E;

// Return Value
#define SMP3E_OK                0
#define SMP3E_ERR               -1
#define SMP3E_ERR_MEM0ALLOC     -2
#define SMP3E_ERR_MEMALLOC      -3

// codec specific structure
typedef struct {
     unsigned char*     p_in;
     unsigned char*     p_out;
     int                bitrate;
     int                n_ch;
     int                downmix;
     int                samplerate;
     int                mpeg_ver;
     int                n_read;
} sMP3_Enc_Struct;

// API functions
#ifdef __MI_USE__ // For multiple instance
extern void *sMP3CreateEnc(sMP3_Enc_Struct* enc_struct);
extern int  sMP3DeleteEnc(void *sMP3E_handler, sMP3_Enc_Struct* enc_struct);
extern int  sMP3EncFrame(void *sMP3E_handler, sMP3_Enc_Struct* enc_struct);
#else
extern int  sMP3CreateEnc( sMP3_Enc_Struct* enc_struct);
extern int  sMP3DeleteEnc( sMP3_Enc_Struct* enc_struct);
extern int  sMP3EncFrame( sMP3_Enc_Struct* enc_struct);
#endif

extern void sMP3E_Ver(char* ver_str);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif

