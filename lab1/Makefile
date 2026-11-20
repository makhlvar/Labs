# ПЕРЕМЕННЫЕ
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -Iinclude
TARGET = app

# Автоматически находим все .cpp в src/ (аналог того, что делает CMake)
SRCS = $(wildcard src/*.cpp)
# Формируем список .o файлов, которые будут лежать в папке build/
OBJS = $(patsubst src/%.cpp, build/%.o, $(SRCS))

# ЦЕЛИ
# Цель all выполняется по умолчанию
all: $(TARGET)

# Цель сборки бинарника: зависит от всех объектных файлов
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# Правило компиляции .cpp в .o (создает папку build, если ее нет)
build/%.o: src/%.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -rf build $(TARGET)

# Флаг, чтобы make не искал файлы с именами all, clean и т.д.
.PHONY: all clean