//
//  bitcrusherd~.c
//  Pd_bitcrusherd~
//
//  Copyright © 2024 Clint Woker. All rights reserved.
//
//  This file implements a Pd external to perform a "bit crush"
//  audio effect by reducing the effective bitdepth of the incoming
//  audio signal.
//  The first (left) inlet is the audio signal
//  (thus the tilde naming, per typical Pd naming convention).
//  The second (right) inlet is the number of bits for the desired bit depth.

//  Note that the bit depth is floating point, as is typical in Pd; log base 2 of the
//  desired number of discrete "steps" in the output waveform,
//  applied to the second inlet,
//  is an alternate way of thinking about usage of this external.

//  One can think of this Pd external as performing a "vertical" reduction of possible values
//  on an audio waveform, where the waveform has time on the x-axis and amplitude on the y-axis.


#include <math.h>
#include "m_pd.h"
#include "bitcrusherd~.h"

static t_class *bitcrusherd_tilde_class;

// boilerplate setup of the external symbol in Pd.
// The external will be available in Pd as "bitcrusherd~".
void bitcrusherd_tilde_setup(void) {
    bitcrusherd_tilde_class = class_new(gensym("bitcrusherd~"),
                                       (t_newmethod)bitcrusherd_tilde_new,
                                       (t_method)bitcrusherd_tilde_free,
                                       sizeof(t_bitcrusherd_tilde),
                                       CLASS_DEFAULT,
                                       A_DEFFLOAT, 0);
    class_addmethod(bitcrusherd_tilde_class,
                    (t_method)bitcrusherd_tilde_dsp, gensym("dsp"), 0);
    CLASS_MAINSIGNALIN(bitcrusherd_tilde_class, t_bitcrusherd_tilde, f);
}

// Establish a floating point right inlet for bit depth,
// and a signal as output.
void *bitcrusherd_tilde_new(t_floatarg bitdepth)
{
    t_bitcrusherd_tilde *x = (t_bitcrusherd_tilde *)pd_new(bitcrusherd_tilde_class);
    x->f_bitdepth = bitdepth;
    x->x_in2 = floatinlet_new(&x->x_obj, &x->f_bitdepth);
    x->x_out = outlet_new(&x->x_obj, &s_signal);
    return (void *)x;
}

// Cleanup operations for inlets and outlets which
// were created in the ..._new function.
void bitcrusherd_tilde_free(t_bitcrusherd_tilde *x)
{
    inlet_free(x->x_in2);
    outlet_free(x->x_out);
}

// Boilerplate operation to add a signal input to the left inlet.
void bitcrusherd_tilde_dsp(t_bitcrusherd_tilde *x, t_signal **sp)
{
    dsp_add(bitcrusherd_tilde_perform, 4, x,
            sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

// Depth bitcrusher algorithm.
// Pd issues a callback on this function for every batch of samples received on the left (DSP) inlet.
// Note that inputs and output signals are floating point values!
// As such the options are either to allow floating point number of bits,
// or perform "real" bit shifts only on the exponent portion of the floating point values.
// I thought it more interesting to allow floating point number of bits.
t_int *bitcrusherd_tilde_perform(t_int *w)
{
    t_bitcrusherd_tilde *x = (t_bitcrusherd_tilde *)(w[1]);
    t_sample  *in1 = (t_sample *)(w[2]);
    t_sample  *out = (t_sample *)(w[3]);
    for (int n = (int)(w[4]); n; n--) {
        // Algorithm in long form, commented out,
        // followed by a shorter implementation:
        /*
         *out = *in1;  // start by setting the output signal value equal to the input signal
         *out += 1; // transpose the output value from [-1, 1] to [0,2]
         *out /= 2; // scale the value from [0,2] to [0,1]
         *out *= (exp2f(x->f_bits) - 1); // scale the value from [0, 2^bits - 1]
         *out = nearbyintf(*out); // Round nearest integer in [0, 2^bits - 1]. Applies small amount of "roughness" to the algorithm.
         *out /= (exp2f(x->f_bits) - 1); //scale the value back to [0,1]
         *out *= 2; // scale the value from [0,1] to [0,2]
         *out -= 1; // transpose the output value from [0,2] back to typical Pd format [-1,1]
         */
        
        // The shorter version:
        *out = nearbyintf((*in1 + 1) / 2 * (exp2f(x->f_bitdepth) - 1)) /
            (exp2f(x->f_bitdepth) - 1) * 2 - 1;
        
        // Advance to next sample value in the batch:
        (void) *in1++;
        (void) *out++;
    }
    // return the pointer to the next set of block information, per Pd convention
    return (w+5);
}


