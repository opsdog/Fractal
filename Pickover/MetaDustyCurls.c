/*

Colormaps:
  4 - has potential - peppermint
  1,5,9,10,11 - has potential
  2 - smooth but little contrast

*/

#include <stdio.h>

main (int argc; char *argv;)
{
  double centerr, centeri, distr;
  double u1, u2;
  double test;
  int nwidth, dwell;
  FILE *DustyInput;

  DustyInput=fopen("DustyCurls-input","w");

  /* let's play with the formula in the loop... */

  centerr=0.0; centeri=0.0, distr=2.0; u1=0.0; u2=0.0;


  /* Sequence 1 u=(-0.74,0.25) */

  /*  centerr=0.0; centeri=0.0, distr=1.5; u1= -0.74; u2=0.25;*/
  /* zoom to here  centerr=0.0; centeri=0.0, distr=0.75; u1= -0.74; u2=0.25;*/
  /* move to here  centerr=0.434; centeri= -0.542, distr=0.075; u1= -0.74; u2=0.25;*/
  /* move to here  centerr=0.4665; centeri= -0.5540, distr=0.008; u1= -0.74; u2=0.25;*/
  /* move to here  centerr=0.46630; centeri= -0.55926, distr=0.0015; u1= -0.74; u2=0.25;*/
  /* move to here  centerr=0.466215; centeri= -0.559032, distr=0.00008; u1= -0.74; u2=0.25;*/
  /* move to blue center  centerr=0.466207; centeri= -0.5590400, distr=0.000025; u1= -0.74; u2=0.25;*/
  /* working on  centerr=0.4662047; centeri= -0.5590420, distr=0.000005; u1= -0.74; u2=0.25;*/

  /* sequence 2 u=(0.0,1.0) */
  /* runs out of precision before showing any real detail */
  /*  centerr=0.0; centeri=0.0, distr=1.5; u1=0.0; u2=1.0;*/
  /*  centerr=0.0; centeri=0.0, distr=0.00000005; u1=0.0; u2=1.0;*/

  /* sequence 3 u=(0.4,0.4) */
  /* positive r moves view to the right */
  /* positive i moves view down */
  /*  centerr=0.0; centeri=0.0, distr=4.0; u1=0.4; u2=0.4;*/
  /* move to  centerr=0.25; centeri=-0.75; distr=1.0; u1=0.4; u2=0.4;*/
  /* move to  */
  centerr=-0.12771; centeri=-0.53721466; distr=1.0; u1=0.4; u2=0.4;
  /* out of prec  centerr=-0.12771; centeri=-0.53721466; distr=0.000005; u1=0.4; u2=0.4;*/

  /* there's a neat still close to this - move up and to the left - VGA u=(-0.74,0.25) */
  /*  centerr=0.42; centeri= -0.53, distr=0.75; u1= -0.7; u2=0.25;*/

  /* another neat still close to here - VGA u=(-0.74,0.25) */
  /*  centerr=0.42; centeri= -0.53, distr=0.2; u1= -0.74; u2=0.25;*/

  /* another neat still close to here - VGA u=(-0.74,0.25) */
  /*  centerr=0.469; centeri= -0.557, distr=0.050; u1= -0.74; u2=0.25;*/

  /* neat still here - VGA u=(-0.74,0.25) */
  /*  centerr=0.46630; centeri= -0.55926, distr=0.0015; u1= -0.74; u2=0.25;*/


  /* pretty u=(0.4,0.4) */
  /*  centerr=0.25; centeri=-0.75; distr=1.0; u1=0.4; u2=0.4;*/

  /* nice still near here - fill corner to corner - u=(0.4,0.4) */
  /*  centerr=-0.1; centeri=-0.5; distr=0.7; u1=0.4; u2=0.4*/



  nwidth=640; test=2.0*1024.0*1024.0; dwell=8*1024;
  /*  nwidth=720; test=2.0*1024.0; dwell=4*1024;*/
  /*  nwidth=1024; test=2.0*1024.0*1024.0; dwell=1024;*/
  /*  nwidth=6000; test=2.0*1024.0*1024.0; dwell=1024;*/

  fprintf(DustyInput,"%d %e %d\n", nwidth, test, dwell);
  fprintf(DustyInput,"%e, %e, %e\n", centerr, centeri, distr);
  fprintf(DustyInput,"%e, %e\n",u1, u2);

  fclose(DustyInput);
}
