# FindNova.cmake
cmake_minimum_required(VERSION 3.10)

find_path(NOVA_INCLUDE_DIR
  NAMES libnova.h
  PATHS /usr/include/libnova /usr/local/include/libnova
)

find_library(NOVA_LIBRARY
  NAMES libnova.so
  PATHS /usr/lib /usr/local/lib
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Nova DEFAULT_MSG NOVA_LIBRARY NOVA_INCLUDE_DIR)

if(NOVA_FOUND)
  set(NOVA_INCLUDE_DIRS ${NOVA_INCLUDE_DIR})
  set(NOVA_LIBRARIES ${NOVA_LIBRARY})
endif()
