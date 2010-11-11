/* ------------------------------------------------------------------
 * Copyright (C) 2009 Samsung Electronics
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
/**
 * @file smp4fm_oc_factory.h
 * @brief declaration for SMp4fmNode
 */

#ifndef __SMP4FM_OC_FACTORY_H_237394792734897294__
#define __SMP4FM_OC_FACTORY_H_237394792734897294__

#include "oscl_base.h"
#include "oscl_scheduler_ao.h"

/* forward declaration */
class PVMFNodeInterface;

#define SMp4fmOcNodeUuid PVUuid(0x057ea45d,0xf969,0x48b7,0xa7,0xa7,0x47,0x7e,0x24,0x12,0x9d,0x2b)

class SMp4fmOcNodeFactory
{
public:
	OSCL_IMPORT_REF static PVMFNodeInterface * CreateSMp4fmOcNode(int32 priority = OsclActiveObject::EPriorityNominal);

	OSCL_IMPORT_REF static bool DeleteSMp4fmOcNode(PVMFNodeInterface * mux);
};
#endif /* __SMP4FM_OC_FACTORY_H_237394792734897294__ */
