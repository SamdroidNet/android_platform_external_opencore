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
#ifndef PVMF_G711FFPARSER_DEFS_H_INCLUDED
#define PVMF_G711FFPARSER_DEFS_H_INCLUDED

///////////////////////////////////////////////
// Port tags
///////////////////////////////////////////////

/** Enumerated list of port tags supported by the node,
** for the port requests.
*/
typedef enum
{
    PVMF_G711FFPARSER_NODE_PORT_TYPE_SOURCE
} PVMFG711FFParserOutPortType;

// Capability mime strings
#define PVMF_G711FFPARSER_PORT_OUTPUT_FORMATS "x-pvmf/port/formattype"
#define PVMF_G711FFPARSER_PORT_OUTPUT_FORMATS_VALTYPE "x-pvmf/port/formattype;valtype=int32"

#define G711_MIN_DATA_SIZE_FOR_RECOGNITION  512
#define G711_DATA_OBJECT_PARSING_OVERHEAD   512
#define G711_HEADER_SIZE 32

#define PVMF_G711_PARSER_NODE_MAX_AUDIO_DATA_MEM_POOL_SIZE   1024*1024
#define PVMF_G711_PARSER_NODE_DATA_MEM_POOL_GROWTH_LIMIT     1
#define PVMF_G711_PARSER_NODE_MAX_NUM_OUTSTANDING_MEDIA_MSGS 4
#define PVMF_G711_PARSER_NODE_MEDIA_MSG_SIZE                 128
#define PVMF_G711_PARSER_NODE_TS_DELTA_DURING_REPOS_IN_MS    32

#endif

