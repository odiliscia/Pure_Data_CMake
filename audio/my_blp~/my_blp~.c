/*my_blp by Oscar Pablo Di Liscia*/
/*Sum of sines using a closed summation formula*/
/*
    The blp UG produces:

    n-1
    ___
    \  / 
     >  sin(T+k*B) = sin[T + ((n-1*B)/2)] * [sin(n*B/2)/sin(B/2)]   
    /__\

  k=0
  
  Where:
  n=number of sinusoidal components
  T=Phasor at starting frequency (Hz.)
  B=Phasor at Frequency interval between the components (Hz.)
  (Moore, Elements of Computer Music, 1990, pp. 271-272)
  */
#include "m_pd.h"
#include <math.h>

#define PI_D   3.14159265358979323846
#define PI2_D  6.28318530717958647692

// exact limit for the 64 mantissa
#define EPSILON_LIMIT 1e-12 

static t_class *my_blp_tilde_class;

typedef struct _my_blp_tilde {
    t_object  x_obj;
    t_sample  x_f;        // for CLASS_MAINSIGNALIN
    t_float   x_np;       // Npartials (from floatinlet)
    
    // phase accumulators
    t_float   x_phase_t;  // base frequency (T)
    t_float   x_phase_b;  // freq interval (B)
    t_float   x_sr;       // actual Sample rate
} t_my_blp_tilde;

/* --- perform func --- */
static t_int *my_blp_tilde_perform(t_int *w)
{
    t_my_blp_tilde *x = (t_my_blp_tilde *)(w[1]);
    t_sample  *in_frec = (t_sample *)(w[2]);  // Base freq (Left Inlet)
    t_sample  *in_step = (t_sample *)(w[3]);  // Interval (Right Inlet)
    t_sample  *out     = (t_sample *)(w[4]);  // Audio output
    t_int      n       = (t_int)(w[5]);


	t_float np     = floor(fabs(x->x_np)); 
    if (np < 1.0) np = 1.0;
  
    t_float ncm1 = np - 1.0;
    t_float amp_scal = 1.0 / np; 
    t_float pi2dsr = (t_float)PI2_D / x->x_sr;

    while (n--)
    {
        // reading input signals in case of parameters changes
        t_float frec_base = (t_float)*in_frec++;
        t_float frec_step = (t_float)*in_step++;

        t_float betad2 = x->x_phase_b * 0.5;
        t_float sinbetad2 = sin(betad2);
        t_float factor;

        // avoid zero division (L'Hôpital limit: when sin(B/2) -> 0)
        if (fabs(sinbetad2) < EPSILON_LIMIT)
        {
            factor = np; 
        }
        else
        {
            factor = sin(betad2 * np) / sinbetad2;
        }

        // Moore's Algorythm 
        t_float signal_sample = sin(x->x_phase_t + (betad2 * ncm1)) * factor;
        
        *out++ = (t_sample)(signal_sample * amp_scal);

        // (Phase Wrap)
        x->x_phase_t += frec_base * pi2dsr;
        if (x->x_phase_t >= (t_float)PI2_D) x->x_phase_t -= (t_float)PI2_D;
        if (x->x_phase_t < 0.0)             x->x_phase_t += (t_float)PI2_D;

        x->x_phase_b += frec_step * pi2dsr;
        if (x->x_phase_b >= (t_float)PI2_D) x->x_phase_b -= (t_float)PI2_D;
        if (x->x_phase_b < 0.0)             x->x_phase_b += (t_float)PI2_D;
    }

    return (w + 6);
}

/* --- DSP chain config --- */
static void my_blp_tilde_dsp(t_my_blp_tilde *x, t_signal **sp)
{
    x->x_sr = (t_float)sp[0]->s_sr;

    dsp_add(my_blp_tilde_perform, 5, (t_int *)x,
            sp[0]->s_vec,   // Inlet 1: Base Freq
            sp[1]->s_vec,   // Inlet 2: Freq interval
            sp[2]->s_vec,   // Audio Output
            sp[0]->s_n);    // Block size
}

static void *my_blp_tilde_new(t_floatarg np_init)
{ 
    t_my_blp_tilde *x = (t_my_blp_tilde *)pd_new(my_blp_tilde_class);
 
    // Inlets creation
	signalinlet_new(&x->x_obj, 0);			//Freq interval
	floatinlet_new(&x->x_obj, &x->x_np); 	//Partials number
	outlet_new(&x->x_obj, &s_signal);		//Audio signal outlet
	
    x->x_sr = (t_float)sys_getsr();
    x->x_np = (np_init > 0.0) ? (t_float)np_init : 10.0; 
    x->x_phase_t = 0.0;
    x->x_phase_b = 0.0;
    x->x_f = 0.0;
  
    return (void *)x;
}

void my_blp_tilde_setup(void) 
{
    my_blp_tilde_class = class_new(gensym("my_blp~"),
        (t_newmethod)my_blp_tilde_new, 0, 
        sizeof(t_my_blp_tilde), CLASS_DEFAULT, 
        A_DEFFLOAT, 0); 

    class_addmethod(my_blp_tilde_class, (t_method)my_blp_tilde_dsp, gensym("dsp"), A_CANT, 0);
    CLASS_MAINSIGNALIN(my_blp_tilde_class, t_my_blp_tilde, x_f);
  
    post("my_blp~: by Oscar Pablo Di Liscia (64-bit float version).");
}