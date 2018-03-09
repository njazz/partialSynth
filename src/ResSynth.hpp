

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

    const std::vector<int> freePartialIndices()
    {
        std::vector<int> ret;
        for (int i = 0; i < maxNumberOfPartials; i++) {
            if (!partials[i]->busy())
                ret.push_back(i);
        }
        return ret;
    }

    const std::vector<ResPartial*> activePartials()
    {
        std::vector<ResPartial*> ret;
        for (int i = 0; i < maxNumberOfPartials; i++) {
            if (partials[i]->busy())
                ret.push_back(partials[i]);
        }
        return ret;
    }

public:
    ResSynth()
    {
        for (int i = 0; i < maxNumberOfPartials; i++)
            partials.push_back(new ResPartial);
    }

    void setData(ResPartialData& data)
    {
        muteActivePartials();

        std::vector<int> freeIdx = freePartialIndices();

        int fi = (int)freeIdx.size();
        int s = (data.size < fi) ? (int)data.size : fi;

        for (int i = 0; i < s; i++) {
            int idx = freeIdx[i];
            if (idx > maxNumberOfPartials)
                continue;
            partials[idx]->set<ResPartial::pGain>(data.gain[i]);
            partials[idx]->set<ResPartial::pDecay>(data.decay[i]);
            partials[idx]->set<ResPartial::pFreq>(data.freq[i]);
            partials[idx]->setBusy(true);
        }
    };
    
    void setData(ResPartialData* data)
    {
        muteActivePartials();
        
        std::vector<int> freeIdx = freePartialIndices();
        
        int fi = (int)freeIdx.size();
        int s = (data->size < fi) ? (int)data->size : fi;
        
        for (int i = 0; i < s; i++) {
            int idx = freeIdx[i];
            if (idx > maxNumberOfPartials)
                continue;
            partials[idx]->set<ResPartial::pGain>(data->gain[i]);
            partials[idx]->set<ResPartial::pDecay>(data->decay[i]);
            partials[idx]->set<ResPartial::pFreq>(data->freq[i]);
            partials[idx]->setBusy(true);
        }
    };

    void muteActivePartials()
    {
        for (int i = 0; i < maxNumberOfPartials; i++) {
            if (partials[i]->busy()) {
                partials[i]->set<ResPartial::pGain>(0);
            }
        }
    }

    void process(const float* in_buffer, float* out_buffer, size_t s)
    {

        //float* buf = new float[s];
        for (int i = 0; i < s; i++) {
            _scratchBuffer[i] = in_buffer[i];
            out_buffer[i] = 0.;
        }

        for (ResPartial* sp : activePartials())
            sp->process(s, _scratchBuffer, out_buffer);

    };
};

#endif /* SineSynth_hpp */
