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
#ifndef PVMF_AC3FFPARSER_TUNABLES_H_INCLUDED
#define PVMF_AC3FFPARSER_TUNABLES_H_INCLUDED

//Default NODE COMMAND vector reserve size
#define PVMF_AC3FFPARSER_NODE_COMMAND_VECTOR_RESERVE 10

//Starting value for command IDs
#define PVMF_AC3FFPARSER_NODE_COMMAND_ID_START 6000

#define PVAC3FF_MEDIADATA_POOLNUM 6
#define PVAC3FF_MEDIADATA_CHUNKSIZE 128

// Temporary until actual max track data size if used.
#define MAXTRACKDATASIZE	7680
#define MAX_PORTS			2
#define NUM_AC3_FRAMES	1	

#endif /*PVMF_AC3FFPARSER_TUNABLES_H_INCLUDED*/
