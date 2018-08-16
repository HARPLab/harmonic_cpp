cmake_minimum_required(VERSION 2.8.3)

if(POLICY CMP0042)
  cmake_policy(SET CMP0042 NEW)
endif()

project(harmonic_cpp)

include(GNUInstallDirs)


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

# Find dependencies
find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem regex)
find_package(cnpy QUIET)

if (NOT cnpy_FOUND)
    message(STATUS "No builtin cnpy found, building from source")
    add_subdirectory(3rdparty/cnpy)
    set(cnpy_INCLUDE_DIRS 3rdparty/cnpy)
    set(cnpy_LIBRARIES cnpy-static)
    set_property(TARGET ${cnpy_LIBRARIES} PROPERTY POSITION_INDEPENDENT_CODE ON) 
endif()

find_package(yaml-cpp )
if (NOT YAML_CPP_FOUND)
#    message(STATUS "No builtin YAML_CPP found, building from source")
#    add_subdirectory(3rdparty/yaml-cpp)
#    set(YAML_CPP_LIBRARIES yaml-cpp)
#    set(YAML_CPP_INCLUDE_DIR 3rdparty/yaml-cpp/include)
#    set_property(TARGET ${YAML_CPP_LIBRARIES} PROPERTY POSITION_INDEPENDENT_CODE ON) 
endif()


# Set version info
set(harmonic_cpp_VERSION_MAJOR 0)
set(harmonic_cpp_VERSION_MINOR 1)
set(harmonic_cpp_VERSION ${harmonic_cpp_VERSION_MAJOR}.${harmonic_cpp_VERSION_MINOR})
set(harmonic_cpp_SOVERSION 0.1)

# Set up targets
include_directories(
    include
    ${cnpy_INCLUDE_DIRS}
    ${YAML_CPP_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
)

set(HDRS
    include/harmonic_cpp/accessors.hpp
    include/harmonic_cpp/morsel_info.hpp
    include/harmonic_cpp/robot_position.hpp
    include/harmonic_cpp/timed_video.hpp
    include/harmonic_cpp/utils.hpp
)

set(SRCS
   src/accessors.cpp
   src/morsel_info.cpp
   src/robot_position.cpp
   src/timed_video.cpp
   src/utils.cpp
)

add_library(harmonic_cpp SHARED
    ${HDRS}
    ${SRCS}
)
set_target_properties(harmonic_cpp PROPERTIES
    PUBLIC_HEADER "${HDRS}"
    VERSION "${harmonic_cpp_VERSION}"
    SOVERSION "${harmonic_cpp_SOVERSION}"
)
target_link_libraries(harmonic_cpp
    PRIVATE 
    cnpy-static
)
target_link_libraries(harmonic_cpp
    PUBLIC
    ${OpenCV_LIBRARIES} 
    ${Boost_LIBRARIES} 
    ${YAML_CPP_LIBRARIES}
)

# Create installation info
install(TARGETS harmonic_cpp
    EXPORT harmonic_cpp-targets
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/harmonic_cpp")
install(EXPORT harmonic_cpp-targets 
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/harmonic_cpp")


# Become findable by CMake
include(CMakePackageConfigHelpers)

configure_package_config_file(harmonic_cpp-config.cmake.in harmonic_cpp-config.cmake
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/harmonic_cpp"
  PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
  NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(harmonic_cpp-config-version.cmake
  VERSION ${harmonic_cpp_VERSION} COMPATIBILITY AnyNewerVersion)

install(FILES
  "${CMAKE_BINARY_DIR}/harmonic_cpp-config.cmake"
  "${CMAKE_BINARY_DIR}/harmonic_cpp-config-version.cmake"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/harmonic_cpp")
  
 option(harmonic_cpp_TESTS ON)
 if (harmonic_cpp_TESTS)
   add_executable(robot_position_test
       src/test/robot_position_test.cpp
   )
   target_link_libraries(robot_position_test
       harmonic_cpp)
       
   add_executable(timed_video_test
       src/test/timed_video_test.cpp
   )
   target_link_libraries(timed_video_test
       harmonic_cpp)
       
   add_executable(accessor_iterator_test
       src/test/accessor_iterator_test.cpp
   )
   target_link_libraries(accessor_iterator_test
       harmonic_cpp)
       
   add_executable(dataset_test
       src/test/dataset_test.cpp
   )
   target_link_libraries(dataset_test
       harmonic_cpp)
       
   add_executable(morsel_test
       src/test/morsel_test.cpp
   )
   target_link_libraries(morsel_test
       harmonic_cpp)
 endif()
 



