# NIST STS
See also `README.old.txt`


### required tools
- cmake
- conan
- C compiler


## Quick Start
If you have [Conan](https://conan.io/downloads.html) and [CMake](https://cmake.org/download/) already installed, just clone the project and:
```bash
mkdir <build_directory>
cd <build_directory>
conan install <path_to_project> --build missing --update
cmake <path_to_project>
cmake --build .
```


## Usage
```bash
assess <stream_length> [-fast]
```
\<stream_length> - the length of the individual bit stream(s) to be processed \
\[fast] - use the faster alternative implementation (v_2) of tests
(set Command arguments: 10000 -fast if you want to test 10 000 bits with faster version of NIST tests)
No or incorrect arguments lead to usage message.


### FFTW library used by default
By default, the fftw3 library is statically linked and the DiscreteFourierTransform test uses the library's functionality to deliver results faster. However, you can disable it by defining NIST_STS_USE_FFTW in cmake configuration stage like so `cmake -DNIST_STS_USE_FFTW=OFF`.


### Conan integration
This project uses [Conan packaging](https://docs.conan.io/) which significantly helps you to consume C++ dependencies faster. Installation is quite easy, just run `pip3 install conan`. \
You also have to prepare your build directory to benefit from conan integration. Please, read the build instructions in QuickStart section.
