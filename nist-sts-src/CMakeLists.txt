cmake_minimum_required(VERSION 3.10)

project(NIST_STS VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

include(cmake/options.cmake)

if(NIST_STS_USE_FFTW)
  include(cmake/add_fftw.cmake)
endif()

add_executable(assess
  src/BM.c
  src/BMA.c
  src/LUTs.c
  src/approximateEntropy.c
  src/assess.c
  src/blockFrequency.c
  src/cephes.c
  src/cusum.c
  src/dfft.c
  src/discreteFourierTransform.c
  src/erf.c
  src/frequency.c
  src/generators.c
  src/genutils.c
  src/linearComplexity.c
  src/longestRunOfOnes.c
  src/main.c
  src/matrix.c
  src/nonOverlappingTemplateMatchings.c
  src/overlappingTemplateMatchings.c
  src/randomExcursions.c
  src/randomExcursionsVariant.c
  src/rank.c
  src/runs.c
  src/serial.c
  src/tools.c
  src/universal.c
  src/utilities.c
)
target_include_directories(assess PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
target_link_libraries(assess PUBLIC
  $<$<BOOL:${NIST_STS_USE_FFTW}>:FFTW3::fftw3>
  $<$<NOT:$<BOOL:${WIN32}>>:m> # C math library
)
install(TARGETS assess
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
