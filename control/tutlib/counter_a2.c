/*
Example taken from Zmolnig, Johannes:  How to write an external for Pure Data, 
Institut for    electronic music and acoustics (IEM),  
http://pdstatic.iem.at/externals-HOWTO/
*/
#include "C:/Program Files/Pd/src/m_pd.h"
#include "tutlib.h"
/******************************************************************/
/******************************************************************/
static t_class *counter_a2_class;
/******************************************************************/
/******************************************************************/
typedef struct _counter_a2 {
  t_object  x_obj;
  t_int i_count;
} t_counter_a2;
/******************************************************************/
/******************************************************************/
void counter_a2_bang(t_counter_a2 *x)
{
  t_float f=x->i_count;
  x->i_count++;
  outlet_float(x->x_obj.ob_outlet, f);
}
/******************************************************************/
void *counter_a2_new(t_floatarg f)
{
  t_counter_a2 *x = (t_counter_a2 *)pd_new(counter_a2_class);

  x->i_count=f;
  outlet_new(&x->x_obj, &s_float);

  return (void *)x;
}
/******************************************************************/
void counter_a2_setup(void) {
  counter_a2_class = class_new(gensym("counter_a2"),
        (t_newmethod)counter_a2_new,
        0, sizeof(t_counter_a2),
        CLASS_DEFAULT,
        A_DEFFLOAT, 0);

  class_addbang(counter_a2_class, counter_a2_bang);
  post("counter_a2\n");
}
/******************************************************************/
/******************************************************************/