#ifndef Bayes_UppperLimit_H
#define Bayes_UppperLimit_H

/**   nev  ! number of observed events
      sac  ! relative uncertainty in acceptance
      xbg  ! expected background
      sbg  ! uncertainty in background
      cl   ! desired CL
      smax ! upper limit of integration
      prec ! integration step size
      plim ! upper limit on Poisson process
      perr ! error on upper limit on Poisson process */
double BayesianUpperLimit ( int nev, double sac, double xbg, double sbg, 
             double cl=.95, double prec=-1., double smax=0., bool prt=false );

#endif
