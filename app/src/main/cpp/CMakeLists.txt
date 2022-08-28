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
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0028 NEW)
include(ExternalProject)
project(reactivex VERSION 1.0.0 LANGUAGES CXX)
## Libraries Section ##
# INSTALL DIR #
SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE PATH <comment> FORCE)
# JSON CPP #
ExternalProject_Add(nlohmann
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
set (NLOHMANN_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
# YAML CPP #
ExternalProject_Add(yamlcpp
        DEPENDS nlohmann
        URL "https://github.com/jbeder/yaml-cpp/archive/refs/tags/yaml-cpp-0.7.0.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cppd.a
        CMAKE_ARGS -Wshadow
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
set (YAMLCPP_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cppd.a)

# ZLIB #
ExternalProject_Add(zlib
        DEPENDS yamlcpp
        URL "https://github.com/madler/zlib/archive/refs/tags/v1.2.12.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libz.a
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DBUILD_SHARED_LIBS=OFF
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
ExternalProject_Add(openssl
        DEPENDS zlib
        URL "https://github.com/mhalikhani94/openssl-cmake/archive/refs/tags/v0.0.1.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libssl_1_1.a
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libcrypto_1_1.a
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DWITH_APPS=OFF
        -DCMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}
        -DANDROID_ABI=${ANDROID_ABI}
        -DANDROID_NDK=${ANDROID_NDK}
        -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
        -DCMAKE_MAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DANDROID_ARM_NEON=${ANDROID_ARM_NEON}
        -DANDROID_TOOLCHAIN=${ANDROID_TOOLCHAIN}
)
set (OPENSSL_ROOT_DIR ${CMAKE_INSTALL_PREFIX}/ssl)
set (OPENSSL_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
set (OPENSSL_SSL_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libssl_1_1.a)
set (OPENSSL_CRYPTO_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libcrypto_1_1.a)
add_library(OpenSSL::SSL STATIC IMPORTED GLOBAL)
add_library(OpenSSL::Crypto STATIC IMPORTED GLOBAL)
set_target_properties(OpenSSL::SSL PROPERTIES IMPORTED_LOCATION ${OPENSSL_SSL_LIBRARIES})
set_target_properties(OpenSSL::Crypto PROPERTIES IMPORTED_LOCATION ${OPENSSL_CRYPTO_LIBRARIES})

set(HAVE_OPENSSL_CRYPTO_H TRUE CACHE INTERNAL "" FORCE)
set(HAVE_OPENSSL_ERR_H TRUE CACHE INTERNAL "" FORCE)
set(HAVE_OPENSSL_PEM_H TRUE CACHE INTERNAL "" FORCE)
set(HAVE_OPENSSL_RSA_H TRUE CACHE INTERNAL "" FORCE)
set(HAVE_OPENSSL_SSL_H TRUE CACHE INTERNAL "" FORCE)
set(HAVE_OPENSSL_X509_H TRUE CACHE INTERNAL "" FORCE)
set(HAVE_OPENSSL_RAND_H TRUE CACHE INTERNAL "" FORCE)
set(HAVE_RAND_STATUS TRUE CACHE INTERNAL "" FORCE)
set(HAVE_RAND_SCREEN FALSE CACHE INTERNAL "" FORCE)
set(HAVE_RAND_EGD FALSE CACHE INTERNAL "" FORCE)

# CURL # # Without ssl support #
ExternalProject_Add(curl
        DEPENDS openssl
        URL "https://github.com/mhalikhani94/curl/archive/refs/tags/v0.0.2.zip"
        BUILD_BYPRODUCTS ${CMAKE_INSTALL_PREFIX}/lib/libcurl-d.a
        CMAKE_ARGS
        -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
        -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
        -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
        -DCURL_USE_OPENSSL=ON
        -DCURL_STATICLIB=1
        -DBUILD_SHARED_LIBS=0
        -DCURL_USE_LIBPSL=OFF
        -DUSE_LIBIDN2=OFF
        -DCURL_USE_LIBSSH2=OFF
        -DCURL_DISABLE_LDAP=ON
        -DOPENSSL_ROOT_DIR=${OPENSSL_ROOT_DIR}
        -DOPENSSL_SSL_LIBRARIES=${OPENSSL_SSL_LIBRARIES}
        -DOPENSSL_CRYPTO_LIBRARIES=${OPENSSL_CRYPTO_LIBRARIES}
        -DOPENSSL_LIBRARIES=${OPENSSL_LIBRARIES}
        -DOPENSSL_INCLUDE_DIR=${OPENSSL_INCLUDE_DIR}
        -DOPENSSL_USE_STATIC_LIBS=TRUE
        -DBUILD_CURL_EXE=0
        -DBUILD_TESTING=OFF
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
set (CURL_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libcurl-d.a)


add_library(yamlcpp_lib STATIC IMPORTED GLOBAL)
add_dependencies(yamlcpp_lib yamlcpp)
set_target_properties(yamlcpp_lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libyaml-cppd.a)

add_library(zlib_lib STATIC IMPORTED GLOBAL)
add_dependencies(zlib_lib zlib)
set_target_properties(zlib_lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libz.a)

add_library(ssl_lib STATIC IMPORTED GLOBAL)
add_dependencies(ssl_lib openssl)
set_target_properties(ssl_lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libssl_1_1.a)

add_library(crypto_lib STATIC IMPORTED GLOBAL)
add_dependencies(crypto_lib openssl)
set_target_properties(crypto_lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libcrypto_1_1.a)

add_library(curl_lib STATIC IMPORTED GLOBAL)
add_dependencies(curl_lib curl)
set_target_properties(curl_lib PROPERTIES IMPORTED_LOCATION
        ${CMAKE_INSTALL_PREFIX}/lib/libcurl-d.a)


set(REACTIVEX_PROJECT_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include)
## Program Section ##
include_directories( ${PROJECT_SOURCE_DIR}/include )

add_library(reactivex
        SHARED
        native-lib.cpp rx-request-manager.cpp rx-request-manager.hpp rx-curl.hpp
)

add_dependencies(reactivex yamlcpp zlib openssl curl nlohmann)

find_library(log-lib
        log
)

target_include_directories(reactivex PUBLIC
        ${REACTIVEX_PROJECT_INCLUDE_DIR}
)

target_link_libraries(reactivex
        ${log-lib}
        curl_lib
        ssl_lib
        crypto_lib
        zlib_lib
        yamlcpp_lib
)
