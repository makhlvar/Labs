# Минимальная версия CMake
cmake_minimum_required(VERSION 3.15)
project(NetpbmGenerator)
# стандарт C++ 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем флаги (как g++ -Wall -Wextra в лекции про компиляцию)
add_compile_options(-Wall -Wextra -Werror -pedantic)

# Говорим, где лежат заголовочные файлы (.hpp)
include_directories(include)

# Собираем все .cpp файлы из папки src в одну переменную SOURCES
file(GLOB SOURCES "src/*.cpp")

# Создаем исполняемый файл 
add_executable(NetpbmGenerator ${SOURCES})