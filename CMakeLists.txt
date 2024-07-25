cmake_minimum_required(VERSION 3.12)
project(homed-angel)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PahoMqttCpp REQUIRED)
find_package(AngelScript REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Nova REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

option(CMAKE_BUILD_TYPE "Build type" Release)

message("Configuring as " ${CMAKE_BUILD_TYPE})
if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(CMAKE_VERBOSE_MAKEFILE ON)
  # set(SANITIZE_FLAG_DEBUG "-fsanitize=thread")
  # set(SANITIZE_FLAG_DEBUG "-fsanitize=address,leak,undefined")
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${SANITIZE_FLAG_DEBUG}")
  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${SANITIZE_FLAG_DEBUG}")
  add_definitions(-DHA_DEBUG)
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/version.txt" PACKAGE_VERSION)
string(STRIP "${PACKAGE_VERSION}" PACKAGE_VERSION)

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h"
  @ONLY
)

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
  src/utils/datetime.h
  src/utils/string.h
  src/homed/value/cobserver.h
  src/datetime/timers/ctimer.h
  src/datetime/timers/ctimercontinuos.h
  src/datetime/timers/ctimeroneshoot.h
  src/scripting/script/cscriptobject.h
  src/datetime/astronomical/csuntracker.h
  src/datetime/astronomical/ctracker.h
  src/datetime/astronomical/castronomical.h
  src/datetime/entities/cdatetime.h
  src/datetime/entities/ctimeinterval.h
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
  src/datetime/timers/ctimer.cpp
  src/datetime/timers/ctimercontinuos.cpp
  src/datetime/timers/ctimeroneshoot.cpp
  src/scripting/script/cscriptobject.cpp
  src/datetime/astronomical/csuntracker.cpp
  src/datetime/astronomical/ctracker.cpp
  src/datetime/astronomical/castronomical.cpp
  src/datetime/entities/cdatetime.cpp
  src/datetime/entities/ctimeinterval.cpp
  )

include_directories(src angelscript)

add_executable(homed-angel
    ${HA_HEADERS}
    ${HA_SOURCES}
)
target_link_libraries(homed-angel
    PahoMqttCpp::paho-mqttpp3
    jsoncpp
    ${ANGELSCRIPT_LIBRARY}
    ${NOVA_LIBRARIES}
    # ${Boost_LIBRARIES}
  )

install(TARGETS homed-angel
  DESTINATION /usr/bin)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/deploy/homed-angel.service
  DESTINATION /usr/share/homed-angel)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/homed-angel.example.json
  DESTINATION /usr/share/homed-angel)
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/scripts/devices_list.as
  DESTINATION /var/lib/homed-angel/scripts)

message("Arch is " ${CMAKE_SYSTEM_PROCESSOR})

set(CPACK_PACKAGE_NAME "homed-angel")
set(CPACK_PACKAGE_VERSION "${PACKAGE_VERSION}")
set(CPACK_PACKAGE_CONTACT "Gorlov Maxim <sheridan@babylon-five.ru>")
set(CPACK_PACKAGE_DESCRIPTION "HOMEd+AngelScript automatisation scripting.")
set(CPACK_PACKAGE_MAINTAINER "Gorlov Maxim <sheridan@babylon-five.ru>")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libpaho-mqttpp3, libjsoncpp25, libangelscript2.35.1, libnova-0.16-0")
set(CPACK_DEBIAN_PACKAGE_USER "homed")
set(CPACK_DEBIAN_PACKAGE_GROUP "homed")
set(CPACK_DEBIAN_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION}_${CMAKE_SYSTEM_PROCESSOR}.deb")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/deploy/debian/control.extra")
set(CPACK_DEBIAN_PACKAGE_POST_INSTALL "${CMAKE_CURRENT_SOURCE_DIR}/deploy/debian//post_install.sh")


include(CPack)
