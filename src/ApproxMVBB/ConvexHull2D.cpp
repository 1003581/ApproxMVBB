// ========================================================================================
//  ApproxMVBB
//  Copyright (C) 2014 by Gabriel Nützi <nuetzig (at) imes (d0t) mavt (d0t) ethz (døt) ch>
//
//  Licensed under GNU General Public License 3.0 or later.
//  Some rights reserved. See COPYING, README.rst.
//
//  @license GPL-3.0 <http://www.gnu.org/licenses/gpl-3.0.html>
// ========================================================================================
//#include <iterator> // for ostream_iterator
//#include "TestFunctions.hpp"


#include "ApproxMVBB/ConvexHull2D.hpp"
namespace ApproxMVBB{
void ConvexHull2D::compute() {
    using namespace PointFunctions;
    using namespace ContainerFunctions;

    m_indicesCH.clear();

    // Need at least 1 point! m_p.col(0)
    if(m_p.cols()==0) {
        return;
    }

    // Compute min point p0
    unsigned int position = minPointYX(m_p);
//    std::cout << "min:" << position << std::endl;
    Vector2 base = m_p.col(position);

    // Indices into m_p
    // first = index into m_p, second =  delete flag!
    using PointData = std::pair<unsigned int, bool>;
    std::vector< PointData > indices; indices.reserve(m_p.cols());

    //Save p0 as first point in indices list (temporary)
    indices.emplace_back(position,false);
    //Add all indices of points not almostEqual to position
    for(unsigned int i = 0; i<m_p.cols(); ++i) {
        if( (i != position) && !almostEqualUlp(base,m_p.col(i))) {
            indices.emplace_back(i,false);
        }
    }

    // Convex hull consists of only 1 or 2 points!
    if(indices.size() <= 2  ) {
        for(auto & pa : indices){ m_indicesCH.emplace_back(pa.first);}  return;
    }

    // Sort by angle
    unsigned int deletedPoints = 0;
    CompareByAngle comp(m_p,base,position,deletedPoints);
    std::sort( indices.begin()+1, indices.end(), comp );


    // Remove almost equal elements
    // move to front if not almost equal to first point
    // skip all deleted points
    auto ifFunc =  [&](const PointData & p1, const PointData & p2){
                        return !almostEqualUlp(this->m_p.col(p1.first),this->m_p.col(p2.first));
                      };
    auto skipFunc = [](const PointData & p1){ return p1.second; };
    auto d2 = ContainerFunctions::moveConsecutiveToFrontIf(indices.begin(),indices.end(), ifFunc, skipFunc);
        std::cout << "Deleted almostEqual: " << std::distance(d2,indices.end()) << std::endl;
    indices.resize( std::distance(indices.begin(),d2) );

    // Convex hull consists of only 1 or 2 points!
    if(indices.size() <= 2  ) {
        for(auto & pa : indices){ m_indicesCH.emplace_back(pa.first);}  return;
    }

    for(auto  a: indices){
        std::cout << a.first<<",";
    }
    std::cout << "Graham Scan points: " << indices.size() << std::endl;

    unsigned int nPoints  = indices.size();
    m_indicesCH.reserve(nPoints);
    unsigned int lastIdx  = indices[0].first;
    unsigned int firstIdx = indices[1].first;
    m_indicesCH.push_back( lastIdx  );
    m_indicesCH.push_back( firstIdx );

    unsigned int lPtIdx  = lastIdx;
    unsigned int mPtIdx  = firstIdx;
    unsigned int currIdx ;
    unsigned int i = 2; // current point;

    // skip the first non left turns in the sequence!
    std::cout << "lastIdx point: " <<lastIdx << ","<< m_p.col(lastIdx).transpose() << std::endl;
    std::cout << "firstIdx point: " <<firstIdx << ","<< m_p.col(firstIdx).transpose() << std::endl;

    while( i<nPoints){
            currIdx = indices[i].first;
            if(leftTurn(m_p.col(lastIdx), m_p.col(firstIdx), m_p.col(currIdx))){
                break;
            }
            ++i;
    };


    std::cout << "i:"<< i << std::endl;
    std::cout << "currIdx point: " <<currIdx << std::endl;
    std::cout << ","<< m_p.col(currIdx).transpose() << std::endl << "===="<<std::endl;
    std::cout << "0,5,8: :" << orient2d(m_p.col(0), m_p.col(5), m_p.col(8)) << std::endl;
    std::cout << "1,5,8: :" << orient2d(m_p.col(1), m_p.col(5), m_p.col(8)) << std::endl;
    std::cout << "5,8,0: :" << orient2d(m_p.col(5), m_p.col(8), m_p.col(0)) << std::endl;

    if ( i < nPoints )
    {
      m_indicesCH.push_back( currIdx );
      decltype(m_indicesCH.rbegin()) revIter;
      lPtIdx  = mPtIdx;
      mPtIdx  = currIdx;

      for (++i ; i < nPoints; ++i )
      {
          currIdx = indices[i].first;

          if ( leftTurn(m_p.col(mPtIdx), m_p.col(currIdx), m_p.col(lastIdx)) )
          {

              while ( !leftTurn(m_p.col(lPtIdx), m_p.col(mPtIdx), m_p.col(currIdx))   )
              {
                  std::cout << "right turn: " <<lPtIdx << ","<< mPtIdx << "," << currIdx << std::endl;
                  //ApproxMVBB_ASSERTMSG(m_indicesCH.size()>2,"");
                  m_indicesCH.pop_back();

                  if( m_indicesCH.size() <= 1) { // Degenerate Case if we come back to the beginning
                    m_indicesCH.push_back(currIdx);
                    mPtIdx  = currIdx;
                    break;
                  }else{
                      mPtIdx = lPtIdx;                      // middle point becomes last
                      revIter = m_indicesCH.rbegin();
                      lPtIdx = *(++revIter);                // previous of .back();

                  }

              }
              std::cout << "left turn: " <<lPtIdx << ","<< mPtIdx << "," << currIdx << std::endl;
              m_indicesCH.push_back( currIdx );
              lPtIdx  = mPtIdx;          // last becomes middle
              mPtIdx  = currIdx;         // middle becomes currIdx
          }else{
              std::cout << "skip point: " << currIdx << std::endl;
          }
      }

    }
}


bool ConvexHull2D::verifyHull() {
    using namespace PointFunctions;
    if(m_indicesCH.size()>2) {
        unsigned int i = 2;
        while( i<m_indicesCH.size()) {
            if( ! (orient2d( m_p.col(m_indicesCH[i-2]), m_p.col(m_indicesCH[i-1]), m_p.col(m_indicesCH[i])) >= 0) ) { // if this is not a left turn
                return false;
            }
            ++i;
        }
    }
    return true;
}


};
