// Copyright René Hansen 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#define USE_XMM_INTRIN 1

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstunits.h"
#include <functional>

#if USE_XMM_INTRIN
#include <xmmintrin.h>
#endif

namespace vstalgorithm
{

//------------------------------------------------------------------------
inline void foreach (Steinberg::Vst::AudioBusBuffers* audioBusBuffers,
					 Steinberg::int32 busCount,
					 std::function<void (Steinberg::Vst::AudioBusBuffers&)> func);

inline void foreach (Steinberg::Vst::AudioBusBuffers& audioBuffer,
					 std::function<void (Steinberg::Vst::Sample32*)>func);

inline void foreach (Steinberg::Vst::IEventList* eventList, 
					 std::function<void (Steinberg::Vst::Event& event)>func);

inline void foreach (Steinberg::Vst::IParamValueQueue* paramQueue, 
					 std::function<void (Steinberg::Vst::ParamID, Steinberg::int32, Steinberg::Vst::ParamValue)> func);

inline void foreach (Steinberg::Vst::IParameterChanges* changes, 
					 std::function<void (Steinberg::Vst::IParamValueQueue*)> func);

inline void foreach (Steinberg::Vst::AudioBusBuffers& buffer0, 
					 Steinberg::Vst::AudioBusBuffers& buffer1,
					 std::function<void (Steinberg::Vst::Sample32* buffer0, Steinberg::Vst::Sample32* buffer1)>func);

inline void foreach (Steinberg::Vst::IUnitInfo* unitInfo, 
					 std::function<void (const Steinberg::Vst::UnitInfo&)>func);

inline void foreach (Steinberg::Vst::IUnitInfo* unitInfo,
					 std::function<void(const Steinberg::Vst::ProgramListInfo&)>func);

inline void foreach(Steinberg::Vst::IUnitInfo* unitInfo, const Steinberg::Vst::ProgramListInfo& progListInfo,
					std::function<void(Steinberg::Vst::String128 name)>func);

inline void copy (Steinberg::Vst::AudioBusBuffers* dst, 
				  Steinberg::Vst::AudioBusBuffers* src, 
				  Steinberg::int32 sliceSize, 
				  Steinberg::int32 beginIndex = 0);

inline void clear (Steinberg::Vst::AudioBusBuffers* audioBusBuffers,
				   Steinberg::int32 sampleCount,
				   Steinberg::int32 busCount = 1);

inline void mix (Steinberg::Vst::AudioBusBuffers& toAudioBuffers, 
				 Steinberg::Vst::AudioBusBuffers& fromAudioBuffers, 
				 Steinberg::int32 sampleCount);

//------------------------------------------------------------------------
#include "vstalgorithm.inl"
//------------------------------------------------------------------------

}