# Pd_bitcrusherd
A bit-depth bitcrusher external for Pd (Pure Data)

## Background information
General background information on bitcrush audio effects is available on [Wikipedia](https://en.wikipedia.org/wiki/Bitcrusher).
General background information and downloads for Pure Data is available at [puredata.info](https://puredata.info).

## Build information
An XCode project is provided to build the library. Alternatively a pre-built version is available in the "prebuilt" directory of the repo.
To install the prebuilt library, perform the following steps. An Apple Silicon Mac is assumed:
1) Install the vanilla version of [Pure Data](https://puredata.info/downloads/pure-data)
2) Create a ```~/Documents/Pd/externals/bitcrusherd~``` directory
3) Copy ```<repo>/prebuilt/bitcrusherd~.pd_darwin``` into the ```~/Documents/Pd/externals/bitcrusherd~``` directory.
4) In Pd Preferences, select Path. If not already added, add ```~/Documents/Pd/externals```.
5) Open <repo>/test patches/bitcrusherd~_test.pd in Pd. Activate DSP in Pd, and use the provided Pd number control to adjust the bitcrush depth value on the provided 440 Hz oscillator. The graph included in the patch will illustrate the effect on the 440 Hz waveform.

## Algorithm information
Refer to the ```bitcrusherd_tilde_perform``` function in ```<repo>/bitcrusherd~.c``` for implementation and explanation of the algorithm.
