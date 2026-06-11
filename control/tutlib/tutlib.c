#include "C:/Program Files/Pd/src/m_pd.h"
#include "tutlib.h"
void tutlib_setup(){

  post("\nNEW TUTLIB LOADED, LIST OF OBJECTS AVAILABLE: \n");
  /*setup for each external*/
  hello2_setup();
  counter_a2_setup(); 
  counter_b2_setup(); 
 
  return;
}






