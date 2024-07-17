# FindAngelScript.cmake
# Этот скрипт ищет библиотеку AngelScript

find_path(ANGELSCRIPT_INCLUDE_DIR
    NAMES angelscript.h
    PATH_SUFFIXES angelscript
)

find_library(ANGELSCRIPT_LIBRARY
    NAMES angelscript
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(AngelScript DEFAULT_MSG
    ANGELSCRIPT_INCLUDE_DIR
    ANGELSCRIPT_LIBRARY
)

mark_as_advanced(ANGELSCRIPT_INCLUDE_DIR ANGELSCRIPT_LIBRARY)
