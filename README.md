# Description

`harmonic_cpp` is a simple C++ library for accessing the HARMONIC dataset (<http://harp.ri.cmu.edu/harmonic>). It currently includes the following features:

* Iterating through the dataset and accessing particular runs based on desired filter criteria (in `accessors.hpp`)
* Loading in robot position information, including linear interpolation for unloaded times (`robot_position.hpp`)
* Loading timestamped videos and iterating through them (using OpenCV for video reading and cnpy to read the timestamps) (`timed_video.hpp`)
* Loading morsel YAML files (`morsel_info.hpp`)

# Installation

The package has been tested on Ubuntu 14.04 and 16.04 as well as MacOS Sierra.

## Dependencies
`harmonic_cpp` relies on the following packages:
* Boost (filesystem, regex)
* OpenCV (>=2.4)
* yaml-cpp

## Optional dependencies

We use the package <https://github.com/rogersce/cnpy> for accessing NumPy files. If not found, it is installed and built.

## Installation

First install the relevant dependencies

#### Ubuntu
```
sudo apt-get install libyaml-cpp-dev libboost-program-options-dev libboost-regex-dev libopencv-dev
```

#### Mac (Homebrew)
```
brew install boost opencv yaml-cpp
```

Now install the package itself

```
git clone git@github.com:HARPLab/harmonic_cpp.git
cd harmonic_cpp
git submodule init
mkdir build && cd build
cmake ..
make
```

The package includes support for CMake `find_package`. To include `harmonic_cpp` in your project, you can use CMake:

```
find_package(harmonic_cpp REQUIRED)
include_directories(${harmonic_cpp_INCLUDES})
target_link_library(my_target ${harmonic_cpp_LIBRARIES})
```

# Support

If you run into problems or have additional needs, please create an issue! 
