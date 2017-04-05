##############################################################################
# @file  CMakeLists.txt
# @brief Build configuration of MIRTK SliceDice tools.
##############################################################################

if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif ( CMAKE_COMPILER_IS_GNUCC )
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
endif ( MSVC )

mirtk_add_executable(
  evaluate-roc-data
  DEPENDS
    LibCommon
    LibNumerics
    LibImage
    LibIO
)
