
set( CXM_SRC_FILES
        "prerequisites.h"
        "prerequisites.cpp"
        "arrayOps.h"
        "arrayOpsSIMD.inl"
        "color.h"
        "color.cpp"
        "matrix.h"
        "matrixOps.h"
        "matrixOpsSIMD.inl"
        "matrixUtils.h"
        "matrixUtilsSIMD.inl"
        "vector.h"
        "vectorOps.h"
)

add_library( ext-cxm STATIC ${CXM_SRC_FILES} )

set_property( TARGET ext-cxm PROPERTY CXX_STANDARD 17 )
target_compile_features( ext-cxm PUBLIC cxx_std_17 )

target_include_directories( ext-cxm PUBLIC
	"${EXT_INCLUDE_DIR}"
)
