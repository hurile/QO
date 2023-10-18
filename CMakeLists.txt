# ---------------------------------------------------------------------------
# QO
# ---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.20)
project(tinydb)

# ---------------------------------------------------------------------------
# Environment
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------

set(THREADS_PREFER_PTHREAD_FLAG ON)
include("${PROJECT_SOURCE_DIR}/vendor/catch2.cmake")
include("${PROJECT_SOURCE_DIR}/vendor/uni.cmake")

# ---------------------------------------------------------------------------
# Includes and Sources
# ---------------------------------------------------------------------------

include("${PROJECT_SOURCE_DIR}/src/local.cmake")
include_directories(
    ${PROJECT_SOURCE_DIR}/src
)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------

include("${PROJECT_SOURCE_DIR}/tools/local.cmake")
include("${PROJECT_SOURCE_DIR}/tests/local.cmake")

# ---------------------------------------------------------------------------
# Configuration
# ---------------------------------------------------------------------------

enable_testing(true)
