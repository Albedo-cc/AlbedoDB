cmake_minimum_required(VERSION 3.21.0)

if (NOT ALBEDO_LIBS_SHARED_TARGET)
    message(FATAL_ERROR "[Albedo DB] >> You have to set ALBEDO_LIBS_SHARED_TARGET, which is an executable object using this library!")
endif()

project("AlbedoDB"
    LANGUAGES CXX
    VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Explicitly require the standard
set(CMAKE_CXX_EXTENSIONS OFF) # Vendor-specific Extensions

# Choose backend
option(backend_MySQL "Set the MySQL as the backend of AlbedoDB" ON)

add_subdirectory(include)
add_library(Albedo::DB ALIAS ${PROJECT_NAME})