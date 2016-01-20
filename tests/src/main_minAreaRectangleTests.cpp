// ========================================================================================
//  ApproxMVBB
//  Copyright (C) 2014 by Gabriel Nützi <nuetzig (at) imes (d0t) mavt (d0t) ethz (døt) ch>
//
//  This Source Code Form is subject to the terms of the Mozilla Public
//  License, v. 2.0. If a copy of the MPL was not distributed with this
//  file, You can obtain one at http://mozilla.org/MPL/2.0/.
// ========================================================================================

#include <iostream>



#include <fstream>
#include <set>

#include "TestConfig.hpp"

#include "ApproxMVBB/ComputeApproxMVBB.hpp"

#include "TestFunctions.hpp"
#include "CPUTimer.hpp"


namespace ApproxMVBB {

namespace MinAreaRectangleTest {

    std::string genTestName( std::string name){
        return "MinAreaRectangleTest-" + name;
    }

    template<typename T = PREC>
    std::string getPrecAbrev();

    template<>
    std::string getPrecAbrev<double>(){
        return "double";
    }
    template<>
    std::string getPrecAbrev<float>(){
        return "float";
    }

    std::string getFileInPath(std::string name){
        return ApproxMVBB_TESTS_INPUT_FILES_DIR "/" + name;
    }

    std::string getPointsDumpPath(std::string name,std::string suffix=".bin"){
        return ApproxMVBB_TESTS_OUTPUT_FILES_DIR "/" + genTestName(name)+"-"+getPrecAbrev()+suffix;
    }

    std::string getFileOutPath(std::string name, std::string suffix=".bin"){
        return ApproxMVBB_TESTS_OUTPUT_FILES_DIR "/" + genTestName(name) +"-"+getPrecAbrev()+"-"+"Out"+suffix;
    }
    std::string getFileValidationPath(std::string name, std::string suffix=".bin"){
        return ApproxMVBB_TESTS_VALIDATION_FILES_DIR "/" + getPrecAbrev()+"/"+genTestName(name) +"-"+getPrecAbrev()+"-"+"Out"+suffix;
    }

    template<typename TMatrix>
    void minRectTest(std::string name, const TMatrix & v) {
        using namespace PointFunctions;
        using namespace TestFunctions;

        setRandomSeedStd(name);

        dumpPointsMatrixBinary( getPointsDumpPath(name,".bin") ,v);
        dumpPointsMatrix( getPointsDumpPath(name,".txt"),v);

        std::cout << "\n\nStart MinAreaRectangle Test "+ name +"" << std::endl;
        START_TIMER(start)

        MinAreaRectangle c(v);
        c.compute();

        STOP_TIMER_SEC(count, start)
        std::cout << "Timings: " << count << " sec for " <<v.cols() << " points" << std::endl;
        std::cout << "End MinAreaRectangle Test "+ name +"" << std::endl;
        auto rect = c.getMinRectangle();

        Matrix2Dyn p(2,7);
        p.col(0) =  rect.m_p;
        p.col(1) =  rect.m_p + rect.m_u*rect.m_uL ;
        p.col(2) =  rect.m_p + rect.m_u*rect.m_uL + rect.m_v*rect.m_vL ;
        p.col(3) =  rect.m_p + rect.m_v*rect.m_vL ;
        p.col(4) =  rect.m_p;
        p.col(5) =  rect.m_u;
        p.col(6) =  rect.m_v;

        dumpPointsMatrixBinary(getFileOutPath(name) ,p);
        //dumpPointsMatrix(getFileOutPath(name,".txt"),p);

        // Compare with validation file
        TMatrix valid = p;
        valid.setConstant(std::numeric_limits<PREC>::signaling_NaN());
        readPointsMatrixBinary( getFileValidationPath(name) , valid);
        assertAlmostEqualArrays(p,valid);

    }

};
};

using namespace ApproxMVBB;
using namespace TestFunctions;
using namespace PointFunctions;
using namespace ApproxMVBB::MinAreaRectangleTest;

TEST(MinAreaRectangleTest, PointsRandom10) {
        static std::mt19937 rng(TestFunctions::randomSeed);
        static std::uniform_real_distribution<PREC> uni(0.0,1.0);
        auto f = [&](PREC) { return uni(rng); };
        // generate points
        Matrix2Dyn t(2,10);
        t = t.unaryExpr( f );
        minRectTest("PointsRandom10",t);
}

TEST(MinAreaRectangleTest, UnitRectangle) {


        Vector2List t;
        t.push_back(Vector2(0,0));
        t.push_back(Vector2(1,0));
        t.push_back(Vector2(1,1));
        t.push_back(Vector2(0,1));

        Matrix2Dyn v(2,t.size());
        for(unsigned int i = 0; i<t.size(); ++i) {
            v.col(i) = t[i];
        }
        minRectTest("UnitRectangle",v);
}


TEST(MinAreaRectangleTest, AlmostLine) {

        Vector2List v;
        v.push_back(Vector2(0,0));
        v.push_back(Vector2(1,1));
        v.push_back(Vector2(1,1+1e-13));
        v.push_back(Vector2(2,2));
        ApproxMVBB::Matrix2Dyn t(2,v.size());
        for(unsigned int i = 0; i<v.size(); ++i) {
            t.col(i) = v[i];
        }
        minRectTest("AlmostLine",t);
}

TEST(MinAreaRectangleTest, Line3) {

        Vector2List t;
        t.push_back(Vector2(0,0));
        t.push_back(Vector2(1,1));
        t.push_back(Vector2(2,2));

        Matrix2Dyn v(2,t.size());
        for(unsigned int i = 0; i<t.size(); ++i) {
            v.col(i) = t[i];
        }
        minRectTest("Line3",v);
}

TEST(MinAreaRectangleTest, Line2) {

        Vector2List t;
        t.push_back(Vector2(0,0));
        t.push_back(Vector2(1,1));

        Matrix2Dyn v(2,t.size());
        for(unsigned int i = 0; i<t.size(); ++i) {
            v.col(i) = t[i];
        }
        minRectTest("Line2",v);
}

TEST(MinAreaRectangleTest, Triangle) {

        Vector2List t;
        t.push_back(Vector2(0,0));
        t.push_back(Vector2(1,1));
        t.push_back(Vector2(1,-1));

        Matrix2Dyn v(2,t.size());
        for(unsigned int i = 0; i<t.size(); ++i) {
            v.col(i) = t[i];
        }
        minRectTest("Triangle",v);
}

TEST(MinAreaRectangleTest, Point) {

        Vector2List t;
        t.push_back(Vector2(1,0));

        Matrix2Dyn v(2,t.size());
        for(unsigned int i = 0; i<t.size(); ++i) {
            v.col(i) = t[i];
        }
        minRectTest("Point",v);
}

TEST(MinAreaRectangleTest, PointsOnCricle1000) {

        unsigned int max = 1000;
        ApproxMVBB::Matrix2Dyn t(2,max);
        for(unsigned int i=0; i<max; i++) {
            t.col(i) = Vector2(std::cos(0.0001/max * i) ,std::sin(0.0001/max * i) );
        }
        minRectTest("PointsOnCricle1000",t);
}

TEST(MinAreaRectangleTest, NoPoint) {
        Matrix2Dyn v(2,0);
        minRectTest("NoPoint",v);
}

TEST(MinAreaRectangleTest, Points2DRectFail) {

        auto t = getPointsFromFile2D(getFileInPath("PointsSimulation2DRectFail.txt"));
        ApproxMVBB::Matrix2Dyn v(2,t.size());
        for(unsigned int i = 0; i<t.size(); ++i) {
            v.col(i) = t[i];
        }
        minRectTest("Points2DRectFail",v);
}

#ifdef ApproxMVBB_TESTS_HIGH_PERFORMANCE
TEST(MinAreaRectangleTest, PointsRandom10M) {
        static std::mt19937 rng(TestFunctions::randomSeed);
        static std::uniform_real_distribution<PREC> uni(-1.0,1.0);
        auto f = [&](PREC) { return uni(rng); };
        // generate points
        ApproxMVBB::Matrix2Dyn t(2,10000000);
        t = t.unaryExpr( f );
        minRectTest("PointsRandom10M",t);
}
#endif

TEST(MinAreaRectangleTest, PointsBadProjection) {

        Matrix2Dyn t(2,400);
        readPointsMatrixBinary(getFileInPath("PointsBadProjection.bin"),t,false);
        minRectTest("PointsBadProjection",t);
}
TEST(MinAreaRectangleTest, PointsBadProjection2) {

        Matrix2Dyn t(2,400);
        readPointsMatrixBinary(getFileInPath("PointsBadProjection2.bin"),t,false);
        minRectTest("PointsBadProjection2",t);
}
TEST(MinAreaRectangleTest, PointsBadProjection3) {

        Matrix2Dyn t(2,400);
        readPointsMatrixBinary(getFileInPath("PointsBadProjection3.bin"),t,false);
        minRectTest("PointsBadProjection3",t);
}



int main(int argc, char **argv) {
        testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
}
