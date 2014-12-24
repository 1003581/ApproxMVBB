MACRO(INCLUDE_ALL_ApproxMVBB_SOURCE 
      SRC INC INCLUDE_DIRS  # Names for the variables
      ApproxMVBB_ROOT_DIR ApproxMVBB_BINARY_DIR)  # Input strings


    # Add all external sources/headers
    INCLUDE(${ApproxMVBB_ROOT_DIR}/cmake/DefineApproxMVBBExternalSources.cmake)
    INCLUDE_DIAMETER_SOURCE(           ApproxMVBB_DIAM_SRC ApproxMVBB_DIAM_INC ApproxMVBB_DIAM_INC_DIRS 
        ${ApproxMVBB_ROOT_DIR}/external/Diameter ${ApproxMVBB_ROOT_DIR} ${ApproxMVBB_BINARY_DIR})
    
    INCLUDE_GEOMETRYPREDICATES_SOURCE( ApproxMVBB_GEOMPRED_SRC ApproxMVBB_GEOMPRED_INC ApproxMVBB_GEOMPRED_INC_DIRS 
        ${ApproxMVBB_ROOT_DIR}/external/GeometryPredicates ${ApproxMVBB_ROOT_DIR} ${ApproxMVBB_BINARY_DIR})


    SET(${SRC}
        ${ApproxMVBB_ROOT_DIR}/src/ApproxMVBB/Common/MyMatrixDefs.cpp
              
        ${ApproxMVBB_ROOT_DIR}/src/ApproxMVBB/ConvexHull2D.cpp           
        ${ApproxMVBB_ROOT_DIR}/src/ApproxMVBB/MinAreaRectangle.cpp  
        ${ApproxMVBB_ROOT_DIR}/src/ApproxMVBB/OOBB.cpp              
        ${ApproxMVBB_ROOT_DIR}/src/ApproxMVBB/AABB.cpp   
        
        ${ApproxMVBB_DIAM_SRC}
        ${ApproxMVBB_GEOMPRED_SRC}
    )

    SET(${INC}
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/Common/MyMatrixTypeDefs.hpp
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/Common/TypeDefs.hpp
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/Common/Exception.hpp
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/Common/StaticAssert.hpp
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/Common/AssertionDebug.hpp
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/Common/Platform.hpp
        
        
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/AABB.hpp  
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/AngleFunctions.hpp  
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/ComputeApproxMVBB.hpp 
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/ContainerFunctions.hpp         
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/ConvexHull2D.hpp       
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/GreatestCommonDivisor.hpp  
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/MakeCoordinateSystem.hpp     
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/MinAreaRectangle.hpp  
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/OOBB.hpp  
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/PointFunctions.hpp     
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/ProjectedPointSet.hpp
        ${ApproxMVBB_ROOT_DIR}/include/ApproxMVBB/TypeDefsPoints.hpp
        
        ${ApproxMVBB_DIAM_INC}
        ${ApproxMVBB_GEOMPRED_INC}
    )

    SET(${INCLUDE_DIRS}
        ${ApproxMVBB_GEOMPRED_INC_DIRS} 
        ${ApproxMVBB_DIAM_INC_DIRS}
        ${ApproxMVBB_ROOT_DIR}/include
        ${ApproxMVBB_BINARY_DIR}/include
    )
    

    # WRITE CONFIGURATION FILE
    INCLUDE(${ApproxMVBB_ROOT_DIR}/cmake/WriteConfigFile.cmake)
    SET(ApproxMVBB_CONFIG_FILE ${ApproxMVBB_BINARY_DIR}/include/ApproxMVBB/Config/Config.hpp) 
    ApproxMVBB_WRITE_CONFIG_FILE( ${ApproxMVBB_CONFIG_FILE} ${ApproxMVBB_ROOT_DIR})
    #=========================
    
    
    
ENDMACRO()
