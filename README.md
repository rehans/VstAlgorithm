# VstAlgorithm
VstAlgorithm is a header only library which supports developers who are building VST3 plug-ins. Inspired by the STL the library contains a couple of helper functions which allow e.g. mixing two audio buses or iterating over parameter changes.

##Example
The VST3 interface declares an IParamValueQueue interface. With this interface the host tells the plug-in all changes of a certain parameter during the next audio slice.

Iterating over all changes of a parameter can be done as follows:
```
vstalgorithm::foreach_ParamValue(paramValueQueue, [&](Vst::ParamID paramId, int32 sampleOffset, Vst::ParamValue value) {
	// Do sth here
});
```

## License
Released under the [MIT License](LICENSE)

## Copyright notice
VST is a trademark of Steinberg Media Technologies GmbH
