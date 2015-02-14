// Copyright René Hansen 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#define USE_XMM_INTRIN 1

#include "pluginterfaces/vst/ivstaudioprocessor.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstunits.h"

#if USE_XMM_INTRIN
#include <xmmintrin.h>
#endif

namespace vstalgorithm
{

//------------------------------------------------------------------------
template <typename T>
inline void foreach (Steinberg::Vst::AudioBusBuffers* audioBusBuffers,
					 Steinberg::int32 busCount,
					 const T& func);

template <typename T>
inline void foreach (Steinberg::Vst::AudioBusBuffers& audioBuffer, const T& func);

template <typename T>
inline void foreach (Steinberg::Vst::IEventList* eventList, const T& func);

template <typename T>
inline void foreach (Steinberg::Vst::IParamValueQueue* paramQueue, const T& func);

template <typename T>
inline void foreach (Steinberg::Vst::IParameterChanges* changes, const T& func);

template <typename T>
inline void foreach (Steinberg::Vst::AudioBusBuffers& buffer0, 
					 Steinberg::Vst::AudioBusBuffers& buffer1,
					 const T& func);

template <typename T>
inline void foreach (Steinberg::Vst::IUnitInfo* unitInfo, const T& func);

template <typename T>
inline void foreach_BusInfo (Steinberg::Vst::IComponent* component, 
							 Steinberg::Vst::MediaType type, 
							 Steinberg::Vst::BusDirection dir, 
							 const T& func);

template <typename T>
inline void foreach_ProgramListInfo (Steinberg::Vst::IUnitInfo* unitInfo, const T& func);


inline void copy (Steinberg::Vst::AudioBusBuffers* dst, 
				  Steinberg::Vst::AudioBusBuffers* src, 
				  Steinberg::int32 sliceSize, 
				  Steinberg::int32 beginIndex = 0);

inline void clear (Steinberg::Vst::AudioBusBuffers* audioBusBuffers,
				   Steinberg::int32 sampleCount,
				   Steinberg::int32 busCount = 1);

inline void mix (Steinberg::Vst::AudioBusBuffers& dst, 
				 Steinberg::Vst::AudioBusBuffers& src, 
				 Steinberg::int32 sampleCount);

//------------------------------------------------------------------------
#include "vstalgorithm.inl"
//------------------------------------------------------------------------

}