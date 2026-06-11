/*
Example taken from Zmolnig, Johannes:  How to write an external for Pure Data, 
Institut for    electronic music and acoustics (IEM),  
http://pdstatic.iem.at/externals-HOWTO/
*/
#include "C:/Program Files/Pd/src/m_pd.h"
/***************************************************************/
/***************************************************************/
static t_class *hello_class;
/***************************************************************/
/***************************************************************/
typedef struct _hello {
  t_object  x_obj;
} t_hello;
/***************************************************************/
/***************************************************************/
void hello_bang(t_hello *x);
void *hello_new(void);
void hello_setup(void);
/***************************************************************/
/***************************************************************/
void hello_bang(t_hello *x)
{
  post("Hello pd newbie !!!");
}
/***************************************************************/
void *hello_new(void)
{
  t_hello *x = (t_hello *)pd_new(hello_class);

  return (void *)x;
}
/***************************************************************/
void hello_setup(void) {
  hello_class = class_new(gensym("hello"), 
        (t_newmethod)hello_new,
        0, sizeof(t_hello),
        CLASS_DEFAULT, 0);
  class_addbang(hello_class, hello_bang);
  post("\nHELLO external by Johannes Zmölnig loaded \n");
}
/***************************************************************/