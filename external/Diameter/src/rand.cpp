// ========================================================================================
//  ApproxMVBB 
//  Copyright (C) 2014 by Gabriel Nützi <nuetzig (at) imes (d0t) mavt (d0t) ethz (døt) ch>
//  
//  Licensed under GNU General Public License 3.0 or later. 
//  Some rights reserved. See COPYING, README.rst.
//
//  @license GPL-3.0 <http://www.gnu.org/licenses/gpl-3.0.html>
// ========================================================================================

#include "ApproxMVBB/Diameter/Utils/rand.h"

namespace Diameter{

static long int _random_calls_ = 0;
static long int _random_seed_ = 0;

long int _GetRandomCalls()
{
  return( _random_calls_ );
}

long int _GetRandomSeed()
{
  return( _random_seed_ );
}



#ifdef WIN32
void _SetRandomSeed( unsigned int seed )
{
  srand( seed );
  _random_seed_ = seed;
  _random_calls_ = 0;
}
#else
void _SetRandomSeed( long int seed )
{
  srand48( seed );
  _random_seed_ = seed;
  _random_calls_ = 0;
}
#endif



#ifdef WIN32
double _GetRandomDoubleNb( )
{
  _random_calls_ ++;
  return( (double)rand()/(double)RAND_MAX );
}
#else
double _GetRandomDoubleNb( )
{
  _random_calls_ ++;
  return( drand48() );
}
#endif


int _GetRandomIntNb( int min, int max )
{
  if ( min <= max )
    return( (int)(floor( min + _GetRandomDoubleNb()*(double)(max-min+1.0) )) );
  return( (int)(floor( max + _GetRandomDoubleNb()*(double)(min-max+1.0) )) );
}

};
