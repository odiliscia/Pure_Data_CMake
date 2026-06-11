/*argsio by Oscar Pablo Di Liscia*/
/*Shows how to read and filter different PD messages and how to 
output different control messages via dedicated outlets*/
#include "C:/Program Files/Pd/src/m_pd.h"
/******************************************************************/
/******************************************************************/
static t_class *argsio_class;
/******************************************************************/
/******************************************************************/
typedef struct _argsio {
  t_object  x_obj;
  t_outlet	*out_bang;
  t_outlet	*out_float;
  t_outlet	*out_symbol;
  t_outlet	*out_list;
  t_outlet	*out_any;
} t_argsio;
/******************************************************************/
/******************************************************************/
/*Function Definitions are not needed, but recommended for neatness*/
void argsio_bang(t_argsio *x);													/*bang     method*/
void argsio_float(t_argsio *x, t_float f);										/*float    method*/
void argsio_symbol(t_argsio *x, t_symbol *s);									/*symbol   method*/
void argsio_list(t_argsio *x, t_symbol *s, t_int argc, t_atom *argv);			/*list     method*/
void argsio_anything(t_argsio *x, t_symbol *s, t_int argc, t_atom *argv);		/*anything method*/
void *argsio_new(void);													
void argsio_setup(void);
/******************************************************************/
/******************************************************************/
void argsio_bang(t_argsio *x)
{
  outlet_bang(x->out_bang);
}
/******************************************************************/
void argsio_float(t_argsio *x, t_float f)
{
  outlet_float(x->out_float, f);
}
/******************************************************************/
void argsio_symbol(t_argsio *x, t_symbol *s)
{
	outlet_symbol(x->out_symbol, s);
}
/******************************************************************/
void argsio_list(t_argsio *x, t_symbol *s, t_int argc, t_atom *argv)
{
	outlet_list(x->out_list, s, argc, argv);
}
/******************************************************************/
void argsio_anything(t_argsio *x, t_symbol *s, t_int argc, t_atom *argv)
{
	outlet_anything(x->out_any, s, argc, argv);
}
/******************************************************************/
void *argsio_new(void)
{
  t_argsio *x = (t_argsio *)pd_new(argsio_class);

	/*Create three inlets, from left to right*/
	/*leftmost inlet (#1) for bang and anything is created by default*/
	inlet_new(&x->x_obj,&x->x_obj.ob_pd,gensym("float"),	gensym("argsio_float")); 	/*inlet #2*/
	inlet_new(&x->x_obj,&x->x_obj.ob_pd,gensym("symbol"),	gensym("argsio_symbol"));	/*inlet #3*/
	inlet_new(&x->x_obj,&x->x_obj.ob_pd,gensym("list"),		gensym("argsio_list"));		/*rightmost inlet #4*/
	/*NB: It is not possible to add a universal method for arbitrary selectors (i.e., "anything")
	  to a right inlet so, the "anything" is forced to be received in the leftmost inlet exclusively*/
	
	/*Create five outlets, from left to right*/  
	x->out_bang 	= outlet_new(&x->x_obj, &s_bang);		/*outlet #1 (leftmost)	*/
	x->out_float 	= outlet_new(&x->x_obj, &s_float);		/*outlet #2				*/
	x->out_symbol 	= outlet_new(&x->x_obj, &s_symbol);		/*outlet #3				*/
	x->out_list 	= outlet_new(&x->x_obj, &s_list);		/*outlet #4				*/
	x->out_any 		= outlet_new(&x->x_obj, 0);				/*outlet #5 (rigthmost)	*/
	
  return (void *)x;
}
/******************************************************************/
void argsio_setup(void) {

  argsio_class = class_new(gensym("argsio"),
        (t_newmethod)argsio_new,
        0, sizeof(t_argsio),
        CLASS_DEFAULT,0);
		
/*Create methods for each one of the messages (gpointers excluded)*/
	class_addbang(argsio_class, argsio_bang); 
	class_addmethod(argsio_class, (t_method)argsio_float,  	  gensym("argsio_float"),  	A_DEFFLOAT, 0); 
	class_addmethod(argsio_class, (t_method)argsio_symbol, 	  gensym("argsio_symbol"), 	A_DEFSYMBOL,0);
	class_addmethod(argsio_class, (t_method)argsio_list,   	  gensym("argsio_list"),   	A_GIMME, 0);
	class_addmethod(argsio_class, (t_method)argsio_anything,  gensym("argsio_anything"),A_GIMME, 0);
	
	/*Since it is not possible to add a universal method for arbitrary selectors (i.e., "anything")
	to a right inlet, this method must be handled in the leftmost inlet allways*/
	class_addanything(argsio_class, argsio_anything);
  
  post("\nARGSIO external by Oscar Pablo Di Liscia loaded\n");
}
/******************************************************************/
/******************************************************************/