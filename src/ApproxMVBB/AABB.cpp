// ========================================================================================
//  ApproxMVBB
//  Copyright (C) 2014 by Gabriel Nützi <nuetzig (at) imes (d0t) mavt (d0t) ethz (døt) ch>
//
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================================

#include "ApproxMVBB/Config/Config.hpp"

#include ApproxMVBB_AABB_INCLUDE_FILE
namespace ApproxMVBB{

void AABB::reset() {
    // Violating the constraint min<max for making a completey empty box!
    m_minPoint(0) = std::numeric_limits<PREC>::max();
    m_maxPoint(0) = std::numeric_limits<PREC>::lowest();
    m_minPoint(1) = std::numeric_limits<PREC>::max();
    m_maxPoint(1) = std::numeric_limits<PREC>::lowest();
    m_minPoint(2) = std::numeric_limits<PREC>::max();
    m_maxPoint(2) = std::numeric_limits<PREC>::lowest();
}


AABB::AABB( const Vector3 &p) {
    m_minPoint = p;
    m_maxPoint = p;
};
AABB::AABB( const Vector3 &l, const Vector3 &u) {
    m_minPoint = Vector3(std::min(l(0),u(0)),std::min(l(1),u(1)),std::min(l(2),u(2)));
    m_maxPoint = Vector3(std::max(l(0),u(0)),std::max(l(1),u(1)),std::max(l(2),u(2)));
};


AABB& AABB::unite(const AABB & box) {
    m_maxPoint(0) = std::max(m_maxPoint(0),box.m_maxPoint(0));
    m_maxPoint(1) = std::max(m_maxPoint(1),box.m_maxPoint(1));
    m_maxPoint(2) = std::max(m_maxPoint(2),box.m_maxPoint(2));
    m_minPoint(0) = std::min( m_minPoint(0),box.m_minPoint(0));
    m_minPoint(1) = std::min( m_minPoint(1),box.m_minPoint(1));
    m_minPoint(2) = std::min( m_minPoint(2),box.m_minPoint(2));
    return *this;
};

AABB AABB::operator+ (const Vector3 &p) {
    AABB a;
    a.m_maxPoint(0) = std::max(m_maxPoint(0),p(0));
    a.m_maxPoint(1) = std::max(m_maxPoint(1),p(1));
    a.m_maxPoint(2) = std::max(m_maxPoint(2),p(2));
    a. m_minPoint(0) = std::min( m_minPoint(0),p(0));
    a. m_minPoint(1) = std::min( m_minPoint(1),p(1));
    a. m_minPoint(2) = std::min( m_minPoint(2),p(2));
    return a;
};

AABB AABB::operator+ (const AABB & box) {
    AABB a;
    a.m_maxPoint(0) = std::max(m_maxPoint(0),box.m_maxPoint(0));
    a.m_maxPoint(1) = std::max(m_maxPoint(1),box.m_maxPoint(1));
    a.m_maxPoint(2) = std::max(m_maxPoint(2),box.m_maxPoint(2));
    a. m_minPoint(0) = std::min( m_minPoint(0),box. m_minPoint(0));
    a. m_minPoint(1) = std::min( m_minPoint(1),box. m_minPoint(1));
    a. m_minPoint(2) = std::min( m_minPoint(2),box. m_minPoint(2));
    return a;
};

AABB& AABB::operator+= (const AABB & box) {
    m_maxPoint(0) = std::max(m_maxPoint(0),box.m_maxPoint(0));
    m_maxPoint(1) = std::max(m_maxPoint(1),box.m_maxPoint(1));
    m_maxPoint(2) = std::max(m_maxPoint(2),box.m_maxPoint(2));
    m_minPoint(0) = std::min( m_minPoint(0),box. m_minPoint(0));
    m_minPoint(1) = std::min( m_minPoint(1),box. m_minPoint(1));
    m_minPoint(2) = std::min( m_minPoint(2),box. m_minPoint(2));
    return *this;
};

AABB&  AABB::transform(const AffineTrafo & M) {

    AABB ret( M*(Vector3( m_minPoint(0), m_minPoint(1), m_minPoint(2))));
    ret.unite(M*(Vector3( m_maxPoint(0), m_minPoint(1), m_minPoint(2))));
    ret.unite(M*(Vector3( m_minPoint(0), m_maxPoint(1), m_minPoint(2))));
    ret.unite(M*(Vector3( m_minPoint(0), m_minPoint(1), m_maxPoint(2))));
    ret.unite(M*(Vector3( m_minPoint(0), m_maxPoint(1), m_maxPoint(2))));
    ret.unite(M*(Vector3( m_maxPoint(0), m_maxPoint(1), m_minPoint(2))));
    ret.unite(M*(Vector3( m_maxPoint(0), m_minPoint(1), m_maxPoint(2))));
    ret.unite(M*(Vector3( m_maxPoint(0), m_maxPoint(1), m_maxPoint(2))));
    *this = ret;
    return *this;
};

void AABB::expandToMinExtentRelative(PREC p, PREC defaultExtent, PREC eps){
    Array3 e = extent();
    Vector3 c = center();
    Array3::Index idx;
    PREC ext = std::abs(e.maxCoeff(&idx)) * p;

    if( ext < eps ){ // extent of max axis almost zero, set all axis to defaultExtent --> cube
        ext = defaultExtent;
        for(int i=0;i<3;++i){
            m_minPoint(i) = c(i) - 0.5*ext;
            m_maxPoint(i) = c(i) + 0.5*ext;
        }
    }else{
        for(int i=0;i<3;++i){
            if(i!=idx && std::abs(e(i)) < ext){
                m_minPoint(i) = c(i) - 0.5*ext;
                m_maxPoint(i) = c(i) + 0.5*ext;
            }
        }
    }
}

void AABB::expandToMinExtentAbsolute(PREC minExtent){
    Array3 e = extent();
    Vector3 c = center();

    PREC l = 0.5*minExtent;
    for(int i=0;i<3;++i){
        if(std::abs(e(i)) < minExtent){
            m_minPoint(i) = c(i) - l;
            m_maxPoint(i) = c(i) + l;
        }
    }
}


void AABB::expandToMinExtentAbsolute(Array3 minExtent){
    Array3 e = extent();
    Vector3 c = center();

    for(int i=0;i<3;++i){
        PREC l = minExtent(i);
        if(std::abs(e(i)) < l){
            m_minPoint(i) = c(i) - 0.5*l;
            m_maxPoint(i) = c(i) + 0.5*l;
        }
    }
}


void AABB2d::reset() {
    // Violating the constraint min<max for making a completey empty box!
    m_minPoint(0) = std::numeric_limits<PREC>::max();
    m_maxPoint(0) = std::numeric_limits<PREC>::lowest();
    m_minPoint(1) = std::numeric_limits<PREC>::max();
    m_maxPoint(1) = std::numeric_limits<PREC>::lowest();
}

AABB2d::AABB2d( const Vector2 &p) {
    m_minPoint = p;
    m_maxPoint = p;
};
AABB2d::AABB2d( const Vector2 &l, const Vector2 &u) {
    m_minPoint = Vector2(std::min(l(0),u(0)),std::min(l(1),u(1)));
    m_maxPoint = Vector2(std::max(l(0),u(0)),std::max(l(1),u(1)));
};

AABB2d& AABB2d::unite(const AABB2d & box) {
    m_maxPoint(0) = std::max(m_maxPoint(0),box.m_maxPoint(0));
    m_maxPoint(1) = std::max(m_maxPoint(1),box.m_maxPoint(1));
    m_minPoint(0) = std::min( m_minPoint(0),box.m_minPoint(0));
    m_minPoint(1) = std::min( m_minPoint(1),box.m_minPoint(1));
    return *this;
};

AABB2d AABB2d::operator+ (const Vector2 &p) {
    AABB2d a;
    a.m_maxPoint(0) = std::max(m_maxPoint(0),p(0));
    a.m_maxPoint(1) = std::max(m_maxPoint(1),p(1));
    a. m_minPoint(0) = std::min( m_minPoint(0),p(0));
    a. m_minPoint(1) = std::min( m_minPoint(1),p(1));
    return a;
};

AABB2d AABB2d::operator+ (const AABB2d & box) {
    AABB2d a;
    a.m_maxPoint(0) = std::max(m_maxPoint(0),box.m_maxPoint(0));
    a.m_maxPoint(1) = std::max(m_maxPoint(1),box.m_maxPoint(1));
    a. m_minPoint(0) = std::min( m_minPoint(0),box. m_minPoint(0));
    a. m_minPoint(1) = std::min( m_minPoint(1),box. m_minPoint(1));
    return a;
};

AABB2d& AABB2d::operator+= (const AABB2d & box) {
    m_maxPoint(0) = std::max(m_maxPoint(0),box.m_maxPoint(0));
    m_maxPoint(1) = std::max(m_maxPoint(1),box.m_maxPoint(1));
    m_minPoint(0) = std::min( m_minPoint(0),box. m_minPoint(0));
    m_minPoint(1) = std::min( m_minPoint(1),box. m_minPoint(1));
    return *this;
};


AABB2d & AABB2d::transform(const AffineTrafo2d & M) {

    AABB2d ret( M*(Vector2( m_minPoint(0), m_minPoint(1))));
    ret.unite(M*(Vector2( m_maxPoint(0), m_minPoint(1))));
    ret.unite(M*(Vector2( m_minPoint(0), m_maxPoint(1))));
    ret.unite(M*(Vector2( m_maxPoint(0), m_maxPoint(1))));
    *this = ret;
    return *this;
};



};
