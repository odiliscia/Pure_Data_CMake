/*reson~.c*/
/*
reson~ is a resonator filter (two pole with unity gain at 
resonance frequency, as described in Moore, 1990, pp. 268-269).
The bandwidth, center frequency and output amplitude may be set and changed
by the user.

INLETS:
leftmost inlet(1): 
		-audio inlet for the input source signal.
		-messages:
	 		print: shows reson~ status at PD console 		
inlet 2		:
		float: the resonator central frequency (Hz). 	
inlet 3		:
		float: the resonator bandwidth frequency (Hz). 
 	
OUTLETS:
outlet 1	: audio output.
	
*/
#include "m_pd.h"
#include <math.h>
/******************************************************************/
/******************************************************************/
static t_class *reson_tilde_class;
/******************************************************************/
/******************************************************************/
#define PI    	3.14159265358979323846264338327
#define PI2   	6.28318530717958647692528676654
/********************************************************/
/*DATA SPACE*/
/********************************************************/
/*two-pole resonator*/
typedef struct {
  t_float R;
  t_float G;
  t_float b1;
  t_float b2;
  t_float ih1;
  t_float ih2;
  t_float oh1;
  t_float oh2;
  double  sr;
}RESON;

typedef struct _reson_tilde {
  t_object x_obj;
  t_sample f;
  t_float  rfcin;
  t_float  rbwin;
  t_float  rfc_ant;
  t_float  rbw_ant;
  RESON    *rf;
  t_float  sr;
  t_float  amp_max;
} t_reson_tilde;

/******************************************************************/
t_float reson_f(t_float input, RESON *fdata);
void    set_reson(t_float frec, t_float bw, RESON *fdata, t_float srate);
void    reson_change(t_float frec, t_float bw, RESON *fdata);
void    reson_tilde_destroy(t_reson_tilde *x); 
void    reson_tilde_any(t_reson_tilde *x, t_symbol *s, int argc, t_atom *argv); 
/*******************************************************************/
t_int *reson_tilde_perform(t_int *w)
{
  t_reson_tilde *x   = (t_reson_tilde *)(w[1]);
  t_sample      *in  = (t_sample *)(w[2]);
  t_sample      *out = (t_sample *)(w[3]);
  t_int          n   = (t_int) (w[4]);

  if (x->rfcin != x->rfc_ant || x->rbwin != x->rbw_ant)
  {
      reson_change(x->rfcin, x->rbwin, x->rf);
      x->rfc_ant = x->rfcin;
      x->rbw_ant = x->rbwin;
  }
  while(n--) 
  {
      *out = reson_f(*in++, x->rf);	
      x->amp_max = (fabs(*out) > x->amp_max ? fabs(*out) : x->amp_max);	
      out++;
  }	
    	
  return (w+5);
}
/******************************************************************/
void reson_tilde_dsp(t_reson_tilde *x, t_signal **sp)
{ 
  dsp_add((t_perfroutine)reson_tilde_perform, 4, (t_int *)x, 
          (t_int *)sp[0]->s_vec, (t_int *)sp[1]->s_vec, (t_int)sp[0]->s_n);
}

/******************************************************************/
void *reson_tilde_new(void)
{ 
  t_reson_tilde *x = (t_reson_tilde *)pd_new(reson_tilde_class);
  x->amp_max = 0.;
  x->sr = (t_float)sys_getsr();
  x->rfcin = 440.0f; // arbitrary initialization
  x->rbwin = 44.0f;

  x->rf = (RESON*)getbytes(sizeof(RESON));
  
  // Set the initial filter data
  set_reson(x->rfcin, x->rbwin, x->rf, x->sr);
  
  // Keep the CF and BW initial values to know later if there is a change.
  x->rfc_ant = x->rfcin;
  x->rbw_ant = x->rbwin;
  
  floatinlet_new(&x->x_obj, &x->rfcin);
  floatinlet_new(&x->x_obj, &x->rbwin);

  outlet_new(&x->x_obj, &s_signal);
    
  return (void *)x;
}
/******************************************************************/
/******************************************************************/
void reson_tilde_setup(void) {

  reson_tilde_class = class_new(gensym("reson~"),
				    (t_newmethod)reson_tilde_new,
				    (t_method)reson_tilde_destroy, sizeof(t_reson_tilde), CLASS_DEFAULT, 0);	

  class_addanything(reson_tilde_class, reson_tilde_any);

  CLASS_MAINSIGNALIN(reson_tilde_class, t_reson_tilde, f);
  class_addmethod(reson_tilde_class,(t_method)reson_tilde_dsp, gensym("dsp"),0);
  
  post("\nreson~ external, Oscar Pablo Di Liscia, 2026 \n");

  return;
}
/******************************************************************/
void reson_tilde_any(t_reson_tilde *x, t_symbol *s, int argc, t_atom *argv)
{
    if (s == gensym("print"))
    {
        post("--- reson~ Status ---");
        post("Frecuencia central asignada (in): %f", x->rfcin);
        post("Bandwidth asignado (in): %f", x->rbwin);
        post("Amplitud maxima registrada: %f", x->amp_max);
        if (x->rf) {
            post("Filtro coeficientes internos -> G: %f, b1: %f, b2: %f", 
                 x->rf->G, x->rf->b1, x->rf->b2);
        }
    }
    else
    {
        pd_error(x, "reson~: mensaje '%s' no soportado", s->s_name);
    }
}
/******************************************************************/
void reson_tilde_destroy(t_reson_tilde *x) 
{
  freebytes(x->rf, sizeof(RESON));
  return;
}
/******************************************************************/
/******************************************************************/
/**************DEFINITIONS OF AUDIO FUNCTIONS *********************/
/******************************************************************/
/******************************************************************/
/******************************************************************/
void set_reson(t_float frec, t_float bw, RESON *fdata, t_float srate)
{
  /*
    From "Elements of Computer Music, F. R. Moore, 1990", pp.268-69
    R = exp(-PI*bw/SR)
    G = 1-R
    b1= 2R * cos(2*PI*f/SR)
    b2= -R*R
    here we use a[0] to store G and gain to store a general gain adjustment
  */

  fdata->sr  = (double)srate;
  fdata->R   = (t_float)exp(-PI*bw/fdata->sr);
  fdata->G   = 1. - fdata->R;
  fdata->b1  = 2. * fdata->R * cos(PI2*frec/fdata->sr);
  fdata->b2  = - (fdata->R*fdata->R);
  
  fdata->ih1 = fdata->ih2 = fdata->oh1 = fdata->oh2 = 0.;

  return;
}
/******************************************************************/
t_float  reson_f(t_float input, RESON *fdata)
{
  t_float output;
  /*difference equation*/
  /* y(n) = G*[x(n) - R*x(n-2)] + b1*y(n-1) + b2*y(n-2) */
 
  output= fdata->G*(input - fdata->R*fdata->ih2) + fdata->oh1*fdata->b1 + fdata->oh2*fdata->b2;
  
  fdata->oh2 = fdata->oh1;
  fdata->oh1 = output;
  fdata->ih2 = fdata->ih1;
  fdata->ih1 = input;

  return(output);
}
/******************************************************************/
void reson_change(t_float frec, t_float bw, RESON *fdata) 
{
  fdata->R   = (t_float)exp(-PI*bw/fdata->sr);
  fdata->G   = 1. - fdata->R;
  fdata->b1  = 2. * fdata->R * cos(PI2*frec/fdata->sr);
  fdata->b2  = - (fdata->R*fdata->R);
  
  return;
}
