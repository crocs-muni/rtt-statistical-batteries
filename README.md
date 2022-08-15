# RTT Statistical Batteries

Set of statistical batteries that are used by [Randomness Testing Toolkit](https://github.com/crocs-muni/randomness-testing-toolkit).

It is highly recommended to use this set as the batteries are modified to be fully compatible with the interface employed by RTT.

**Following batteries are included:**

* **NIST Statistical Test Suite** [Official page](http://csrc.nist.gov/groups/ST/toolkit/rng/documentation_software.html)  
Official implementation is not used due to being too slow. Instead optimized version is used: [NIST-STS-optimised](https://github.com/sysox/NIST-STS-optimised).
* **Dieharder: A Random Number Test Suite** [Official page](http://www.phy.duke.edu/~rgb/General/dieharder.php)  
Slightly modified version of Dieharder 3.31.1 is used. Modifications affect only battery output format, not the results itself.
* **TestU01** [Official page](http://simul.iro.umontreal.ca/testu01/tu01.html)  
This battery is shipped only as a library, we have custom command-line interface implemented.
* **BSI** Implementation was extracted from ParanoYa application (created at Faculty of Electrical Engineering and Information Technology, STU in Bratislava). Battery is not used by [RTT](https://github.com/crocs-muni/randomness-testing-toolkit).
* **FIPS** [Source code](https://salsa.debian.org/hmh/rng-tools/-/blob/master/fips.c) Battery is not used by [RTT](https://github.com/crocs-muni/randomness-testing-toolkit).

## Installation
Build process was tested on Debian 8. All used tools should be also available on other Linux distributions.

### Quick build

    wget https://github.com/crocs-muni/rtt-statistical-batteries/archive/master.zip
    unzip master.zip
    cd rtt-statistical-batteries-master
    sudo ./INSTALL

### Prerequisites
All required packages are installed in script INSTALL. If you want to manually install them you will need following packages (or their equivalents): **libgsl0-dev**, **build-essential**, **autotools-dev**, **automake**, **libtool**.

For inspecting on how the batteries are built, see inside of the script **INSTALL**.
