/*args by Oscar Pablo Di Liscia*/
/*Shows how to read and filter different PD messages*/
#include "C:/Program Files/Pd/src/m_pd.h"
#include <string.h>
/******************************************************************/
/******************************************************************/
static t_class *args_class;
/******************************************************************/
/******************************************************************/
typedef struct _args {
  t_object  x_obj;
} t_args;
/******************************************************************/
/******************************************************************/
/*Function Definitions are not needed, but recommended for neatness*/
void args_bang(t_args *x);												/*bang     method*/
void args_float(t_args *x, t_float f);									/*float    method*/
void args_symbol(t_args *x, t_symbol *s);								/*symbol   method*/
void args_list(t_args *x, t_symbol *s, t_int argc, t_atom *argv);		/*list     method*/
void args_anything(t_args *x, t_symbol *s, t_int argc, t_atom *argv);	/*anything method*/
void *args_new(void);													
void args_setup(void);
/******************************************************************/
//Function prototypes
/******************************************************************/
void args_bang(t_args *x)
{
  post("\nargs: bang received");
}
/******************************************************************/
void args_float(t_args *x, t_float f)
{
  post("\nargs: float (%f) received", f);
}
/******************************************************************/
void args_symbol(t_args *x, t_symbol *s)
{
	/*	
		s is a pointer of the symbol received
		The data structure member of s, s_name, is a pointer to
		a string which is the symbol
	*/
	post("\nargs: symbol (%s) received", s->s_name);
}
/******************************************************************/
void args_list(t_args *x, t_symbol *s, t_int argc, t_atom *argv)
{
    int i;
    post("\nargs: %s received has %d args", s->s_name, argc);
    for(i = 0; i < argc; ++i) {
        // see m_pd.h types and macros
        if(argv[i].a_type == A_FLOAT) {
            post("float(%f), \t\t\t at list[%d]", atom_getfloat(&argv[i]), i);
        }	
        else if(argv[i].a_type == A_SYMBOL) {
            post("symbol (%s), \t\t\t at list[%d]", atom_getsymbol(&argv[i])->s_name, i);
        }
    }
}
/******************************************************************/
void args_anything(t_args *x, t_symbol *s, t_int argc, t_atom *argv)
{
    int i;
	/*
	Same case of list, but without the list descriptor.
	Beware: in this case s->s_name stores the selector (first argument).
	*/
    post("\nargs: anything received with %d args", argc);
    post("selector (%s), \t\t\t at s->s_name", s->s_name); // the selector
    for(i = 0; i < argc; ++i) {
        if(argv[i].a_type == A_FLOAT) {
            post("float(%f), \t\t\t at list[%d]", atom_getfloat(&argv[i]), i);
        }	
        else if(argv[i].a_type == A_SYMBOL) {
            post("symbol (%s), \t\t\t at list[%d]", atom_getsymbol(&argv[i])->s_name, i);
        }
    }
}
/******************************************************************/
void *args_new(void)
{
  t_args *x = (t_args *)pd_new(args_class);

  return (void *)x;
}
/******************************************************************/
void args_setup(void) {
  args_class = class_new(gensym("args"),
        (t_newmethod)args_new,
        0, sizeof(t_args),
        CLASS_DEFAULT,0);
/*Create methods for each one of the messages (gpointers excluded)*/
  class_addbang(args_class, args_bang);
  class_addfloat(args_class, args_float);
  class_addsymbol(args_class, args_symbol);
  class_addlist(args_class, args_list);
  class_addanything(args_class, args_anything);
  
  post("\nARGS external by Oscar Pablo Di Liscia loaded\n");
}
/******************************************************************/
/******************************************************************/