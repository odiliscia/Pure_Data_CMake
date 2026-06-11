/*argsi by Oscar Pablo Di Liscia*/
/*Shows how to read and filter different PD messages in differents (dedicated) inlets*/
#include "m_pd.h"
#include <string.h>
/******************************************************************/
/******************************************************************/
static t_class *argsi_class;
/******************************************************************/
/******************************************************************/
typedef struct _argsi {
  t_object  x_obj;
} t_argsi;
/******************************************************************/
/******************************************************************/
/*Function Definitions are not needed, but recommended for neatness*/
void argsi_bang(t_argsi *x); 						/*bang     method*/
void argsi_float(t_argsi *x, t_float f); 				/*float    method*/
void argsi_symbol(t_argsi *x, t_symbol *s);				/*symbol   method*/
void argsi_list(t_argsi *x, t_symbol *s, t_int argc, t_atom *argv);	/*list     method*/
void argsi_anything(t_argsi *x, t_symbol *s, t_int argc, t_atom *argv);	/*anything method*/
void *argsi_new(void);													
void argsi_setup(void);
/******************************************************************/
/******************************************************************/
void argsi_bang(t_argsi *x)
{
  post("\nargsi: bang received in the leftmost inlet (#1)");
}
/******************************************************************/
void argsi_float(t_argsi *x, t_float f)
{
  post("\nargsi: float (%f) received in the second inlet", f);
}
/******************************************************************/
void argsi_symbol(t_argsi *x, t_symbol *s)
{
	/*	
		s is a pointer of the symbol received
		The data structure member of s, s_name, is a pointer to
		a string which is the descriptor of the symbol
	*/
	post("\nargsi: symbol (%s) received in the third inlet", s->s_name);
}
/******************************************************************/
void argsi_list(t_argsi *x, t_symbol *s, t_int argc, t_atom *argv)
{
    int i;
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
void argsi_anything(t_argsi *x, t_symbol *s, t_int argc, t_atom *argv)
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
void *argsi_new(void)
{
  t_argsi *x = (t_argsi *)pd_new(argsi_class);

	/*the order of the creation functions sets the order of the inlets from left to right*/
	/*leftmost inlet (#1) is created by default*/
	inlet_new(&x->x_obj,&x->x_obj.ob_pd,gensym("float"),gensym("argsi_float")); 	/*inlet #2*/
	inlet_new(&x->x_obj,&x->x_obj.ob_pd,gensym("symbol"),gensym("argsi_symbol"));	/*inlet #3*/
	inlet_new(&x->x_obj,&x->x_obj.ob_pd,gensym("list"),gensym("argsi_list"));	/*rightmost inlet*/
	
	/*It is not possible to add an universal method for arbitrary selectors (i.e., "anything")
	  to a right inlet*/
	
  return (void *)x;
}
/******************************************************************/
void argsi_setup(void) {
  argsi_class = class_new(gensym("argsi"),
        (t_newmethod)argsi_new,
        0, sizeof(t_argsi),
        CLASS_DEFAULT,0);

/*Create methods for each one of the messages (gpointers excluded)*/

	class_addbang(argsi_class, argsi_bang); 
	class_addmethod(argsi_class, (t_method)argsi_float,  	gensym("argsi_float"),    A_DEFFLOAT, 0); 
	class_addmethod(argsi_class, (t_method)argsi_symbol, 	gensym("argsi_symbol"),   A_DEFSYMBOL,0);
	class_addmethod(argsi_class, (t_method)argsi_list,   	gensym("argsi_list"),     A_GIMME, 0);
	class_addmethod(argsi_class, (t_method)argsi_anything,  gensym("argsi_anything"), A_GIMME, 0);
	
	/*Since it is not possible to add a universal method for arbitrary selectors (i.e., "anything")
	to a right inlet, so, this method must be handled in the leftmost inlet allways*/
	class_addanything(argsi_class, argsi_anything);
  
  post("\nARGSI external by Oscar Pablo Di Liscia loaded\n");
}
/******************************************************************/
/******************************************************************/
