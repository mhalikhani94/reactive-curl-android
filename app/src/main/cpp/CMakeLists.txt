#
# Copyright (C) 2019 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

## Project Section ##

cmake_minimum_required(VERSION 3.18.1)
include(ExternalProject)
project(reactivex VERSION 1.0.0 LANGUAGES CXX)
## Libraries Section ##
# INSTALL DIR #
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH <comment> FORCE)
ELSE()
    SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH <comment> FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
# JSON CPP #
ExternalProject_Add(jsoncpp
        URL "https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.zip"
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DJSON_BuildTests=OFF
        -DJSON_MultipleHeaders=OFF
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_NDK=${ANDROID_NDK}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}
        -DANDROID_TOOLCHAIN=${ANDROID_TOOLCHAIN}
        )
set (JSONCPP_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
# YAML CPP #
ExternalProject_Add(yamlcpp
        DEPENDS jsoncpp
        URL "https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-0.7.0.zip"
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DYAML_BUILD_SHARED_LIBS=OFF
        -DYAML_CPP_BUILD_TESTS=OFF
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_NDK=${ANDROID_NDK}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}
        -DANDROID_TOOLCHAIN=${ANDROID_TOOLCHAIN}
        )
set (YAMLCPP_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set (YAMLCPP_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cpp.a)

# ZLIB #
ExternalProject_Add(zlib
        DEPENDS yamlcpp
        URL "https://github.com/madler/zlib/archive/refs/tags/v1.2.12.zip"
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_NDK=${ANDROID_NDK}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}
        -DANDROID_TOOLCHAIN=${ANDROID_TOOLCHAIN}
)
set (ZLIB_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
if(WIN32)
    set (ZLIB_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libzlibstatic.a)
else()
    set (ZLIB_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libz.a)
endif(WIN32)
# OPENSSL # # Temporary disabled! #
#ExternalProject_Add(openssl
#        DEPENDS zlib
#        URL "https://github.com/janbar/openssl-cmake/archive/refs/tags/1.1.1n-20220327.tar.gz"
#        CMAKE_ARGS
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
#        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
#        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
#        -DWITH_APPS=OFF
#        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
#        -DANDROID_ABI=${ANDROID_ABI}
#        -DANDROID_NDK=${ANDROID_NDK}
#        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
#        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
#        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
#        -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}
#        -DANDROID_TOOLCHAIN=${ANDROID_TOOLCHAIN}
#)
#set (OPENSSL_ROOT_DIR ${CMAKE_INSTALL_PREFIX}/ssl)
#set (OPENSSL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
#set (OPENSSL_SSL_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libssl.a)
#set (OPENSSL_CRYPTO_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libcrypto.a)
# CURL # # Without ssl support #
ExternalProject_Add(curl
        DEPENDS zlib
        URL "https://github.com/curl/curl/releases/download/curl-7_84_0/curl-7.84.0.tar.gz"
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DCURL_USE_OPENSSL=OFF
        -DCURL_STATICLIB=1
        -DBUILD_SHARED_LIBS=0
        -DCURL_USE_LIBPSL=OFF
        -DUSE_LIBIDN2=OFF
        -DCURL_USE_LIBSSH2=OFF
        # -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
        # -DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
        # -DOPENSSL_USE_STATIC_LIBS=TRUE
        -DBUILD_CURL_EXE=0
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_NDK=${ANDROID_NDK}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}
        -DANDROID_TOOLCHAIN=${ANDROID_TOOLCHAIN}
)

set (CURL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set (CURL_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libcurl.a)


add_library(yamlcpp_lib STATIC IMPORTED)
add_library(zlib_lib STATIC IMPORTED)
add_library(curl_lib STATIC IMPORTED)

set_target_properties(yamlcpp_lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cppd.a)

set_target_properties(zlib_lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libz.a)

set_target_properties(curl_lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libcurl-d.a)

add_dependencies(curl_lib curl)
add_dependencies(zlib_lib zlib)
add_dependencies(yamlcpp_lib yamlcpp)

## Program Section ##
include_directories( C:/workspace/15-android-reactive-x/app/src/main/include/ )
#
add_library(reactivex
        SHARED
        native-lib.cpp rx-request-manager.cpp rx-request-manager.hpp rx-curl.hpp
)

find_library(log-lib
        log
)

target_include_directories(reactivex PUBLIC
        ${JSONCPP_INCLUDE_DIR}
        ${YAMLCPP_INCLUDE_DIR}
        ${CURL_INCLUDE_DIR}
)

add_dependencies(reactivex yamlcpp zlib curl)

target_link_libraries(reactivex INTERFACE
        ${log-lib}
        curl_lib
        zlib_lib
        yamlcpp_lib
)

