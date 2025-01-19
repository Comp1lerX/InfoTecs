# Компилятор и флаги
CXX = g++
CXXFLAGS = -std=c++17 -fPIC -Wall -Wextra -pthread
LDFLAGS = -shared

# Названия файлов
LIB_NAME = liblogger.so
APP_NAME = app
LIB_SRC = liblogger.cpp
APP_SRC = main.cpp
LIB_HEADERS = liblogger.h

# Директория для копирования библиотеки
LIB_DEST_DIR = ./lib

# Цели сборки
all: $(LIB_NAME) $(APP_NAME) copy-lib update-bashrc

# Сборка динамической библиотеки
$(LIB_NAME): $(LIB_SRC) $(LIB_HEADERS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(LIB_SRC)

# Сборка консольного приложения
$(APP_NAME): $(APP_SRC) $(LIB_NAME)
	$(CXX) $(CXXFLAGS) -o $@ $(APP_SRC) -L. -llogger

copy-lib: $(LIB_NAME)
	mkdir -p $(LIB_DEST_DIR)
	cp $(LIB_NAME) $(LIB_DEST_DIR)

update-bashrc:
	@if ! grep -q "export LD_LIBRARY_PATH=.*$(LIB_DEST_DIR)" ~/.bashrc; then \
		echo "export LD_LIBRARY_PATH=$(LIB_DEST_DIR):$$LD_LIBRARY_PATH" >> ~/.bashrc; \
		echo "Добавлено в ~/.bashrc: export LD_LIBRARY_PATH=$(LIB_DEST_DIR):\$$LD_LIBRARY_PATH"; \
	else \
		echo "LD_LIBRARY_PATH уже содержит $(LIB_DEST_DIR)"; \
	fi

# Удаление скомпилированных файлов
clean:
	rm -f $(LIB_NAME) $(APP_NAME)
	rm -rf $(LIB_DEST_DIR)
