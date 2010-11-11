/*
**
** Copyright 2008, The Android Open Source Project
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

//#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "MetadataDriver"
#include <utils/Log.h>

#include <media/thread_init.h>
#include <core/SkBitmap.h>
#include <private/media/VideoFrame.h>

#include "metadatadriver.h"

/* Mobile Media Lab. Start */
#define USE_DMC_GALLERY 

#ifdef USE_DMC_GALLERY
#define ENABLE_LOG 0
#define MAX_VIDEO_WIDTH 720
#define MAX_VIDEO_HEIGHT 480
#define DEFAULT_VIDEO_WIDTH  352
#define DEFAULT_VIDEO_HEIGHT 288

#if ENABLE_LOG 
#define THUMB_LOG LOGI
#else
#define THUMB_LOG {}
#endif

#include "sthmb.h"
#endif
/* Mobile Media Lab. End */

using namespace android;

const char* MetadataDriver::ALBUM_ART_KEY = "graphic";

/* Mobile Media Lab. Start */
#ifdef USE_DMC_GALLERY
int tbl_supported_mt_vid[] = {
	SCMN_MT_VID_H263,
	SCMN_MT_VID_H264,		
	SCMN_MT_VID_VC1,
	SCMN_MT_VID_MPG4,
	SCMN_MT_VID_SORENSON_H263
};

int tbl_supported_mt_aud[] = {
	SCMN_MT_AUD_AAC,
	SCMN_MT_AUD_AC3,
	SCMN_MT_AUD_AMR_NB,
	SCMN_MT_AUD_AMR_WB,
	SCMN_MT_AUD_MP3,
	SCMN_MT_AUD_PCM,
	SCMN_MT_AUD_WMA,
	SCMN_MT_AUD_EVRC,
	SCMN_MT_AUD_QCP
};

static int isPlayableMedia(STHMB_MINFO minfo)
{
	int mt_vid, mt_aud;
	unsigned int detail_vid;
	int i=0;
	int l=sizeof(tbl_supported_mt_vid)/sizeof(tbl_supported_mt_vid[0]);
	int iVidFound = 0;

	// check the number of video streams
	if(minfo.scnt_vid < 1)
		return -1;

	mt_vid = minfo.vinfo[0].mt;
	detail_vid = minfo.vinfo[0].details;
	
	while(i < l)
	{
		if(mt_vid == tbl_supported_mt_vid[i])
		{
			switch(mt_vid)
			{
				case SCMN_MT_VID_H264:
					if((detail_vid & STHMB_DETAIL_VID_H264_PROFILE_BASELINE) != 0)
					{
						iVidFound = 1;
						break;
					}
					else 
					{
						return -1;
					}
					iVidFound = 1;
					break;
	
				case SCMN_MT_VID_MPG4:
					// check the exceptional cases
					if( (STHMB_DETAIL_VID_MPG4_GMC & detail_vid) != 0)
					{
						return -1;
					}
					else if( (STHMB_DETAIL_VID_MPG4_INTERLACED & detail_vid) != 0)
					{
						return -1;
					}
					else if( (STHMB_DETAIL_VID_MPG4_OBMC & detail_vid) != 0)
					{
						return -1;
					}						
					else if( (STHMB_DETAIL_VID_MPG4_QPEL & detail_vid) != 0)
					{
						return -1;
					}		
					iVidFound = 1;
					break;		
					
				default:
					iVidFound = 1;
					break;

			}
			break;
		}
		i++;
	}

	if( 0 == iVidFound )
		return -1;

	// check the number of audio streams
	if(minfo.scnt_aud < 1)
		return 1;

	mt_aud = minfo.ainfo[0].mt;
	i=0;
	l=sizeof(tbl_supported_mt_aud)/sizeof(tbl_supported_mt_aud[0]);	
	while(i < l)
	{
		if(mt_aud == tbl_supported_mt_aud[i])
			return 1;

		i++;
	}

	return -1;	
}

static VideoFrame* save_img_to_videoframe(SCMN_IMGB* img, int pos, int add)
{
	VideoFrame *videoFrame;
	unsigned char *p;
	int i, j;
	int tmpLen;

	videoFrame = new VideoFrame;
	if( NULL == videoFrame )
		return NULL;

	int w = videoFrame->mWidth = (uint32_t)img->w[0];
	int h = videoFrame->mHeight = (uint32_t)img->h[0];
	int dw = videoFrame->mDisplayWidth = (uint32_t)img->w[0];
	int dh = videoFrame->mDisplayHeight =(uint32_t) img->h[0];

	if(img->cs == SCMN_CS_RGB565 || img->cs == SCMN_CS_BGR565 )
	{
		videoFrame->mSize = w*h<<1;
		videoFrame->mData = (unsigned char*)oscl_malloc(videoFrame->mSize);	
		if( NULL == videoFrame->mData )
		{
			delete videoFrame;
			return NULL;
		}
		p = (unsigned char *)img->a[0];
		tmpLen = 0;
		for(i=0; i<img->h[0]; i++)
		{
			oscl_memcpy(videoFrame->mData + tmpLen, p, img->w[0]<<1 );
			p += img->s[0];
			tmpLen += img->s[0];
		}
	}
	else
	{
		delete videoFrame;
		videoFrame = NULL;
		return NULL;
	}	

    return videoFrame;
}

static void img_free(SCMN_IMGB * img)
{
	int i;

	for(i=0; i<4; i++)
	{
		if(img->a[i]) oscl_free(img->a[i]);
	}
}

static int img_alloc(int cs, int w, int h, SCMN_IMGB * img)
{
	oscl_memset(img, 0, sizeof(SCMN_IMGB));

	img->cs = cs;

	if(cs == SCMN_CS_RGB565 || cs == SCMN_CS_BGR565)
	{
		img->w[0] = w;
		img->h[0] = h;
		img->s[0] = w<<1;
		
		img->a[0] = (char*)oscl_malloc((w*h)<<1);
		if(NULL == img->a[0])
			return -1;
		if (img->a[0] == NULL)
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
	return 0;
}
#endif
/* Mobile Media Lab. End */

const char* MetadataDriver::METADATA_KEYS[NUM_METADATA_KEYS] = {
        "tracknumber",
        "album",
        "artist",
        "author",
        "composer",
        "date",
        "genre",
        "title",
        "year",
        "duration",
        "num-tracks",
        "drm/is-protected",
        "track-info/codec-name",
        "rating",
        "comment",
        "copyright",
        "track-info/bit-rate",
        "track-info/frame-rate",
        "track-info/video/format",
        "track-info/video/height",
        "track-info/video/width",
};

static void dumpkeystolog(PVPMetadataList list)
{
    LOGV("dumpkeystolog");
    uint32 n = list.size();
    for(uint32 i = 0; i < n; ++i) {
        LOGI("@@@@@ wma key: %s", list[i].get_cstr());
    }
}

MetadataDriver::MetadataDriver(uint32 mode): OsclActiveObject(OsclActiveObject::EPriorityNominal, "MetadataDriver")
{
    LOGV("constructor");
    mMode = mode;
    mUtil = NULL;
    mDataSource = NULL;
#if BEST_THUMBNAIL_MODE
    mLocalDataSource = NULL;
#endif
    mCmdId = 0;
    mContextObjectRefValue = 0x5C7A; // Some random number
    mContextObject = mContextObjectRefValue;
    mMediaAlbumArt = NULL;
/* SYS.LSI Start */
    mSharedFd = -1;
    mSharedFdLength = -1;
/* SYS.LSI End */
    mVideoFrame = NULL;
    for (uint32 i = 0; i < NUM_METADATA_KEYS; ++i) {
        mMetadataValues[i][0] = '\0';
    }
    LOGV("constructor: Mode (%d).", mMode);

}

/*static*/ int MetadataDriver::startDriverThread(void *cookie)
{
    LOGV("startDriverThread");
    MetadataDriver *driver = (MetadataDriver *)cookie;
    return driver->retrieverThread();
}

int MetadataDriver::retrieverThread()
{
    LOGV("retrieverThread");
    if (!InitializeForThread()) {
        LOGV("InitializeForThread fail");
        mSyncSem->Signal();
        return -1;
    }

	/* Mobile Media Lab. Start */
#ifndef USE_DMC_GALLERY
	OMX_Init();
#endif
	/* Mobile Media Lab. End */
    OsclScheduler::Init("PVAuthorEngineWrapper");
    mState = STATE_CREATE;
    AddToScheduler();
    RunIfNotReady();
    OsclExecScheduler *sched = OsclExecScheduler::Current();
    sched->StartScheduler();

    mSyncSem->Signal();  // Signal that doSetDataSource() is done.
    OsclScheduler::Cleanup();

	/* Mobile Media Lab. Start */
#ifndef USE_DMC_GALLERY
	OMX_Deinit();
#endif
	/* Mobile Media Lab. End */
    UninitializeForThread();
	
    return 0;
}


MetadataDriver::~MetadataDriver()
{
    LOGV("destructor");
    mCmdId = 0;
    delete mVideoFrame;
    mVideoFrame = NULL;
    delete mMediaAlbumArt;
    mMediaAlbumArt = NULL;
    delete mSyncSem;
    mSyncSem = NULL;
/* SYS.LSI Start */
    closeSharedFdIfNecessary(); 
/* SYS.LSI End */
}

const char* MetadataDriver::extractMetadata(int keyCode)
{
    LOGV("extractMetadata()");
    char *value = NULL;
    if (mMode & GET_METADATA_ONLY) {
        // Comparing int with unsigned int
        if (keyCode < 0 || keyCode >= (int) NUM_METADATA_KEYS) {
            LOGE("extractMetadata: Invalid keyCode: %d.", keyCode);
        } else {
            value = mMetadataValues[keyCode];
        }
    }
    if (value == NULL || value[0] == '\0') {
        return NULL;
    }
    return value;
}

MediaAlbumArt *MetadataDriver::extractAlbumArt()
{
    LOGV("extractAlbumArt");
    if (mMode & GET_METADATA_ONLY) {  // copy out
        if (mMediaAlbumArt != NULL && mMediaAlbumArt->mSize > 0) {
            return new MediaAlbumArt(*mMediaAlbumArt);
        } else {
            LOGE("failed to extract album art");
            return NULL;
        }
    }
    LOGE("extractAlbumArt: invalid mode (%d) to extract album art", mMode);
    return NULL;
}

// How to better manage these constant strings?
bool MetadataDriver::containsSupportedKey(const OSCL_HeapString<OsclMemAllocator>& str) const
{
    LOGV("containsSupportedKey");
    const char* cStr = str.get_cstr();
    for (uint32 i = 0; i < NUM_METADATA_KEYS; ++i) {
        if (strcasestr(cStr, METADATA_KEYS[i])) {
            return true;
        }
    }

    // Key "graphic" is a special metadata key for retrieving album art image.
    if (strcasestr(cStr, "graphic")) {
        return true;
    }
    return false;
}

// Delete unnecessary keys before retrieving the metadata values to avoid
// retrieving all metadata values for all metadata keys
void MetadataDriver::trimKeys()
{
    dumpkeystolog(mMetadataKeyList);
    mActualMetadataKeyList.clear();
    uint32 n = mMetadataKeyList.size();
    mActualMetadataKeyList.reserve(n);
    for (uint32 i = 0; i < n; ++i) {
        if (containsSupportedKey(mMetadataKeyList[i])) {
            mActualMetadataKeyList.push_back(mMetadataKeyList[i]);
        }
    }
    mMetadataKeyList.clear();
}

// Returns:
// 1. UNKNOWN_ERROR
//    a. If the metadata value(s) is too long, and cannot be hold in valueLength bytes
//    b. If nothing is found
// 2. OK
//    a. If metadata value(s) is found
status_t MetadataDriver::extractMetadata(const char* key, char* value, uint32 valueLength)
{
    bool found = false;
    value[0] = '\0';
    for (uint32 i = 0, n = mMetadataValueList.size(); i < n; ++i) {
        if (strcasestr(mMetadataValueList[i].key, key)) {
            found = true;
            switch(GetValTypeFromKeyString(mMetadataValueList[i].key)) {
                case PVMI_KVPVALTYPE_CHARPTR: {
                    uint32 length = oscl_strlen(mMetadataValueList[i].value.pChar_value) + 1;
                    if (length > valueLength) {
                        return UNKNOWN_ERROR;
                    }
                    oscl_snprintf(value, length, "%s", mMetadataValueList[i].value.pChar_value);
                    value[length] = '\0';
                    LOGV("value of char: %s.", mMetadataValueList[i].value.pChar_value);
                    break;
                }
                case PVMI_KVPVALTYPE_WCHARPTR: {
                    // Assume string is in UCS-2 encoding so convert to UTF-8.
                    uint32 length = oscl_strlen(mMetadataValueList[i].value.pWChar_value) + 1;
                    if (length > valueLength) {
                        return UNKNOWN_ERROR;
                    }
                    length = oscl_UnicodeToUTF8(mMetadataValueList[i].value.pWChar_value, length, value, valueLength);
                    value[length] = '\0';
                    LOGV("value of wchar: %ls.", mMetadataValueList[i].value.pWChar_value);
                    break;
                }
                case PVMI_KVPVALTYPE_UINT32:
                    oscl_snprintf(value, valueLength, "%d", mMetadataValueList[i].value.uint32_value);
                    value[valueLength] = '\0';
                    break;

                case PVMI_KVPVALTYPE_INT32:
                    oscl_snprintf(value, valueLength, "%d", mMetadataValueList[i].value.int32_value);
                    value[valueLength] = '\0';
                    break;

                case PVMI_KVPVALTYPE_UINT8:
                    oscl_snprintf(value, valueLength, "%d", mMetadataValueList[i].value.uint8_value);
                    value[valueLength] = '\0';
                    break;

                case PVMI_KVPVALTYPE_FLOAT:
                    oscl_snprintf(value, valueLength, "%f", mMetadataValueList[i].value.float_value);
                    value[valueLength] = '\0';
                    break;

                case PVMI_KVPVALTYPE_DOUBLE:
                    oscl_snprintf(value, valueLength, "%f", mMetadataValueList[i].value.double_value);
                    value[valueLength] = '\0';
                    break;

                case PVMI_KVPVALTYPE_BOOL:
                    oscl_snprintf(value, valueLength, "%s", mMetadataValueList[i].value.bool_value? "true": "false");
                    value[valueLength] = '\0';
                    break;

                default:
                    return UNKNOWN_ERROR;
            }
            break;
        }
    }
    return found? OK: UNKNOWN_ERROR;
}

void MetadataDriver::cacheMetadataRetrievalResults()
{
	/* Mobile Media Lab. Start */
#ifdef USE_DMC_GALLERY
	// implement 'metadata retrieval function'
	// save each metadata into mMetadataValues[key]
	STHMB sthmb;
	STHMB_MINFO  media_info = {0,};
	STHMB_PARAM param = {0,};
	SCMN_IMGB img = {0, };
	STHMB_STAT stat = {0,};
	int tmp = 0;
	uint32 len = 20;

	int err = 0;
	int rcode = 0;

	////// file name conversion
	char fname[MAX_STRING_LENGTH];
	oscl_UnicodeToUTF8(mDataSourceUrl.get_cstr(), oscl_strlen(mDataSourceUrl.get_cstr()), fname, sizeof(fname));


	sthmb = sthmb_open(fname, &media_info, &err);
	if(sthmb == NULL)
	{
		return;
	}
	/* --tracknumber : mMetadataValues[0] */
	/* --album : mMetadataValues[1] */
	/* --artist : mMetadataValues[2] */
	/* --author : mMetadataValues[3] */
	/* --composer : mMetadataValues[4] */
	/* --date : mMetadataValues[5] */
	/* --genre : mMetadataValues[6] */
	/* --title : mMetadataValues[7] */
	/* --year : mMetadataValues[8]*/
	/* Not yet implemented */

	/* --duration : mMetadataValues[9] */
	tmp = media_info.duration / 10000;
	oscl_snprintf(mMetadataValues[9], len, "%lld", tmp);

	/* --num-tracks : mMetadataValues[10] */
	/* --drm/is-protected : mMetadataValues[11] */
	if( STHMB_DRM_NONE == media_info.drm )
	{
		oscl_snprintf( mMetadataValues[11], len, "false" );
	}
	else
	{
		oscl_snprintf( mMetadataValues[11], len, "true" );
	}
	/* --track-info/codec-name : mMetadataValues[12] */
	/* --rating : mMetadataValues[13] */
	/* --comment : mMetadataValues[14] */
	/* --copyright : mMetadataValues[15] */
	/* Not yet implemented */

	/* --track-info/bit-rate : mMetadataValues[16] */
	if (media_info.vinfo[0].bitrate < 0)
		media_info.vinfo[0].bitrate = 0;
	oscl_snprintf(mMetadataValues[16], len, "%d", media_info.vinfo[0].bitrate);
	/* --track-info/frame-rate : mMetadataValues[17] */
	oscl_snprintf(mMetadataValues[17], len, "%d", media_info.vinfo[0].fps);
	/* --track-info/video/format : mMetadataValues[18] */
	if (media_info.vinfo[0].mt == SCMN_MT_VID_H263 )
	{
		oscl_snprintf(mMetadataValues[18], 12, "%s", "H.263 video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_MPG4)
	{
		oscl_snprintf(mMetadataValues[18], 13, "%s", "MPEG-4 video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_H264)
	{
		oscl_snprintf(mMetadataValues[18], 12, "%s", "H.264 video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_MPG1)
	{
		oscl_snprintf(mMetadataValues[18], 13, "%s", "MPEG-1 video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_MPG2)
	{
		oscl_snprintf(mMetadataValues[18], 13, "%s", "MPEG-2 video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_WMV7)
	{
		oscl_snprintf(mMetadataValues[18], 11, "%s", "WMV7 video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_WMV8)
	{
		oscl_snprintf(mMetadataValues[18], 11, "%s", "WMV8 video");
	}		
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_WMV9)
	{
		oscl_snprintf(mMetadataValues[18], 11, "%s", "WMV9 video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_VC1)
	{
		oscl_snprintf(mMetadataValues[18], 10, "%s", "VC1 video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_VP6)
	{
		oscl_snprintf(mMetadataValues[18], 10, "%s", "VP6 video");
	}		
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_VP7)
	{
		oscl_snprintf(mMetadataValues[18], 10, "%s", "VP7 video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_DIV3)
	{
		oscl_snprintf(mMetadataValues[18], 10, "%s", "Div3video");
	}
	else if (media_info.vinfo[0].mt == SCMN_MT_VID_SORENSON_SPARK)
	{
		oscl_snprintf(mMetadataValues[18], 10, "%s", "Sorenson H.263");
	}	
	else
	{
		oscl_snprintf(mMetadataValues[18], 8, "%s", "unknown");
	}
	/* --track-info/video/height : mMetadataValueList[19] */
	oscl_snprintf(mMetadataValues[19], len, "%d", media_info.vinfo[0].h);
	/* --track-info/video/width : mMetadataValueList[20] */ 
	oscl_snprintf(mMetadataValues[20], len, "%d", media_info.vinfo[0].w);
	sthmb_close(sthmb);	
#else //--USE_DMC_GALLERY
	
    LOGV("cacheMetadataRetrievalResults");
#if _METADATA_DRIVER_INTERNAL_DEBUG_ENABLE_
    for (uint32 i = 0, n = mMetadataValueList.size(); i < n; ++i) {
        LOGV("Value %d:   Key string: %s.", (i+1), mMetadataValueList[i].key);
    }
#endif
    for (uint32 i = 0; i < NUM_METADATA_KEYS; ++i) {
        LOGV("extract metadata key: %s", METADATA_KEYS[i]);
        extractMetadata(METADATA_KEYS[i], mMetadataValues[i], MAX_METADATA_STRING_LENGTH - 1);
    }
    doExtractAlbumArt();
#endif //--USE_DMC_GALLERY
	/* Mobile Media Lab. End */
}

status_t MetadataDriver::extractEmbeddedAlbumArt(const PvmfApicStruct* apic)
{
    LOGV("extractEmbeddedAlbumArt");
    char* buf  = (char*) apic->iGraphicData;
    uint32 size = apic->iGraphicDataLen;
    LOGV("extractEmbeddedAlbumArt: Embedded graphic or album art (%d bytes) is found.", size);
    if (size && buf) {
        delete mMediaAlbumArt;
        mMediaAlbumArt = new MediaAlbumArt();
        if (mMediaAlbumArt == NULL) {
            LOGE("extractEmbeddedAlbumArt: Not enough memory to hold a MediaAlbumArt object");
            return NO_MEMORY;
        }
        mMediaAlbumArt->mSize = size;
        mMediaAlbumArt->mData = new uint8[size];
        if (mMediaAlbumArt->mData == NULL) {
            LOGE("extractEmbeddedAlbumArt: Not enough memory to hold the binary data of a MediaAlbumArt object");
            delete mMediaAlbumArt;
            mMediaAlbumArt = NULL;
            return NO_MEMORY;
        }
        memcpy(mMediaAlbumArt->mData, buf, size);
        return NO_ERROR;
    }
    return BAD_VALUE;
}

status_t MetadataDriver::extractExternalAlbumArt(const char* url)
{
    LOGV("extractExternalAlbumArt: External graphic or album art is found: %s.", url);
    delete mMediaAlbumArt;
    mMediaAlbumArt = new MediaAlbumArt(url);
    return (mMediaAlbumArt && mMediaAlbumArt->mSize > 0)? OK: BAD_VALUE; 
}

// Finds the first album art and extract it.
status_t MetadataDriver::doExtractAlbumArt()
{
    LOGV("doExtractAlbumArt");
    status_t status = UNKNOWN_ERROR;
    for (uint32 i = 0, n = mMetadataValueList.size(); i < n; ++i) {
        if (strcasestr(mMetadataValueList[i].key, ALBUM_ART_KEY)) {
            LOGV("doExtractAlbumArt: album art key: %s", mMetadataValueList[i].key);
            if (PVMI_KVPVALTYPE_KSV == GetValTypeFromKeyString(mMetadataValueList[i].key)) {
                const char* embeddedKey = "graphic;format=APIC;valtype=ksv";
                const char* externalKey = "graphic;valtype=char*";
                if (strstr(mMetadataValueList[i].key, embeddedKey) && mMetadataValueList[i].value.key_specific_value) {
                    // Embedded album art.
                    status = extractEmbeddedAlbumArt(((PvmfApicStruct*)mMetadataValueList[i].value.key_specific_value));
                } else if (strstr(mMetadataValueList[i].key, externalKey)) {
                    // Album art linked with an external url.
                    status = extractExternalAlbumArt(mMetadataValueList[i].value.pChar_value);
                }

                if (status != OK) {
                    continue;
                }
                return status;  // Found the album art.
            }
        }
    }
    return UNKNOWN_ERROR;
}

void MetadataDriver::clearCache()
{
    LOGV("clearCache");
    delete mVideoFrame;
    mVideoFrame = NULL;
    delete mMediaAlbumArt;
    mMediaAlbumArt = NULL;
    for(uint32 i = 0; i < NUM_METADATA_KEYS; ++i) {
        mMetadataValues[i][0] = '\0';
    }
}

/* SYS.LSI Start */
status_t MetadataDriver::setDataSourceFd(
        int fd, int64_t offset, int64_t length) {
    LOGV("setDataSourceFd");

    closeSharedFdIfNecessary();

    if (offset < 0 || length < 0) {
        if (offset < 0) {
            LOGE("negative offset (%lld)", offset);
        }
        if (length < 0) {
            LOGE("negative length (%lld)", length);
        }
        return INVALID_OPERATION;
    }

    mSharedFd = dup(fd);

    char url[80];
    sprintf(url, "sharedfd://%d:%lld:%lld", mSharedFd, offset, length);

    clearCache();
    return doSetDataSource(url);
}
/* SYS.LSI End */
status_t MetadataDriver::setDataSource(const char* srcUrl)
{
    LOGV("setDataSource");
/* SYS.LSI Start */
    closeSharedFdIfNecessary();
/* SYS.LSI End */
    // Don't let somebody trick us in to reading some random block of memory.
/* SYS.LSI Start */
#ifdef ENABLE_SHAREDFD_PLAYBACK
    if (strncmp("sharedfd://", srcUrl, 11) == 0) {
        LOGE("setDataSource: Invalid url (%s).", srcUrl);
        return UNKNOWN_ERROR;
    }
#else
    if (strncmp("mem://", srcUrl, 6) == 0) {
        LOGE("setDataSource: Invalid url (%s).", srcUrl);
        return UNKNOWN_ERROR;
    }
#endif
/* SYS.LSI End */
    if (oscl_strlen(srcUrl) > MAX_STRING_LENGTH) {
        LOGE("setDataSource: Data source url length (%d) is too long.", oscl_strlen(srcUrl));
        return UNKNOWN_ERROR;
    }
    clearCache();
    return doSetDataSource(srcUrl);
}

status_t MetadataDriver::doSetDataSource(const char* dataSrcUrl)
{
    LOGV("doSetDataSource");
    if (mMode & GET_FRAME_ONLY) {
#if BEST_THUMBNAIL_MODE
        mFrameSelector.iSelectionMethod = PVFrameSelector::SPECIFIC_FRAME;
        mFrameSelector.iFrameInfo.iTimeOffsetMilliSec = 0;
#else
        mFrameSelector.iSelectionMethod=PVFrameSelector::SPECIFIC_FRAME;
        mFrameSelector.iFrameInfo.iFrameIndex=0;
#endif
    }
    mIsSetDataSourceSuccessful = false;
    oscl_wchar tmpWCharBuf[MAX_STRING_LENGTH];
    oscl_UTF8ToUnicode(dataSrcUrl, oscl_strlen(dataSrcUrl), tmpWCharBuf, sizeof(tmpWCharBuf));
    mDataSourceUrl.set(tmpWCharBuf, oscl_strlen(tmpWCharBuf));
    mSyncSem = new OsclSemaphore();
    mSyncSem->Create();
    createThreadEtc(MetadataDriver::startDriverThread, this, "PVMetadataRetriever");
    mSyncSem->Wait();
    return mIsSetDataSourceSuccessful? OK: UNKNOWN_ERROR;
}

VideoFrame* MetadataDriver::captureFrame()
{
	/* Mobile Media Lab. Start */
#ifdef USE_DMC_GALLERY
	LOGV("captureFrame");

	STHMB sthmb;
	STHMB_MINFO  media_info = {0,};
	STHMB_PARAM param = {0,};
	SCMN_IMGB img = {0, };
	STHMB_STAT stat = {0,};
	SCMN_BITB bitb = {0,};

	int err = 0;
	int rcode = 0;

	if(mSharedFd > 0)
	{
		bitb.addr = oscl_malloc( mSharedFdLength );
  		bitb.size = mSharedFdLength;

		int read_size = read( mSharedFd, bitb.addr, mSharedFdLength );
		sthmb = sthmb_open_bitb(&bitb, &media_info, &err);
	}
	else
	{
	////// file name conversion
	char fname[MAX_STRING_LENGTH];
	oscl_UnicodeToUTF8(mDataSourceUrl.get_cstr(), oscl_strlen(mDataSourceUrl.get_cstr()), fname, sizeof(fname));
	sthmb = sthmb_open(fname, &media_info, &err);
	}
	if(sthmb == NULL)
	{
		THUMB_LOG( "sthmb_open() fail" );
		return NULL;
	}

	if( media_info.vinfo[0].w > MAX_VIDEO_WIDTH || media_info.vinfo[0].h > MAX_VIDEO_HEIGHT )
	{
		if(mSharedFd>0)
		{
			oscl_free(bitb.addr);
		}	
		THUMB_LOG( "[DMC_GALLERY] bigger than max-video width or height" );
		sthmb_close(sthmb);
		return NULL;
	}

	if( -1 == isPlayableMedia(media_info) )
	{
		sthmb_close(sthmb);
		return NULL;
	}

	if(media_info.vinfo[0].w < 1 || media_info.vinfo[0].h < 1 )
	{
		media_info.vinfo[0].w = DEFAULT_VIDEO_WIDTH;
		media_info.vinfo[0].h = DEFAULT_VIDEO_HEIGHT;		
	}

	if (img_alloc( SCMN_CS_RGB565, media_info.vinfo[0].w, media_info.vinfo[0].h, &img ) )
	{
		if(mSharedFd>0)
		{
			oscl_free(bitb.addr);
		}	
		THUMB_LOG( "img_alloc() failed" );
		sthmb_close(sthmb);
		return NULL;
	} 	

	//param.opt = STHMB_PARAM_OPT_FIT_FULL | STHMB_PARAM_OPT_DITHER;   	  
	param.opt = STHMB_PARAM_OPT_FIT_FULL;   	  	

	param.time = media_info.duration / 10;	/* 10% position */
	rcode = sthmb_extract(sthmb, &param, &img, &stat);
	if(rcode != STHMB_OK)
	{
		param.time = 0;		/* first frame */
		rcode = sthmb_extract(sthmb, &param, &img, &stat);
		if(rcode != STHMB_OK)
		{
			if(mSharedFd>0)
			{
				oscl_free(bitb.addr);
			}
			sthmb_close(sthmb);	  
			img_free(&img);
			THUMB_LOG( "[DMC_GALLERY] sthmb_extract() failed" );
			return NULL;		
		}
	}
 
	VideoFrame *videoFrame = save_img_to_videoframe(&img, 0, 1);

	if(mSharedFd>0)
	{
		oscl_free(bitb.addr);
	}
	sthmb_close(sthmb);
	img_free(&img);
	return videoFrame;
#else
	  LOGV("captureFrame");
	  if (mMode & GET_FRAME_ONLY) {  // copy out
		  if (mVideoFrame != NULL && mVideoFrame->mSize > 0) {
			  return new VideoFrame(*mVideoFrame);
		  } else {
			  LOGE("failed to capture frame");
			  return NULL;
		  }
	  }
	  LOGE("captureFrame: invalid mode (%d) to capture a frame", mMode);
#endif
	/* Mobile Media Lab. End */

    return NULL;
}

void MetadataDriver::doColorConversion()
{
    LOGV("doColorConversion");
    // Do color conversion using PV's color conversion utility

// RainAde : /* 2009.04.21 by icarus : aliginment for color conversion (thunbnail error) */
#if 0
    int width  = mFrameBufferProp.iFrameWidth;
    int height = mFrameBufferProp.iFrameHeight;
    int displayWidth  = mFrameBufferProp.iDisplayWidth;
    int displayHeight = mFrameBufferProp.iDisplayHeight;
#else
	int width  = (mFrameBufferProp.iFrameWidth + 1) & -2;
	int height = (mFrameBufferProp.iFrameHeight + 1) & -2;
	int displayWidth  = (mFrameBufferProp.iDisplayWidth + 1) & -2;
	int displayHeight = (mFrameBufferProp.iDisplayHeight + 1) & -2;
#endif
    SkBitmap *bitmap = new SkBitmap();
    if (!bitmap) {
        LOGE("doColorConversion: cannot instantiate a SkBitmap object.");
        return;
    }
    bitmap->setConfig(SkBitmap::kRGB_565_Config, displayWidth, displayHeight);
    if (!bitmap->allocPixels()) {
        LOGE("allocPixels failed");
        delete bitmap;
        return;
    }
    ColorConvertBase* colorConverter = ColorConvert16::NewL();
    if (!colorConverter ||
        !colorConverter->Init(width, height, width, displayWidth, displayHeight, displayWidth, CCROTATE_NONE) ||
        !colorConverter->SetMode(1) ||
        !colorConverter->Convert(mFrameBuffer, (uint8*)bitmap->getPixels())) {
        LOGE("failed to do color conversion");
        delete colorConverter;
        delete bitmap;
        return;
    }
    delete colorConverter;

    // Store the SkBitmap pixels in a private shared structure with known
    // internal memory layout so that the pixels can be sent across the
    // binder interface
    delete mVideoFrame;
    mVideoFrame = new VideoFrame();
    if (!mVideoFrame) {
        LOGE("failed to allocate memory for a VideoFrame object");
        delete bitmap;
        return;
    }
    mVideoFrame->mWidth = width;
    mVideoFrame->mHeight = height;
    mVideoFrame->mDisplayWidth  = displayWidth;
    mVideoFrame->mDisplayHeight = displayHeight;
    mVideoFrame->mSize = bitmap->getSize();
    LOGV("display width (%d) and height (%d), and size (%d)", displayWidth, displayHeight, mVideoFrame->mSize);
    mVideoFrame->mData = new uint8[mVideoFrame->mSize];
    if (!mVideoFrame->mData) {
        LOGE("doColorConversion: cannot allocate buffer to hold SkBitmap pixels");
        delete bitmap;
        delete mVideoFrame;
        mVideoFrame = NULL;
        return;
    }
    memcpy(mVideoFrame->mData, (uint8*) bitmap->getPixels(), mVideoFrame->mSize);
    delete bitmap;
}

// Instantiate a frame and metadata utility object.
void MetadataDriver::handleCreate()
{
    LOGV("handleCreate");
    int error = 0;
    OSCL_TRY(error, mUtil = PVFrameAndMetadataFactory::CreateFrameAndMetadataUtility((char*)PVMF_MIME_YUV420, this, this, this));
    if (error || mUtil->SetMode(PV_FRAME_METADATA_INTERFACE_MODE_SOURCE_METADATA_AND_THUMBNAIL) != PVMFSuccess) {
        handleCommandFailure();
    } else {
        mState = STATE_ADD_DATA_SOURCE;
        RunIfNotReady();
    }
}

// Create a data source and add it.
void MetadataDriver::handleAddDataSource()
{
    LOGV("handleAddDataSource");
    int error = 0;
    mDataSource = new PVPlayerDataSourceURL;
    if (mDataSource) {
        mDataSource->SetDataSourceURL(mDataSourceUrl);
        mDataSource->SetDataSourceFormatType((char*)PVMF_MIME_FORMAT_UNKNOWN);
        if (mMode & GET_FRAME_ONLY) {	
#if BEST_THUMBNAIL_MODE
            // Set the intent to thumbnails.
            mLocalDataSource = new PVMFLocalDataSource();
            mLocalDataSource->iIntent = BITMASK_PVMF_SOURCE_INTENT_THUMBNAILS;
            mDataSource->SetDataSourceContextData((OsclAny*)mLocalDataSource);
#endif
        }

		/* Mobile Media Lab. Start */
#ifdef USE_DMC_GALLERY
        int fd;
        long long offset;
        long long len;
		char url[MAX_STRING_LENGTH];
		oscl_UnicodeToUTF8(mDataSourceUrl.get_cstr(), oscl_strlen(mDataSourceUrl.get_cstr()), url, sizeof(url));
        if (sscanf(url, "sharedfd://%d:%lld:%lld", &fd, &offset, &len) == 3) {
			mSharedFdLength = len;
        }
        if (mMode & GET_METADATA_ONLY) {
            mState = STATE_GET_METADATA_KEYS;
        } else if (mMode & GET_FRAME_ONLY) {
            mState = STATE_GET_FRAME;
        } else {
            THUMB_LOG("CommandCompleted: Neither retrieve metadata nor capture frame.");
            mState = STATE_REMOVE_DATA_SOURCE;
        }
        mIsSetDataSourceSuccessful = true;	
		RunIfNotReady();
#else
         OSCL_TRY(error, mCmdId = mUtil->AddDataSource(*mDataSource, (OsclAny*)&mContextObject));
         OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());	
#endif
		/* Mobile Media Lab. End */
    }
}

void MetadataDriver::handleRemoveDataSource()
{
    LOGV("handleRemoveDataSource");
    int error = 0;
    OSCL_TRY(error, mCmdId = mUtil->RemoveDataSource(*mDataSource, (OsclAny*)&mContextObject));
    OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());
}

// Clean up, due to either failure or task completion.
void MetadataDriver::handleCleanUp()
{
    LOGV("handleCleanUp");
    if (mUtil)
    {
        PVFrameAndMetadataFactory::DeleteFrameAndMetadataUtility(mUtil);
        mUtil = NULL;
    }
#if BEST_THUMBNAIL_MODE
    delete mLocalDataSource;
    mLocalDataSource = NULL;
#endif
    delete mDataSource;
    mDataSource = NULL;

    OsclExecScheduler *sched=OsclExecScheduler::Current();
    if (sched) {
        sched->StopScheduler();
    }
}

// Retrieve all the available metadata keys.
void MetadataDriver::handleGetMetadataKeys()
{
    int error = 0;
    mMetadataKeyList.clear();

	/* Mobile Media Lab. Start */
#ifdef USE_DMC_GALLERY
	mNumMetadataValues = NUM_METADATA_KEYS;
	
	mMetadataKeyList.reserve(NUM_METADATA_KEYS);

	mMetadataKeyList.push_back("tracknumber");
	mMetadataKeyList.push_back("album");		
	mMetadataKeyList.push_back("artist");		
	mMetadataKeyList.push_back("author");		
	mMetadataKeyList.push_back("composer");		
	mMetadataKeyList.push_back("date");
	mMetadataKeyList.push_back("genre");		
	mMetadataKeyList.push_back("title");		
	mMetadataKeyList.push_back("year");		
	mMetadataKeyList.push_back("duration");		
	mMetadataKeyList.push_back("num-tracks");
	mMetadataKeyList.push_back("drm/is-protected");		
	mMetadataKeyList.push_back("track-info/codec-name");		
	mMetadataKeyList.push_back("rating");		
	mMetadataKeyList.push_back("comment");		
	mMetadataKeyList.push_back("copyright");
	mMetadataKeyList.push_back("track-info/bit-rate");		
	mMetadataKeyList.push_back("track-info/frame-rate");		
	mMetadataKeyList.push_back("track-info/video/format");		
	mMetadataKeyList.push_back("track-info/video/height");		
	mMetadataKeyList.push_back("track-info/video/width");					

    mState = STATE_REMOVE_DATA_SOURCE;
    cacheMetadataRetrievalResults();	
	
	RunIfNotReady();	
#else	
    OSCL_TRY(error, mCmdId = mUtil->GetMetadataKeys(mMetadataKeyList, 0, -1, NULL, (OsclAny*)&mContextObject));
    OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());
#endif
	/* Mobile Media Lab. End */
}

// Retrieve a frame and store the contents into an internal buffer.
void MetadataDriver::handleGetFrame()
{
    LOGV("handleGetFrame");
    int error = 0;
    mFrameBufferSize = MAX_VIDEO_FRAME_SIZE;
    OSCL_TRY(error, mCmdId = mUtil->GetFrame(mFrameSelector, mFrameBuffer, mFrameBufferSize, mFrameBufferProp, (OsclAny*)&mContextObject));
    OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());
}

// Retrieve all the available metadata values associated with the given keys.
void MetadataDriver::handleGetMetadataValues()
{
    LOGV("handleGetMetadataValues()");
    int error = 0;
    mNumMetadataValues = 0;
    mMetadataValueList.clear();
    trimKeys();  // Switch to use actual supported key list.

	/* Mobile Media Lab. Start */
#ifdef USE_DMC_GALLERY
	RunIfNotReady();
#else
    OSCL_TRY(error, mCmdId = mUtil->GetMetadataValues(mActualMetadataKeyList, 0, -1, mNumMetadataValues, mMetadataValueList, (OsclAny*)&mContextObject));
    OSCL_FIRST_CATCH_ANY(error, handleCommandFailure());
#endif
	/* Mobile Media Lab. End */
}

void MetadataDriver::Run()
{
    LOGV("Run (%d)", mState);
    switch(mState) {
        case STATE_CREATE:
            handleCreate();
            break;
        case STATE_ADD_DATA_SOURCE:
            handleAddDataSource();
            break;
        case STATE_GET_METADATA_KEYS:
            handleGetMetadataKeys();
            break;
        case STATE_GET_METADATA_VALUES:
            handleGetMetadataValues();
            break;
        case STATE_GET_FRAME:
            handleGetFrame();
            break;
        case STATE_REMOVE_DATA_SOURCE:
            handleRemoveDataSource();
            break;
        default:
            handleCleanUp();
            break;
    }
}

bool MetadataDriver::isCommandSuccessful(const PVCmdResponse& aResponse) const
{
    LOGV("isCommandSuccessful");
    bool success = ((aResponse.GetCmdId() == mCmdId) &&
            (aResponse.GetCmdStatus() == PVMFSuccess) &&
            (aResponse.GetContext() == (OsclAny*)&mContextObject));
    if (!success) {
        LOGV("isCommandSuccessful: Command id(%d and expected %d) and status (%d and expected %d), data corruption (%s) at state (%d).",
             aResponse.GetCmdId(), mCmdId, aResponse.GetCmdStatus(), PVMFSuccess, (aResponse.GetContext() == (OsclAny*)&mContextObject)? "false": "true", mState);
    }
    return success;
}

void MetadataDriver::handleCommandFailure()
{
    LOGV("handleCommandFailure");
    if (mState == STATE_REMOVE_DATA_SOURCE) {
        mState = STATE_CLEANUP_AND_COMPLETE;
    }
    else{
        mState = STATE_REMOVE_DATA_SOURCE;
    }
    RunIfNotReady();
}

// Callback handler for a request completion by frameandmetadatautility.
void MetadataDriver::CommandCompleted(const PVCmdResponse& aResponse)
{
    LOGV("CommandCompleted (%d)", mState);
    if (!isCommandSuccessful(aResponse)) {
        handleCommandFailure();
        return;
    }

    switch(mState) {
        case STATE_ADD_DATA_SOURCE:
            if (mMode & GET_METADATA_ONLY) {
                mState = STATE_GET_METADATA_KEYS;
            } else if (mMode & GET_FRAME_ONLY) {
                mState = STATE_GET_FRAME;
            } else {
                LOGV("CommandCompleted: Neither retrieve metadata nor capture frame.");
                mState = STATE_REMOVE_DATA_SOURCE;
            }
            mIsSetDataSourceSuccessful = true;
            break;
        case STATE_GET_METADATA_KEYS:
            mState = STATE_GET_METADATA_VALUES;
            break;
        case STATE_GET_METADATA_VALUES:
            if (mMode & GET_FRAME_ONLY) {
                mState = STATE_GET_FRAME;
            } else {
                mState = STATE_REMOVE_DATA_SOURCE;
            }
            cacheMetadataRetrievalResults();
            break;
        case STATE_GET_FRAME:
            doColorConversion();
            mState = STATE_REMOVE_DATA_SOURCE;
            break;
        case STATE_REMOVE_DATA_SOURCE:
            mState = STATE_CLEANUP_AND_COMPLETE;
            break;
        default:
            mState = STATE_CLEANUP_AND_COMPLETE;
            break;
    }
    RunIfNotReady();
}

void MetadataDriver::HandleErrorEvent(const PVAsyncErrorEvent& aEvent)
{
    // Error occurs, clean up and terminate.
    LOGE("HandleErrorEvent: Event [type(%d), response type(%d)] received.", aEvent.GetEventType(), aEvent.GetResponseType());
    handleCommandFailure();
}


void MetadataDriver::HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent)
{
    LOGV("HandleInformationalEvent: Event [type(%d), response type(%d)] received.", aEvent.GetEventType(), aEvent.GetResponseType());
}

/* SYS.LSI Start */
void MetadataDriver::closeSharedFdIfNecessary() {
    if (mSharedFd >= 0) {
        close(mSharedFd);
        mSharedFd = -1;
    }
}
/* SYS.LSI End */
//------------------------------------------------------------------------------
#include <media/PVMetadataRetriever.h>

namespace android {

//#define LOG_NDEBUG 0
//#define LOG_TAG "PVMetadataRetriever"

// A concrete subclass of MediaMetadataRetrieverInterface implementation
// Use the MetadataDriver object as a delegate and forward related calls
// to the MetadataDriver object.
PVMetadataRetriever::PVMetadataRetriever()
{
    LOGV("constructor");
    mMetadataDriver = new MetadataDriver();
}

PVMetadataRetriever::~PVMetadataRetriever()
{
    LOGV("destructor");
    Mutex::Autolock lock(mLock);
    delete mMetadataDriver;
}

status_t PVMetadataRetriever::setDataSource(const char *url)
{
    LOGV("setDataSource (%s)", url);
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return INVALID_OPERATION;
    }
    if (url == 0) {
        LOGE("Null pointer is passed as argument");
        return INVALID_OPERATION;
    }
    return mMetadataDriver->setDataSource(url);
}

status_t PVMetadataRetriever::setDataSource(int fd, int64_t offset, int64_t length)
{
    LOGV("setDataSource fd(%d), offset(%lld), length(%lld)", fd, offset, length);
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return INVALID_OPERATION;
    }
    if (offset < 0 || length < 0) {
        if (offset < 0) {
            LOGE("negative offset (%lld)", offset);
        }
        if (length < 0) {
            LOGE("negative length (%lld)", length);
        }
        return INVALID_OPERATION;
    }
/* SYS.LSI Start */
#ifdef ENABLE_SHAREDFD_PLAYBACK
    return mMetadataDriver->setDataSourceFd(fd, offset, length);
#else
    return NO_ERROR;
#endif
/* SYS.LSI End */
}

status_t PVMetadataRetriever::setMode(int mode)
{
    LOGV("setMode (%d)", mode);
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return INVALID_OPERATION;
    }
    if (mode < 0x00 || mode > 0x03) {
        LOGE("set to invalid mode (%d)", mode);
        return INVALID_OPERATION;
    }
    return mMetadataDriver->setMode(mode);
}

status_t PVMetadataRetriever::getMode(int* mode) const
{
    LOGV("getMode");
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return INVALID_OPERATION;
    }
    if (mode == 0) {
        LOGE("Null pointer is passed as argument");
        return INVALID_OPERATION;
    }
    return mMetadataDriver->getMode(mode);
}

VideoFrame *PVMetadataRetriever::captureFrame()
{
    LOGV("captureFrame");
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return NULL;
    }
    return mMetadataDriver->captureFrame();
}

MediaAlbumArt *PVMetadataRetriever::extractAlbumArt()
{
    LOGV("extractAlbumArt");
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return NULL;
    }
    return mMetadataDriver->extractAlbumArt();
}

const char* PVMetadataRetriever::extractMetadata(int keyCode)
{
    LOGV("extractMetadata");
    Mutex::Autolock lock(mLock);
    if (mMetadataDriver == 0) {
        LOGE("No MetadataDriver available");
        return NULL;
    }
    return mMetadataDriver->extractMetadata(keyCode);
}

};  // android
