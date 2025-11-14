cmake_minimum_required(VERSION 3.10)

project(TrafficSimulation)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Flags tối ưu hóa
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -march=native")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")

# Đường dẫn SFML
set(SFML_ROOT "D:/SFML-3.0.2")

file(GLOB SOURCE_FILES "src/*.cpp")

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES} 
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${SFML_ROOT}/include"
    "include"
)

# Preprocessor definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    SFML_STATIC
)

# Liên kết thư viện
target_link_libraries(${PROJECT_NAME} PRIVATE 
    # SFML Graphics
    debug "${SFML_ROOT}/lib/libsfml-graphics-d.a"
    optimized "${SFML_ROOT}/lib/libsfml-graphics.a"
    
    # SFML Window
    debug "${SFML_ROOT}/lib/libsfml-window-d.a"
    optimized "${SFML_ROOT}/lib/libsfml-window.a"
    
    # SFML System
    debug "${SFML_ROOT}/lib/libsfml-system-d.a"
    optimized "${SFML_ROOT}/lib/libsfml-system.a"
)

# Thư viện hệ thống cho Windows
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        opengl32
        winmm
        gdi32
    )
endif()

# Sao chép assets
if(WIN32)
    set(ASSETS_DIR "${CMAKE_SOURCE_DIR}/assets")
    
    add_custom_command(
        TARGET ${PROJECT_NAME} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            "${ASSETS_DIR}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
        COMMENT "Đang sao chép thư mục 'assets'..."
    )
endif()

# Sao chép DLLs
if(WIN32)
    set(DEBUG_DLLS
        "${SFML_ROOT}/bin/sfml-graphics-d-3.dll"
        "${SFML_ROOT}/bin/sfml-window-d-3.dll"
        "${SFML_ROOT}/bin/sfml-system-d-3.dll"
    )
    
    set(RELEASE_DLLS
        "${SFML_ROOT}/bin/sfml-graphics-3.dll"
        "${SFML_ROOT}/bin/sfml-window-3.dll"
        "${SFML_ROOT}/bin/sfml-system-3.dll"
    )

    add_custom_command(
        TARGET ${PROJECT_NAME} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DEBUG_DLLS} "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Đang sao chép Debug SFML DLLs..."
        CONFIGURATIONS Debug
    )

    add_custom_command(
        TARGET ${PROJECT_NAME} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RELEASE_DLLS} "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Đang sao chép Release SFML DLLs..."
        CONFIGURATIONS Release
    )
endif()

# Sao chép MinGW DLLs
if(WIN32 AND CMAKE_COMPILER_IS_GNUCXX) 
    get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
    message(STATUS "Đang tìm MinGW DLLs trong: ${MINGW_BIN_DIR}")

    set(MINGW_DLLS
        "${MINGW_BIN_DIR}/libstdc++-6.dll"
        "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
        "${MINGW_BIN_DIR}/libwinpthread-1.dll"
    )

    add_custom_command(
        TARGET ${PROJECT_NAME} 
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${MINGW_DLLS}
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
        COMMENT "Đang sao chép MinGW runtime DLLs..."
    )
endif()