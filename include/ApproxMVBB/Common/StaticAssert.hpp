// ========================================================================================
//  ApproxMVBB 
//  Copyright (C) 2014 by Gabriel Nützi <nuetzig (at) imes (d0t) mavt (d0t) ethz (døt) ch>
//  
//  Licensed under GNU General Public License 3.0 or later. 
//  Some rights reserved. See COPYING, README.rst.
//
//  @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
// ========================================================================================

#ifndef MVBB_Common_StaticAssert_hpp
#define MVBB_Common_StaticAssert_hpp


#include <type_traits>

namespace ApproxMVBB{
	#define STATIC_ASSERT(B) static_assert( B , "no message");
	#define STATIC_ASSERTM(B,COMMENT) static_assert( B , COMMENT);
};



#endif
