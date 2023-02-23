cmake_minimum_required(VERSION 3.21.0)

if (NOT PROJECT_NAME)
    message(FATAL_ERROR "[Albedo DB] >> This library must have a parent project!")
else()
    set(ALBEDODB_PARENT_PROJECT ${PROJECT_NAME})
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