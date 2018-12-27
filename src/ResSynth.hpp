

//
//  SineSynth.hpp
//  PartialSynth
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#ifndef ResSynth_hpp
#define ResSynth_hpp

#include <stdio.h>

#include "ResPartial.hpp"
#include <vector>

static const int maxNumberOfPartials = 4096;

typedef struct ResPartialData {
    float* freq = 0;
    float* decay = 0;
    float* gain = 0;
    size_t size = 0;

    
    ResPartialData() = default;

    explicit ResPartialData(size_t s)
    {
        freq = new float[s];
        decay = new float[s];
        gain = new float[s];
        size = s;
    }
    ~ResPartialData()
    {
        if (freq)
            delete[] freq;
        if (decay)
            delete[] decay;
        if (gain)
            delete[] gain;
    }

    ResPartialData(ResPartialData& d)
    {
        if (size != d.size) {
            freq = new float[d.size];
            decay = new float[d.size];
            gain = new float[d.size];
            size = d.size;
        }

        memcpy(freq, d.freq, size);
        memcpy(decay, d.decay, size);
        memcpy(gain, d.gain, size);
    }

    void clear()
    {
        if (freq) {
            delete[] freq;
            freq = 0;
        }
        if (decay) {
            delete[] decay;
            decay = 0;
        }
        if (gain) {
            delete[] gain;
            gain = 0;
        }

        size = 0;
    }

} ResPartialData;

class ResSynth {
    
    
    float _scratchBuffer[8192]; //todo check ranges

    std::vector<ResPartial*> partials;
    std::vector<int> _freePartialIndices;

    void updateFreePartialIndices()
    {
        _freePartialIndices.clear();
        for (int i = 0; i < maxNumberOfPartials; i++) {
            if (!partials[i]->gate)
                _freePartialIndices.push_back(i);
        }
    }

    inline std::vector<ResPartial*> activePartials()
    {
        std::vector<ResPartial*> ret;
        for (int i = 0; i < maxNumberOfPartials; i++) {
            if (partials[i]->busy())
                ret.push_back(partials[i]);
        }
        return ret;
    }

    // moved from partial
    float fSamplingFreq = 44100; //ok
    float fConst0 = (6.2831855f / fmin(1.92e+05f, fmax(1.0f, (float)fSamplingFreq)));

    float _sharedBuffer[8192];
    float _sharedBufferLastSample = 0;

public:
    ResSynth();

    void setData(ResPartialData& data);
    
    float gainSmooth = 0.9997;
    float freqSmooth = 0;
    
    void setData(ResPartialData* data);

    ///> to be used when the updated frames have the same frequencies
    void setIndexedData(ResPartialData* data);

    void muteActivePartials();

    void process(const float* in_buffer, float* out_buffer, size_t s);;
};

#endif /* SineSynth_hpp */
