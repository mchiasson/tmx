cmake_minimum_required(VERSION 3.2)

##
# Hunter Cross-platform package manager
##
include("cmake/HunterGate.cmake")
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.22.19.tar.gz"
    SHA1 "54749ffc945f78362aa680a7abbafcaa40ff9069"
)

#-----------#
# Configure
#-----------#

project(tmx VERSION 1.0.0 LANGUAGES C)
set(API_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
set(BUILD_VERSION "${PROJECT_VERSION}")

option(BUILD_SHARED_LIBS "Build shared libraries (dll / so)" off)

#-----------#
#    Env
#-----------#

set(SOURCES
    "src/tmx.c"
    "src/tsx.c"
    "src/tmx_utils.c"
    "src/tmx_err.c"
    "src/tmx_xml.c"
    "src/tmx_mem.c"
    "src/tmx_hash.c")
set(HEADERS "src/tmx.h" "src/tsx.h")

include(CheckIncludeFiles)
CHECK_INCLUDE_FILES("stdint.h" STDINT_H)
if(NOT STDINT_H)
    message(FATAL_ERROR "error: required header stdint.h not found")
endif(NOT STDINT_H)

include(TestBigEndian)
TEST_BIG_ENDIAN(SYS_BE)
if(SYS_BE)
    add_definitions(-DSYS_BIG_ENDIAN)
endif(SYS_BE)

add_definitions(-DWANT_ZLIB)
hunter_add_package(ZLIB)
find_package(ZLIB CONFIG REQUIRED)

hunter_add_package(libxml2)
find_package(libxml2 CONFIG REQUIRED)

if(MSVC)
    # disable warning on _strncpy (spams the output)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

#-----------#
#   Build
#-----------#

list(APPEND targets tmx)

add_library(tmx STATIC ${SOURCES})
set_target_properties(tmx PROPERTIES VERSION ${BUILD_VERSION})
target_link_libraries(tmx PUBLIC ZLIB::zlib ${LIBXML2_LIBRARIES})

set_property(TARGET tmx
             APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
             $<INSTALL_INTERFACE:include>)

if(BUILD_SHARED_LIBS)
    list(APPEND targets tmx_shared)

    add_library(tmx_shared SHARED ${SOURCES})
    target_link_libraries(tmx_shared PRIVATE ZLIB::zlib libxml2::libxml2)

    if(MSVC OR CYGWIN OR MINGW)
        target_compile_definitions(tmx_shared
                                   PRIVATE   "TMXEXPORT=__declspec(dllexport)"
                                   INTERFACE "TMXEXPORT=__declspec(dllimport)")
    endif()

    set_target_properties(tmx_shared PROPERTIES OUTPUT_NAME tmx)
    set_target_properties(tmx_shared PROPERTIES SOVERSION ${API_VERSION})
    set_target_properties(tmx_shared PROPERTIES VERSION ${BUILD_VERSION})

    set_property(TARGET tmx_shared
                 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                 $<INSTALL_INTERFACE:include>)
endif(BUILD_SHARED_LIBS)

#-----------#
#  Install
#-----------#

set(config_install_dir "lib/cmake/${PROJECT_NAME}")
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

install(
    FILES
        src/tmx.h
        src/tmx_utils.h
        src/tsx.h
    DESTINATION
        include
)

install(TARGETS tmx
  EXPORT "${targets_export_name}"
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  FRAMEWORK DESTINATION Library/Frameworks
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${version_config}" COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)
