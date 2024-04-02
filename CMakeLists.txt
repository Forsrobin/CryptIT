# ================================ #
# === Set up build requirements == #
# ================================ #
cmake_minimum_required(VERSION 3.28.1)
project(CryptIT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN On)

set(TARGET_NAME ${PROJECT_NAME})

if (BUILD_TESTS)
    enable_testing()
endif ()

if(BUILD_CUDA)
  enable_language(CUDA)
endif ()

# ================================ #
# === Find required modules   ==== #
# ================================ #
# Modules that need's to be in root (Local projects)
find_package(Qt6 REQUIRED)
if (NOT Qt6_FOUND)
  message(FATAL_ERROR "Qt6 not found")
endif (NOT Qt6_FOUND)

find_package(cryptopp REQUIRED)
if (NOT cryptopp_FOUND)
  message(FATAL_ERROR "CryptoPP not found")
endif (NOT cryptopp_FOUND)

find_package(GTest)
if (NOT GTest_FOUND)
  message(FATAL_ERROR "GTest not found")
endif (NOT GTest_FOUND)

include(CheckLanguage)
check_language(CUDA)

if(NOT DEFINED CMAKE_CUDA_STANDARD)
    set(CMAKE_CUDA_STANDARD 11)
    set(CMAKE_CUDA_STANDARD_REQUIRED ON)
endif()

# ================================ #
# === Create build targets ======= #
# ================================ #
add_subdirectory(src)
