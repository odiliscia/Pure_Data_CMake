/*
Example taken from Zmolnig, Johannes:  How to write an external for Pure Data, 
Institut for    electronic music and acoustics (IEM),  
http://pdstatic.iem.at/externals-HOWTO/
*/
#include "C:/Program Files/Pd/src/m_pd.h"
#include "tutlib.h"
/***************************************************************/
/***************************************************************/
static t_class *hello2_class;
/***************************************************************/
/***************************************************************/
typedef struct _hello2 {
  t_object  x_obj;
} t_hello2;
/***************************************************************/
/***************************************************************/
void hello2_bang(t_hello2 *x);
void *hello2_new(void);
/***************************************************************/
/***************************************************************/
void hello2_bang(t_hello2 *x)
{
  post("Hello again, pd newbie !!!");
}
/***************************************************************/
void *hello2_new(void)
{
  t_hello2 *x = (t_hello2 *)pd_new(hello2_class);

  return (void *)x;
}
/***************************************************************/
void hello2_setup(void) {
  hello2_class = class_new(gensym("hello2"), 
        (t_newmethod)hello2_new,
        0, sizeof(t_hello2),
        CLASS_DEFAULT, 0);
  class_addbang(hello2_class, hello2_bang);
  post("hello2\n");
}
/***************************************************************/