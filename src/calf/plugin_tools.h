/* Calf DSP plugin pack
 * Tools to use in plugins
 *
 * Copyright (C) 2001-2010 Krzysztof Foltman, Markus Schmidt, Thor Harald Johansen and others
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 * Boston, MA 02111-1307, USA.
 */
#ifndef CALF_PLUGIN_TOOLS_H
#define CALF_PLUGIN_TOOLS_H

#include <config.h>

#include "giface.h"
#include "vumeter.h"

namespace calf_plugins {

/// Base class for universal stereo level metering
struct stereo_in_out_metering_base
{
    dsp::vumeter vumeter_in, vumeter_out;
    
    stereo_in_out_metering_base()
    {
        reset();
    }
    void reset()
    {
        vumeter_in.reset();
        vumeter_out.reset();
    }
};
    
/// Universal stereo level metering for a specific plugin
template<class Metadata>
class stereo_in_out_metering: public stereo_in_out_metering_base
{
public:
    inline void process(float *const *params, const float *const *inputs, const float *const *outputs, unsigned int offset, unsigned int nsamples)
    {
        if (params[Metadata::param_meter_in] || params[Metadata::param_clip_in]) {
            if (inputs)
                vumeter_in.update_stereo(inputs[0] ? inputs[0] + offset : NULL, inputs[1] ? inputs[1] + offset : NULL, nsamples);
            else
                vumeter_in.update_zeros(nsamples);
            if (params[Metadata::param_meter_in])
                *params[Metadata::param_meter_in] = vumeter_in.level;
            if (params[Metadata::param_clip_in])
                *params[Metadata::param_clip_in] = vumeter_in.clip > 0 ? 1.f : 0.f;
        }
        if (params[Metadata::param_meter_out] || params[Metadata::param_clip_out]) {
            if (outputs)
                vumeter_out.update_stereo(outputs[0] ? outputs[0] + offset : NULL, outputs[1] ? outputs[1] + offset : NULL, nsamples);
            else
                vumeter_out.update_zeros(nsamples);
            if (params[Metadata::param_meter_out])
                *params[Metadata::param_meter_out] = vumeter_out.level;
            if (params[Metadata::param_clip_out])
                *params[Metadata::param_clip_out] = vumeter_out.clip > 0 ? 1.f : 0.f;
        }
    }
    void bypassed(float *const *params, unsigned int nsamples)
    {
        reset();
        process(params, NULL, NULL, 0, nsamples);
    }
};

};

#endif

