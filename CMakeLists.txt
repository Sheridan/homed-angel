cmake_minimum_required(VERSION 3.12)
project(homed_angel)

# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find_package(Boost 1.74 REQUIRED COMPONENTS system)
find_package(PahoMqttCpp REQUIRED)
find_package(AngelScript REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)
# pkg_check_modules(ANGELSCRIPT REQUIRED angelscript)

option(CMAKE_BUILD_TYPE "Build type" Release)

message("Configuring as " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(CMAKE_VERBOSE_MAKEFILE ON)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  add_definitions(-DHA_DEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

set(HA_HEADERS
  angelscript/sdk/add_on/scriptarray/scriptarray.h
  angelscript/sdk/add_on/scriptbuilder/scriptbuilder.h
  angelscript/sdk/add_on/scriptstdstring/scriptstdstring.h
  src/configuration/ccmdoptions.h
  src/configuration/cconfiguration.h
  src/defines/class_helpers.h
  src/homed/base/cbase.h
  src/homed/base/ccontainer.h
  src/homed/chomed.h
  src/homed/model/containers/cdevices.h
  src/homed/model/containers/cendpoints.h
  src/homed/model/containers/cproperties.h
  src/homed/model/entities/cdevice.h
  src/homed/model/entities/cendpoint.h
  src/homed/model/entities/cproperty.h
  src/homed/value/ccolor.h
  src/homed/value/cstorage.h
  src/homed/value/cvalue.h
  src/log.h
  src/main.h
  src/mqtt/cdevicename.h
  src/mqtt/cmqttcallback.h
  src/mqtt/cmqttclient.h
  src/mqtt/cpayload.h
  src/mqtt/ctopic.h
  src/scripting/cangel.h
  src/scripting/fs/cfilewatcher.h
  src/scripting/helpers/clogger.h
  src/scripting/helpers/cstrings.h
  src/scripting/script/cscript.h
  src/scripting/script/cscriptenvironment.h
  src/scripting/script/cscriptsmanager.h
  src/st.h
  src/tools/singletone/csingletone.h
  src/tools/singletone/singletone.h
  src/utils/datetime.h
  src/utils/string.h
  src/homed/value/cobserver.h
  )

set(HA_SOURCES
  angelscript/sdk/add_on/scriptarray/scriptarray.cpp
  angelscript/sdk/add_on/scriptbuilder/scriptbuilder.cpp
  angelscript/sdk/add_on/scriptstdstring/scriptstdstring_utils.cpp
  angelscript/sdk/add_on/scriptstdstring/scriptstdstring.cpp
  src/configuration/ccmdoptions.cpp
  src/configuration/cconfiguration.cpp
  src/homed/base/cbase.cpp
  src/homed/base/ccontainer.cpp
  src/homed/chomed.cpp
  src/homed/model/containers/cdevices.cpp
  src/homed/model/containers/cendpoints.cpp
  src/homed/model/containers/cproperties.cpp
  src/homed/model/entities/cdevice.cpp
  src/homed/model/entities/cendpoint.cpp
  src/homed/model/entities/cproperty.cpp
  src/homed/value/ccolor.cpp
  src/homed/value/cstorage.cpp
  src/homed/value/cvalue.cpp
  src/main.cpp
  src/mqtt/cdevicename.cpp
  src/mqtt/cmqttcallback.cpp
  src/mqtt/cmqttclient.cpp
  src/mqtt/cpayload.cpp
  src/mqtt/ctopic.cpp
  src/scripting/cangel.cpp
  src/scripting/fs/cfilewatcher.cpp
  src/scripting/helpers/clogger.cpp
  src/scripting/helpers/cstrings.cpp
  src/scripting/script/cscript.cpp
  src/scripting/script/cscriptenvironment.cpp
  src/scripting/script/cscriptsmanager.cpp
  src/tools/singletone/csingletone.cpp
  src/utils/datetime.cpp
  src/utils/string.cpp
  src/homed/value/cobserver.cpp
  )

include_directories(src angelscript)

add_executable(homed_angel
    ${HA_HEADERS}
    ${HA_SOURCES}
)
target_link_libraries(homed_angel
    PahoMqttCpp::paho-mqttpp3
    jsoncpp
    ${ANGELSCRIPT_LIBRARY}
    # ${Boost_LIBRARIES}
  )

  install(TARGETS homed-angel DESTINATION /usr/bin)

set(CPACK_PACKAGE_NAME "homed_angel")
set(CPACK_PACKAGE_VERSION "0.8.0")
set(CPACK_PACKAGE_CONTACT "Gorlov Maxim <sheridan@babylo-five.ru>")
set(CPACK_PACKAGE_DESCRIPTION "HomeD Exporter is a tool to export HOMEd Zigbee data metrics.")
set(CPACK_PACKAGE_MAINTAINER "Gorlov Maxim <sheridan@babylo-five.ru>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpaho-mqttpp3, libboost-system, libjsoncpp1")

include(CPack)
