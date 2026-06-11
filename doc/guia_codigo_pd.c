/*********************************************/
/*********************************************/
/* pd sources guide of code in files*/
/*********************************************/
/*********************************************/
void d_arithmetic_setup(void)
{
    plus_setup();
    minus_setup();
    times_setup();
    over_setup();
    max_setup();
    min_setup();
}
/*********************************************/
void d_array_setup(void)
{
    tabwrite_tilde_setup();
    tabplay_tilde_setup();
    tabread_tilde_setup();
    tabread4_tilde_setup();
    tabosc4_tilde_setup();
    tabsend_setup();
    tabreceive_setup();
    tabread_setup();
    tabread4_setup();
    tabwrite_setup();
}
/*********************************************/
void d_ctl_setup(void)
{
    sig_tilde_setup();
    line_tilde_setup();
    vline_tilde_setup();
    snapshot_tilde_setup();
    vsnapshot_tilde_setup();
    env_tilde_setup();
    threshold_tilde_setup();
}
/*********************************************/
void d_dac_setup(void)
{
    dac_setup();
    adc_setup();
}
/*********************************************/
void d_math_setup(void)
{
    t_symbol *s = gensym("acoustics~.pd");
    clip_setup();
    sigrsqrt_setup();
    sigsqrt_setup();
    sigwrap_setup();
    mtof_tilde_setup();
    ftom_tilde_setup();
    dbtorms_tilde_setup();
    rmstodb_tilde_setup();
    dbtopow_tilde_setup();
    powtodb_tilde_setup();
    pow_tilde_setup();
    exp_tilde_setup();
    log_tilde_setup();
    abs_tilde_setup();
}
/*********************************************/
void d_misc_setup(void)
{
    print_setup();
    bang_tilde_setup();
}
/*********************************************/
void d_osc_setup(void)
{
    phasor_setup();
    cos_setup();
    osc_setup();
    sigvcf_setup();
    noise_setup();
}
/*********************************************/
void d_soundfile_setup(void)
{
    soundfiler_setup();
    readsf_setup();
    writesf_setup();
}
/*********************************************/
void x_time_setup(void)
{
//ver los prototipos de funciones de clock en m_sched y las definiciones en m_pd.h 
    delay_setup();
    metro_setup();
    line_setup();
    timer_setup();
    pipe_setup();
}
/*********************************************/
//ver funciones y macros en s_midi.c
void x_midi_setup(void)
{
    midiin_setup();
    midirealtimein_setup();
    notein_setup();
    ctlin_setup();
    pgmin_setup();
    bendin_setup();
    touchin_setup();
    polytouchin_setup();
    midiclkin_setup();
    midiout_setup();
    noteout_setup();
    ctlout_setup();
    pgmout_setup();
    bendout_setup();
    touchout_setup();
    polytouchout_setup();
    makenote_setup();
    stripnote_setup();
    poly_setup();
    bag_setup();
}
/*********************************************/

