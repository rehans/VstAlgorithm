// Copyright René Hansen 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

template <typename T>
void foreach (Steinberg::Vst::AudioBusBuffers* audioBusBuffers,
			  Steinberg::int32 busCount,
			  const T& func)
{
	if (!audioBusBuffers)
		return;

	for (Steinberg::int32 busIndex = 0; busIndex < busCount; ++busIndex)
	{
		func (audioBusBuffers[busIndex]);
	}
}

//------------------------------------------------------------------------
template <typename T>
void foreach (Steinberg::Vst::AudioBusBuffers& audioBuffer, const T& func)
{
	for (Steinberg::int32 channelIndex = 0; channelIndex < audioBuffer.numChannels; ++channelIndex)
	{
		if (!audioBuffer.channelBuffers32 [channelIndex])
			return;

		func (audioBuffer.channelBuffers32 [channelIndex]);
	}
}

//------------------------------------------------------------------------
template <typename T>
void foreach (Steinberg::Vst::IEventList* eventList, const T& func)
{
	if (!eventList)
		return;

	auto eventCount = eventList->getEventCount ();
	for (Steinberg::int32 eventIndex = 0; eventIndex < eventCount; ++eventIndex)
	{
		Steinberg::Vst::Event event = { 0 };
		if (eventList->getEvent (eventIndex, event) != Steinberg::kResultOk)
			continue;

		func (event);
	}
}

//------------------------------------------------------------------------
template <typename T>
void foreach (Steinberg::Vst::IUnitInfo* unitInfo, const T& func)
{
	if (!unitInfo)
		return;

	for (Steinberg::int32 i = 0; i < unitInfo->getUnitCount(); ++i)
	{
		Steinberg::Vst::UnitInfo info = { 0 };
		if (unitInfo->getUnitInfo(i, info) != Steinberg::kResultOk)
			continue;

		func (info);
	}
}

//------------------------------------------------------------------------
template <typename T>
inline void foreach_BusInfo (Steinberg::Vst::IComponent* component,
							 Steinberg::Vst::MediaType type,
							 Steinberg::Vst::BusDirection dir, 
							 const T& func)
{
	if (!component)
		return;

	Steinberg::int32 busCount = component->getBusCount (type, dir);
	for (Steinberg::int32 busIdx = 0; busIdx < busCount; ++busIdx)
	{
		Steinberg::Vst::BusInfo info;
		if (component->getBusInfo (type, dir, busIdx, info) != Steinberg::kResultOk)
			continue;

		func (info);
	}
}

//------------------------------------------------------------------------
template <typename T>
void foreach_ProgramListInfo (Steinberg::Vst::IUnitInfo* unitInfo, const T& func)
{
	if (!unitInfo)
		return;

	for (Steinberg::int32 i = 0; i < unitInfo->getUnitCount(); ++i)
	{
		Steinberg::Vst::ProgramListInfo info = { 0 };
		if (unitInfo->getProgramListInfo(i, info) != Steinberg::kResultOk)
			continue;

		func(info);
	}
}

//------------------------------------------------------------------------
template <typename T>
void foreach (Steinberg::Vst::IParamValueQueue* paramQueue, const T& func)
{
	if (!paramQueue)
		return;

	auto paramId = paramQueue->getParameterId ();
	auto numPoints = paramQueue->getPointCount ();
	for (Steinberg::int32 pointIndex = 0; pointIndex < numPoints; ++pointIndex)
	{
		Steinberg::int32 sampleOffset = 0;
		Steinberg::Vst::ParamValue value = 0;
		if (paramQueue->getPoint (pointIndex, sampleOffset, value) != Steinberg::kResultOk)
			continue;

		func (paramId, sampleOffset, value);
	}
}

//------------------------------------------------------------------------
template <typename T>
void foreach (Steinberg::Vst::IParameterChanges* changes, const T& func)
{
	if (!changes)
		return;

	auto paramCount = changes->getParameterCount ();
	for (Steinberg::int32 paramIndex = 0; paramIndex < paramCount; ++paramIndex)
	{
		auto paramValueQueue = changes->getParameterData (paramIndex);
		if (!paramValueQueue)
			continue;

		func (paramValueQueue);
	}
}

//------------------------------------------------------------------------
template <typename T>
void foreach (Steinberg::Vst::AudioBusBuffers& buffer0, 
			  Steinberg::Vst::AudioBusBuffers& buffer1,
			  const T& func)
{
	Steinberg::int32 numChannels = std::min (buffer0.numChannels, buffer1.numChannels);

	for (Steinberg::int32 channelIndex = 0; channelIndex < numChannels; ++channelIndex)
	{
		func (buffer0.channelBuffers32[channelIndex], buffer1.channelBuffers32[channelIndex], channelIndex);
	}
}

//------------------------------------------------------------------------
void copy (Steinberg::Vst::AudioBusBuffers* dst, 
		   Steinberg::Vst::AudioBusBuffers* src, 
		   Steinberg::int32 sliceSize, 
		   Steinberg::int32 beginIndex)
{
	if (!dst || !src)
		return;

	Steinberg::int32 numChannels = std::min (src->numChannels, dst->numChannels);
	size_t numBytes = sliceSize * sizeof (Steinberg::Vst::Sample32);
	for (Steinberg::int32 chIdx = 0; chIdx < numChannels; ++chIdx)
	{
		memcpy (&dst->channelBuffers32[chIdx][beginIndex], src->channelBuffers32[chIdx], numBytes);
	}
}

//------------------------------------------------------------------------
void clear (Steinberg::Vst::AudioBusBuffers* audioBusBuffers,
			Steinberg::int32 sampleCount,
			Steinberg::int32 busCount)
{
	if (!audioBusBuffers)
		return;

	const Steinberg::int32 numBytes = sampleCount * sizeof (Steinberg::Vst::Sample32);
	foreach (audioBusBuffers, busCount, [&](Steinberg::Vst::AudioBusBuffers& audioBuffer)
	{
		foreach (audioBuffer, [&](Steinberg::Vst::Sample32* channelBuffer)
		{
			if (!channelBuffer)
				return;

			memset (channelBuffer, 0, numBytes);
		});
	});
}

//------------------------------------------------------------------------
void mix (Steinberg::Vst::AudioBusBuffers& dst,
		  Steinberg::Vst::AudioBusBuffers& src,
		  Steinberg::int32 sampleCount)
{
	foreach (dst, src, [&](Steinberg::Vst::Sample32* buffer0, Steinberg::Vst::Sample32* buffer1, Steinberg::int32 /*channelIndex*/)
	{
#if USE_XMM_INTRIN
		static const Steinberg::int32 kVectorSize = 4;
		if ((sampleCount & (kVectorSize-1)) == 0) // fast modulo: sampleCount % kVectorSize == 0
		{
			for (Steinberg::int32 sampleIdx = 0; sampleIdx < sampleCount; sampleIdx += kVectorSize)
			{
				_mm_store_ps (buffer0, _mm_add_ps  (_mm_load_ps (buffer0), _mm_load_ps (buffer1)));
				_mm_store_ps (buffer1, _mm_set_ps1 (0.f));

				buffer0 += kVectorSize;
				buffer1 += kVectorSize;
			}
		}
		else
#endif
		{
			for (Steinberg::int32 sampleIndex = 0; sampleIndex < sampleCount; ++sampleIndex)
			{
				buffer0[sampleIndex] += buffer1[sampleIndex];
				buffer1[sampleIndex] = 0;
			}
		}
	});
}