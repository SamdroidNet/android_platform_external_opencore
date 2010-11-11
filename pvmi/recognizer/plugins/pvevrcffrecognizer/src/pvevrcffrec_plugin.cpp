/* ------------------------------------------------------------------
 * Copyright (C) 2008 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
#include "pvevrcffrec_plugin.h"
#include "pvfile.h"
#include "oscl_file_io.h"

PVMFStatus PVEVRCFFRecognizerPlugin::SupportedFormats(PVMFRecognizerMIMEStringList& aSupportedFormatsList)
{
    // Return EVRC as supported type
    OSCL_HeapString<OsclMemAllocator> supportedformat = PVMF_MIME_EVRCFF;
    aSupportedFormatsList.push_back(supportedformat);
    return PVMFSuccess;
}


PVMFStatus PVEVRCFFRecognizerPlugin::Recognize(PVMFDataStreamFactory& aSourceDataStreamFactory, PVMFRecognizerMIMEStringList* aFormatHint,
        Oscl_Vector<PVMFRecognizerResult, OsclMemAllocator>& aRecognizerResult)
{

	PVMFCPMPluginAccessInterfaceFactory *pTemp = OSCL_STATIC_CAST(PVMFCPMPluginAccessInterfaceFactory*, &aSourceDataStreamFactory);
	
    OSCL_UNUSED_ARG(aFormatHint);
    OSCL_wHeapString<OsclMemAllocator> tmpfilename;
    char* filename = NULL;
    wchar_t* Widechar = NULL;
    bool variable = 0;
    Oscl_FileServer fileServ;
    PVFile pvfile;
    pvfile.SetCPM(&aSourceDataStreamFactory);

    variable = pvfile.IsOpen();

    if (!(pvfile.Open(tmpfilename.get_cstr(), Oscl_File::MODE_READ | Oscl_File::MODE_BINARY, fileServ)))
    {
		Widechar = const_cast<wchar_t*>(tmpfilename.get_cstr());
		filename = (char *)malloc( 20 * sizeof(char));
		memset(filename,0,20);
		wcstombs(filename,Widechar,20);
		
        char* readData = NULL;

        readData = (char*)(oscl_malloc(sizeof(char) * EVRCFF_MIN_DATA_SIZE_FOR_RECOGNITION));
        if (readData != NULL)
        {
            int bytesRead = 0;
            bytesRead = pvfile.Read(readData, sizeof(char), EVRCFF_MIN_DATA_SIZE_FOR_RECOGNITION);
            if (bytesRead != EVRCFF_MIN_DATA_SIZE_FOR_RECOGNITION)
            {
                pvfile.Close();
                oscl_free(readData);
                return PVMFFailure;
            }
            if (readData[0] == '#' && readData[1] == '!' && readData[2] == 'E' && readData[3] == 'V' && readData[4] == 'R' && readData[5] == 'C')
            {
				PVMFRecognizerResult result;
                result.iRecognizedFormat = PVMF_MIME_EVRCFF;
                result.iRecognitionConfidence = PVMFRecognizerConfidenceCertain;
                aRecognizerResult.push_back(result);
            }
        }
        pvfile.Close();
        oscl_free(readData);
        return PVMFFailure;
    }
    else
    {
        return PVMFFailure;
    }
    return PVMFSuccess;
}


PVMFStatus PVEVRCFFRecognizerPlugin::GetRequiredMinBytesForRecognition(uint32& aBytes)
{
    aBytes = EVRCFF_MIN_DATA_SIZE_FOR_RECOGNITION;
    return PVMFSuccess;
}


