# Here should all build options be defined. Do not make a spaghetti from build system!
# Basic approach is to define a variable:
# set(VARIABLE_A VAL CACHE TYPE "Description")
# and then check for its value and do something about it
# if(cond VARIABLE_A)
#   define_set_do_some_stuff()
# endif()


set(NIST_STS_USE_FFTW ON CACHE BOOL "Accelerate DFT test with help of fftw3 library")

if(NIST_STS_USE_FFTW)
  add_definitions(-DNIST_STS_USE_FFTW)
endif()
