/*
Example taken from Zmolnig, Johannes:  How to write an external for Pure Data, 
Institut for    electronic music and acoustics (IEM),  
http://pdstatic.iem.at/externals-HOWTO/
*/
#include "C:/Program Files/Pd/src/m_pd.h"
#include "tutlib.h"
/******************************************************************/
/******************************************************************/
static t_class *counter_b2_class;
/******************************************************************/
/******************************************************************/
typedef struct _counter_b2 {
  t_object  x_obj;
  t_int i_count;
  t_float step;
  t_int i_down, i_up;
  t_outlet *f_out, *b_out;
} t_counter_b2;
/******************************************************************/
/******************************************************************/
void counter_b2_bang(t_counter_b2 *x)
{
  t_float f=x->i_count;
  t_int step = x->step;
  x->i_count+=step;

  if (x->i_down-x->i_up) {
    if ((step>0) && (x->i_count > x->i_up)) {
      x->i_count = x->i_down;
      outlet_bang(x->b_out);
    } else if (x->i_count < x->i_down) {
      x->i_count = x->i_up;
      outlet_bang(x->b_out);
    }
  }

  outlet_float(x->f_out, f);
}
/******************************************************************/
void counter_b2_reset(t_counter_b2 *x)
{
  x->i_count = x->i_down;
}
/******************************************************************/
void counter_b2_set(t_counter_b2 *x, t_floatarg f)
{
  x->i_count = f;
}
/******************************************************************/
void counter_b2_bound(t_counter_b2 *x, t_floatarg f1, t_floatarg f2)
{
  x->i_down = (f1<f2)?f1:f2;
  x->i_up   = (f1>f2)?f1:f2;
}
/******************************************************************/
void *counter_b2_new(t_symbol *s, int argc, t_atom *argv)
{
  t_counter_b2 *x = (t_counter_b2 *)pd_new(counter_b2_class);
  t_float f1=0, f2=0;

  x->step=1;
  switch(argc){
  default:
  case 3:
    x->step=atom_getfloat(argv+2);
  case 2:
    f2=atom_getfloat(argv+1);
  case 1:
    f1=atom_getfloat(argv);
    break;
  case 0:
	break; 
  }
  if (argc<2)f2=f1;

  x->i_down = (f1<f2)?f1:f2;
  x->i_up   = (f1>f2)?f1:f2;

  x->i_count=x->i_down;

  inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("list"), gensym("bound"));
  floatinlet_new(&x->x_obj, &x->step);

  x->f_out = outlet_new(&x->x_obj, &s_float);
  x->b_out = outlet_new(&x->x_obj, &s_bang);

  return (void *)x;
}
/******************************************************************/
void counter_b2_setup(void) {
  counter_b2_class = class_new(gensym("counter_b2"),
        (t_newmethod)counter_b2_new,
        0, sizeof(t_counter_b2),
        CLASS_DEFAULT, 
        A_GIMME, 0);

  class_addbang  (counter_b2_class, counter_b2_bang);
  class_addmethod(counter_b2_class,
        (t_method)counter_b2_reset, gensym("reset"), 0);
  class_addmethod(counter_b2_class, 
        (t_method)counter_b2_set, gensym("set"),
        A_DEFFLOAT, 0);
  class_addmethod(counter_b2_class,
        (t_method)counter_b2_bound, gensym("bound"),
        A_DEFFLOAT, A_DEFFLOAT, 0);

  class_sethelpsymbol(counter_b2_class, gensym("help-counter_b2"));
  post("counter_b2\n");
}
/******************************************************************/
/******************************************************************/
