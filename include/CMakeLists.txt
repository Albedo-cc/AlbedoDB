# include <AlbedoLog.hpp>

# Load backend
if (backend_MySQL)
    message(NOTICE "[Albedo DB] >> Backend is MySQL")
    add_library(MySQL SHARED IMPORTED)
    add_subdirectory(backends/MySQL)
else()
    message(FATAL_ERROR "[Albedo DB] >> Unknown backend!")
endif()

# Create library
add_library(${PROJECT_NAME}
        STATIC
        AlbedoDB.hpp
        ${ALBEDODB_BACKEND_FILES})

AlbedoDB_load_backend()

# Copy DLL
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:MySQL>
    $<TARGET_FILE_DIR:${ALBEDODB_PARENT_PROJECT}>)

source_group("" FILES AlbedoDB.hpp)
source_group("backend" FILES ${ALBEDODB_BACKEND_FILES})
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})