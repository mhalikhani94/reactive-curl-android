# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.18.1)

# Declares and names the project.

project("reactivex")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        reactivex

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        native-lib.cpp rx-request-manager.cpp rx-request-manager.hpp rx-curl.hpp)

add_library(
        lib_yaml_cpp
        STATIC
        IMPORTED
)

add_library(
        lib_z
        STATIC
        IMPORTED
)


add_library(
        lib_openssl
        STATIC
        IMPORTED
)

add_library(
        lib_crypto
        STATIC
        IMPORTED
)

add_library(
        lib_curl
        STATIC
        IMPORTED
)

set_target_properties( # Specifies the target library.
        lib_yaml_cpp
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        C:/workspace/15-android-reactive-x/app/src/main/jniLibs/${ANDROID_ABI}/libyaml-cppd.a)

set_target_properties( # Specifies the target library.
        lib_z
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        C:/workspace/15-android-reactive-x/app/src/main/jniLibs/${ANDROID_ABI}/libz.a)

set_target_properties( # Specifies the target library.
        lib_openssl
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        C:/workspace/15-android-reactive-x/app/src/main/jniLibs/${ANDROID_ABI}/libssl_1_1.a)

set_target_properties( # Specifies the target library.
        lib_crypto
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        C:/workspace/15-android-reactive-x/app/src/main/jniLibs/${ANDROID_ABI}/libcrypto_1_1.a)

set_target_properties( # Specifies the target library.
        lib_curl
        # Specifies the parameter you want to define.
        PROPERTIES IMPORTED_LOCATION
        # Provides the path to the library you want to import.
        C:/workspace/15-android-reactive-x/app/src/main/jniLibs/${ANDROID_ABI}/libcurl-d.a)

include_directories( C:/workspace/15-android-reactive-x/app/src/main/include/ )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists beforeq
# completing its build.

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
        reactivex

        # Links the target library to the log library
        # included in the NDK.
        lib_curl lib_z lib_openssl lib_crypto lib_yaml_cpp  ${log-lib})