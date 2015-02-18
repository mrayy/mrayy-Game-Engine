
#pragma once

#ifndef ___ADV_MATH___
#define ___ADV_MATH___

#include <math.h>

namespace mray{
namespace math{

double BezierBlend(int k,int n,double mu)
{
   int nn,nMax,nMin;
   double blend=1;

   nn = n;
   nMax=max(k,n - k);
   nMin=min(k,n - k);
   while (nn > nMax) {
      blend *= nn;
      nn--;
      if (nMin > 1) {
         blend /= (double)nMin;
         nMin--;
      }
   }
   if (k > 0)
      blend *= pow(mu,(double)k);
   if (n-k > 0)
      blend *= pow(1-mu,(double)(n-k));

   return(blend);
}


}//math
}//mray

#endif







