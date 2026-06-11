/* 
 * my_tabread4~ : External individual basado en el tabread4~ moderno de Miller Puckette.
 * Adaptado para compilar por fuera del core usando únicamente la API pública de Pd.
 * Soporta audio multicanal y precisión de 32/64 bits (t_word).
 */

#include "m_pd.h"

static t_class *my_tabread4_tilde_class;

typedef struct _my_tabread4_tilde
{
    t_object  x_obj;
    t_symbol *x_arrayname; // Guarda el nombre de la tabla objetivo
    t_float   x_f;         // Requerido por CLASS_MAINSIGNALIN
} t_my_tabread4_tilde;

/* --- FUNCIÓN DE RENDIMIENTO (INNER LOOP) --- */
static t_int *my_tabread4_tilde_perform(t_int *w)
{
    t_symbol *arrayname = (t_symbol *)(w[1]);
    t_sample *in = (t_sample *)(w[2]);
    t_sample *onset = (t_sample *)(w[3]);
    t_sample *out = (t_sample *)(w[4]);
    t_int n = (t_int)(w[5]);
    
    int maxindex;
    t_word *buf;
    int i;
    const t_sample one_over_six = 1./6.;

    // Buscamos la tabla de forma segura en la API pública
    t_garray *a = (t_garray *)pd_findbyclass(arrayname, garray_class);
    
    // Si la tabla no existe o no se pueden obtener sus datos, salimos a silencio
    if (!a || !garray_getfloatwords(a, &maxindex, &buf))
        goto zero;

    maxindex -= 3;
    if (maxindex < 1)
        goto zero;

    for (i = 0; i < n; i++)
    {
        double findex = (double)*in++ + (double)*onset++;
        int index = findex;
        t_sample frac, a_val, b, c, d, cminusb;
        t_word *wp;

        if (index < 1)
            index = 1, frac = 0;
        else if (index > maxindex)
            index = maxindex, frac = 1;
        else 
            frac = findex - index;
            
        wp = buf + index;
        a_val = wp[-1].w_float; // Cambiado 'a' por 'a_val' para evitar confusiones léxicas
        b = wp[0].w_float;
        c = wp[1].w_float;
        d = wp[2].w_float;
        cminusb = c - b;
        
        // Algoritmo original de interpolación cúbica de Miller Puckette
        *out++ = b + frac * (
            cminusb - one_over_six * ((t_sample)1.-frac) * (
                (d - a_val - (t_sample)3.0 * cminusb) * frac +
                (d + a_val*(t_sample)2.0 - b*(t_sample)3.0)
            )
        );
    }
    return (w+6);

 zero:
    while (n--)
        *out++ = 0;

    return (w+6);
}

/* --- MENSAJE 'SET' --- */
/* Permite cambiar el nombre de la tabla en tiempo de ejecución de forma dinámica */
static void my_tabread4_tilde_set(t_my_tabread4_tilde *x, t_symbol *s)
{
    x->x_arrayname = s;
}

/* --- CONFIGURACIÓN DEL DSP (SOPORTE MULTICANAL) --- */
static void my_tabread4_tilde_dsp(t_my_tabread4_tilde *x, t_signal **sp)
{
    int length = sp[0]->s_length;
    
    // En la API multicanal, determinamos cuántos canales procesar basándonos en la entrada
    int nchans = sp[0]->s_nchans;
    if (sp[1]->s_nchans > nchans) 
        nchans = sp[1]->s_nchans;
        
    // Configuramos los canales de salida del objeto
    signal_setmultiout(&sp[2], nchans);

    // Registramos el uso de la tabla en el DSP de Pd para evitar crasheos si se borra la tabla
    t_garray *a = (t_garray *)pd_findbyclass(x->x_arrayname, garray_class);
    if (a) {
        garray_usedindsp(a);
    }

    // Agregamos un perform por cada canal de audio activo
    for (int i = 0; i < nchans; i++)
    {
        dsp_add(my_tabread4_tilde_perform, 5, 
            x->x_arrayname,                                   // w[1]
            sp[0]->s_vec + (i % sp[0]->s_nchans) * length,    // w[2] (Input)
            sp[1]->s_vec + (i % sp[1]->s_nchans) * length,    // w[3] (Onset)
            sp[2]->s_vec + i * length,                        // w[4] (Output)
            (t_int)length                                     // w[5]
        );
    }
}

/* --- CONSTRUCTOR --- */
static void *my_tabread4_tilde_new(t_symbol *s)
{
    t_my_tabread4_tilde *x = (t_my_tabread4_tilde *)pd_new(my_tabread4_tilde_class);
    
    x->x_arrayname = s;
    x->x_f = 0;
    
    // Creamos los Inlets y Outlets de señal obligatorios
    signalinlet_new(&x->x_obj, 0);                 // Entrada de Onset (señal derecha)
    outlet_new(&x->x_obj, gensym("signal"));       // Salida de señal principal
    
    return (void *)x;
}

/* --- INICIALIZACIÓN DE LA CLASE --- */
void my_tabread4_tilde_setup(void)
{
    // CLASS_MULTICHANNEL habilita el soporte de cables gruesos de audio en Pd Vanilla moderno
    my_tabread4_tilde_class = class_new(gensym("my_tabread4~"),
        (t_newmethod)my_tabread4_tilde_new, 0,
        sizeof(t_my_tabread4_tilde), CLASS_MULTICHANNEL, A_DEFSYM, 0);
        
    CLASS_MAINSIGNALIN(my_tabread4_tilde_class, t_my_tabread4_tilde, x_f);
    
    class_addmethod(my_tabread4_tilde_class, (t_method)my_tabread4_tilde_dsp,
        gensym("dsp"), A_CANT, 0);
        
    class_addmethod(my_tabread4_tilde_class, (t_method)my_tabread4_tilde_set,
        gensym("set"), A_DEFSYM, 0);
		
	post("my_tabread4~: by Miller Puckette (64-bit float version).");
}
