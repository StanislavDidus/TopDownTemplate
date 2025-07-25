cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)

# Збираємо всі .cpp
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/*.cpp")

# Вказуємо include шляхи
include_directories(
  "${CMAKE_SOURCE_DIR}/SDL2/lib/SDL2-2.0.3/include"
  "${CMAKE_SOURCE_DIR}/SDL2/lib/FreeImage/inc"
)

# Додаємо імпортовані бібліотеки вручну
add_library(SDL2 STATIC IMPORTED)
set_target_properties(SDL2 PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/SDL2/lib/SDL2-2.0.3/lib/x64/SDL2.lib"
)

add_library(SDL2main STATIC IMPORTED)
set_target_properties(SDL2main PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/SDL2/lib/SDL2-2.0.3/lib/x64/SDL2main.lib"
)

add_library(FreeImage STATIC IMPORTED)
set_target_properties(FreeImage PROPERTIES
  IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/SDL2/lib/FreeImage/lib64/FreeImage.lib"
)

# Створюємо виконуваний файл
add_executable(MyProject ${SOURCES})

# Лінкуємо всі бібліотеки
target_link_libraries(MyProject SDL2 SDL2main FreeImage)
