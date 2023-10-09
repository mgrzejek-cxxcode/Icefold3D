
set( IC3_MATH_SRC_FILES
        "Prerequisites.h"
        "Prerequisites.cpp"
        "ArrayOps.h"
        "ArrayOpsSIMD.inl"
        "Color.h"
        "Color.cpp"
        "Matrix.h"
        "MatrixOps.h"
        "MatrixOpsSIMD.inl"
        "MatrixUtils.h"
        "MatrixUtilsSIMD.inl"
        "Vector.h"
        "VectorOps.h"
        )

add_library( Ic3.Math STATIC
        ${IC3_MATH_SRC_FILES}
        )

set_target_properties( Ic3.Math PROPERTIES
        OUTPUT_NAME "Ic3.Math"
        )

target_include_directories( Ic3.Math PUBLIC
        ${IC3_INCLUDE_DIR}
        )

target_link_libraries( Ic3.Math PUBLIC
        ${IC3_LINK_INPUT}
        Ic3.Cppx
        )
