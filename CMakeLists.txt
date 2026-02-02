cmake_minimum_required(VERSION 3.10)
project("CHIP-8 EMULATOR"
					VERSION 0.1.0
					LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDART_REQUIRE ON)
set(CMAKE_C_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)

file(SOURCES "src/*.c")

add_executable(chip8 ${SOURCES})

target_link_libraries(chip8 PRIVATE SDL2::SDL2)

if (APPLE) 
    target_link_libraries(chip8 PRIVATE "-framework Cocoa" "-framework OpenGL")
endif()

add_custom_command(TARGET chip8 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/roms 
		$<TARGET_FILE_DIR:chip8>/roms
)
