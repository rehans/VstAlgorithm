// Copyright René Hansen 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

void foreach (Steinberg::Vst::AudioBusBuffers* audioBusBuffers,
			  Steinberg::int32 busCount,
			  std::function<void (Steinberg::Vst::AudioBusBuffers&)> func)
{
	if (!audioBusBuffers)
		return;

	for (Steinberg::int32 busIndex = 0; busIndex < busCount; ++busIndex)
	{
		func (audioBusBuffers[busIndex]);
	}
}

//------------------------------------------------------------------------
void foreach (Steinberg::Vst::AudioBusBuffers& audioBuffer,
			  std::function<void (Steinberg::Vst::Sample32*)>func)
{
	for (Steinberg::int32 channelIndex = 0; channelIndex < audioBuffer.numChannels; ++channelIndex)
	{
		if (!audioBuffer.channelBuffers32 [channelIndex])
			return;

		func (audioBuffer.channelBuffers32 [channelIndex]);
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
void foreach (Steinberg::Vst::IEventList* eventList,
			  std::function<void (Steinberg::Vst::Event& event)>func)
{
	if (!eventList)
		return;

	Steinberg::int32 eventCount = eventList->getEventCount ();
	for (Steinberg::int32 eventIndex = 0; eventIndex < eventCount; ++eventIndex)
	{
		Steinberg::Vst::Event event = { 0 };
		if (eventList->getEvent (eventIndex, event) != Steinberg::kResultOk)
			continue;

		func (event);
	}
}

//------------------------------------------------------------------------
void foreach (Steinberg::Vst::IUnitInfo* unitInfo,
			  std::function<void(const Steinberg::Vst::UnitInfo&)>func)
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
void foreach (Steinberg::Vst::IUnitInfo* unitInfo,
			  std::function<void(const Steinberg::Vst::ProgramListInfo&)>func)
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
void foreach (Steinberg::Vst::IUnitInfo* unitInfo, const Steinberg::Vst::ProgramListInfo& progListInfo,
			  std::function<void(Steinberg::Vst::String128 name)>func)
{
	if (!unitInfo)
		return;

	for (Steinberg::int32 progIndex = 0; progIndex < progListInfo.programCount; ++progIndex)
	{
		Steinberg::Vst::String128 progName;
		if (unitInfo->getProgramName (progListInfo.id, progIndex, progName) != Steinberg::kResultOk)
			continue;

		func (progName);
	}
}
//------------------------------------------------------------------------
void copy (Steinberg::Vst::AudioBusBuffers* to, 
		   Steinberg::Vst::AudioBusBuffers* from, 
		   Steinberg::int32 sliceSize, 
		   Steinberg::int32 beginIndex)
{
	if (!to || !from)
		return;

	size_t numBytes = sliceSize * sizeof (Steinberg::Vst::Sample32);
	for (int32_t chIdx = 0; chIdx < to->numChannels && chIdx < to->numChannels; ++chIdx)
	{
		memcpy (&to->channelBuffers32[chIdx][beginIndex], from->channelBuffers32[chIdx], numBytes);
	}
}

//------------------------------------------------------------------------
void foreach (Steinberg::Vst::IParamValueQueue* paramQueue,
			  std::function<void (Steinberg::Vst::ParamID, Steinberg::int32, Steinberg::Vst::ParamValue)> func)
{
	if (!paramQueue)
		return;

	Steinberg::Vst::ParamID paramId = paramQueue->getParameterId ();
	Steinberg::int32 numPoints = paramQueue->getPointCount ();
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
void foreach (Steinberg::Vst::IParameterChanges* changes,
			  std::function<void (Steinberg::Vst::IParamValueQueue*)> func)
{
	if (!changes)
		return;

	Steinberg::int32 paramCount = changes->getParameterCount ();
	for (Steinberg::int32 paramIndex = 0; paramIndex < paramCount; ++paramIndex)
	{
		auto paramValueQueue = changes->getParameterData (paramIndex);
		if (!paramValueQueue)
			continue;

		func (paramValueQueue);
	}
}

//------------------------------------------------------------------------
void foreach (Steinberg::Vst::AudioBusBuffers& buffer0, 
			  Steinberg::Vst::AudioBusBuffers& buffer1,
			  std::function<void (Steinberg::Vst::Sample32* buffer0, Steinberg::Vst::Sample32* buffer1)>func)
{
	if (buffer0.numChannels != buffer1.numChannels)
		return;

	for (Steinberg::int32 channelIndex = 0; channelIndex < buffer0.numChannels; ++channelIndex)
	{
		func (buffer0.channelBuffers32[channelIndex], buffer1.channelBuffers32[channelIndex]);
	}
}

//------------------------------------------------------------------------
void mix (Steinberg::Vst::AudioBusBuffers& toAudioBuffers,
		  Steinberg::Vst::AudioBusBuffers& fromAudioBuffers,
		  Steinberg::int32 sampleCount)
{
	foreach (toAudioBuffers, fromAudioBuffers, [&](Steinberg::Vst::Sample32* buffer0, Steinberg::Vst::Sample32* buffer1)
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