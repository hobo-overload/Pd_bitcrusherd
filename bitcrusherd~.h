//
//  bitcrusherd~.h
//  Pd_bitcrusherd~
//
//  Copyright © 2024 Clint Woker. All rights reserved.
//

#pragma once

typedef struct _bitcrusherd_tilde {
    t_object  x_obj;
    t_sample f_bitdepth;
    t_sample f;
    
    t_inlet  *x_in2;
    t_outlet *x_out;
} t_bitcrusherd_tilde;

void bitcrusherd_tilde_setup(void);
void *bitcrusherd_tilde_new(t_floatarg bits);
void bitcrusherd_tilde_free(t_bitcrusherd_tilde *x);
void bitcrusherd_tilde_dsp(t_bitcrusherd_tilde *x, t_signal **sp);
t_int *bitcrusherd_tilde_perform(t_int *w);
