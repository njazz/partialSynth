//
//  SineSynth.cpp
//  PartialSynth
//
//  Created by Alex on 04/02/2018.
//  Copyright © 2018 Alex Nadzharov. All rights reserved.
//

#include "ResSynth.hpp"

ResSynth::ResSynth()
{
    for (int i = 0; i < maxNumberOfPartials; i++)
        partials.push_back(new ResPartial);
}

void ResSynth::setData(ResPartialData& data)
{
    setData(&data);
};

float gainSmooth = 0.9997;
float freqSmooth = 0;

void ResSynth::setData(ResPartialData* data)
{
    muteActivePartials();

    updateFreePartialIndices();

    int fi = (int)_freePartialIndices.size();
    int s = (data->size < fi) ? (int)data->size : fi;

    for (int i = 0; i < s; i++) {
        int idx = _freePartialIndices[i];
        if (idx > maxNumberOfPartials)
            continue;

        partials[idx]->setSmooth<ResPartial::pGain>(gainSmooth);
        partials[idx]->setSmooth<ResPartial::pFreq>(0);
        partials[idx]->set<ResPartial::pGain>(data->gain[i]);
        partials[idx]->set<ResPartial::pDecay>(data->decay[i]);
        partials[idx]->set<ResPartial::pFreq>(data->freq[i]);
        partials[idx]->updateFreq();
        partials[idx]->setBusy(true);
        partials[idx]->gate = true;
    }
};

///> to be used when the updated frames have the same frequencies
void ResSynth::setIndexedData(ResPartialData* data)
{
    int s = (data->size < 1024) ? (int)data->size : 1024;

    for (int i = 0; i < s; i++) {
        if (partials[i]->value<ResPartial::pFreq>() == data->freq[i])
            if (partials[i]->value<ResPartial::pGain>() == data->gain[i])
                continue;

        partials[i]->setSmooth<ResPartial::pGain>(gainSmooth);
        partials[i]->setSmooth<ResPartial::pFreq>(freqSmooth);
        partials[i]->set<ResPartial::pGain>(data->gain[i]);
        partials[i]->set<ResPartial::pDecay>(data->decay[i]);
        partials[i]->set<ResPartial::pFreq>(data->freq[i]);
        partials[i]->updateFreq();
        partials[i]->setBusy(true);
        partials[i]->gate = true;
    }
    for (int i = s; i < 1024; i++) {
        partials[i]->gate = false;
    }
}

void ResSynth::muteActivePartials()
{
    for (int i = 0; i < maxNumberOfPartials; i++) {
        if (partials[i]->busy()) {
            partials[i]->setSmooth<ResPartial::pGain>(gainSmooth);
            partials[i]->set<ResPartial::pGain>(freqSmooth);
        }
    }
}

void ResSynth::process(const float* in_buffer, float* out_buffer, size_t s)
{

    for (int i = 0; i < s; i++) {
        _scratchBuffer[i] = in_buffer[i];
        out_buffer[i] = 0.;
    }

    _sharedBuffer[0] = _sharedBufferLastSample;
    for (int i = 0; i < s; i++)
        _sharedBuffer[i] = fConst0 * _scratchBuffer[i];

    for (ResPartial* sp : partials) {
        if (sp->busy()) {
            sp->process(s, _scratchBuffer, out_buffer);
        }
    }
};
