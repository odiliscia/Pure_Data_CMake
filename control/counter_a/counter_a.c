/*
Example taken from Zmolnig, Johannes:  How to write an external for Pure Data, 
Institut for    electronic music and acoustics (IEM),  
http://pdstatic.iem.at/externals-HOWTO/
*/
#include "C:/Program Files/Pd/src/m_pd.h"
/******************************************************************/
/******************************************************************/
static t_class *counter_a_class;
/******************************************************************/
/******************************************************************/
typedef struct _counter_a {
  t_object  x_obj;
  t_int i_count;
} t_counter_a;
/******************************************************************/
/******************************************************************/
void counter_a_bang(t_counter_a *x)
{
  t_float f=x->i_count;
  x->i_count++;
  outlet_float(x->x_obj.ob_outlet, f);
}
/******************************************************************/
void *counter_a_new(t_floatarg f)
{
  t_counter_a *x = (t_counter_a *)pd_new(counter_a_class);

  x->i_count=f;
  outlet_new(&x->x_obj, &s_float);

  return (void *)x;
}
/******************************************************************/
void counter_a_setup(void) {
  counter_a_class = class_new(gensym("counter_a"),
        (t_newmethod)counter_a_new,
        0, sizeof(t_counter_a),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0);

  class_addbang(counter_a_class, counter_a_bang);
  post("\nCOUNTER_A external by Johannes Zmölnig loaded\n");
}
/******************************************************************/
/******************************************************************/