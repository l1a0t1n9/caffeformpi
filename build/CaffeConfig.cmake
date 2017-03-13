# Config file for the Caffe package.
#
# Note:
#   Caffe and this config file depends on opencv,
#   so put `find_package(OpenCV)` before searching Caffe
#   via `find_package(Caffe)`. All other lib/includes
#   dependencies are hard coded in the file
#
# After successful configuration the following variables
# will be defined:
#
#   Caffe_INCLUDE_DIRS - Caffe include directories
#   Caffe_LIBRARIES    - libraries to link against
#   Caffe_DEFINITIONS  - a list of definitions to pass to compiler
#
#   Caffe_HAVE_CUDA    - signals about CUDA support
#   Caffe_HAVE_CUDNN   - signals about cuDNN support


# OpenCV dependency

if(NOT OpenCV_FOUND)
  set(Caffe_OpenCV_CONFIG_PATH "/usr/share/OpenCV")
  if(Caffe_OpenCV_CONFIG_PATH)
    get_filename_component(Caffe_OpenCV_CONFIG_PATH ${Caffe_OpenCV_CONFIG_PATH} ABSOLUTE)

    if(EXISTS ${Caffe_OpenCV_CONFIG_PATH} AND NOT TARGET opencv_core)
      message(STATUS "Caffe: using OpenCV config from ${Caffe_OpenCV_CONFIG_PATH}")
      include(${Caffe_OpenCV_CONFIG_PATH}/OpenCVModules.cmake)
    endif()

  else()
    find_package(OpenCV REQUIRED)
  endif()
  unset(Caffe_OpenCV_CONFIG_PATH)
endif()

# Compute paths
get_filename_component(Caffe_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(Caffe_INCLUDE_DIRS "/ssd/lijingling/resnet_MSRA/caffe-mpi/src;/usr/include;/ssd/lijingling/resnet_MSRA/caffe-mpi/build/include;/usr/local/cuda/include;/usr/include/opencv;/usr/include/atlas;/usr/local/include;/usr/local/nccl/include;/ssd/lijingling/resnet_MSRA/caffe-mpi/include")



# Our library dependencies
if(NOT TARGET caffe AND NOT caffe_BINARY_DIR)
  include("${Caffe_CMAKE_DIR}/CaffeTargets.cmake")
endif()

# List of IMPORTED libs created by CaffeTargets.cmake
set(Caffe_LIBRARIES caffe)

# Definitions
set(Caffe_DEFINITIONS "")

# Cuda support variables
set(Caffe_CPU_ONLY OFF)
set(Caffe_HAVE_CUDA TRUE)
set(Caffe_HAVE_CUDNN TRUE)
