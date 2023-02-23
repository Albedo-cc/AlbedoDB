if (WIN32)
    target_include_directories(MySQL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/MySQL-8.0/include)
    set_property(TARGET MySQL
        PROPERTY 
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/MySQL-8.0/WIN32/libmysql.dll)
    set_property(TARGET MySQL
        PROPERTY
        IMPORTED_IMPLIB ${CMAKE_CURRENT_SOURCE_DIR}/MySQL-8.0/WIN32/libmysql.lib)
else()
    message(FATAL_ERROR "Sorry, only support MySQL-WIN32 now!")
endif()

file(GLOB BACKEND_FILES
    "albedodb_backend.h"
    "albedodb_backend.cc")

set(ALBEDODB_BACKEND_FILES ${BACKEND_FILES} PARENT_SCOPE)
set(ENV{ALBEDODB_BACKEND_INCLUDE} ${CMAKE_CURRENT_SOURCE_DIR})

function(AlbedoDB_load_backend)
    target_link_libraries(${PROJECT_NAME} PUBLIC MySQL)
    target_include_directories(${PROJECT_NAME} PUBLIC $ENV{ALBEDODB_BACKEND_INCLUDE})
endfunction(AlbedoDB_load_backend)