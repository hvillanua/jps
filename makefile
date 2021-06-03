CC := g++
APP := jps

SOURCE_DIR := backend/src
OBJECT_DIR := obj


COMMON_FLAGS = -Wall -std=c++17 -lpthread -lboost_system -lz
COMPILE_FLAGS := -g
LINK_FLAGS := -g

C_SOURCES := $(wildcard $(SOURCE_DIR)/*.cpp)
C_HEADERS := $(wildcard $(SOURCE_DIR)/*.hpp)

C_OBJECTS := $(patsubst $(SOURCE_DIR)/%,${OBJECT_DIR}/%, ${C_SOURCES:.cpp=.o})

.PHONY: all clean
.SILENT: clean


all: build


# Final link
build: $(C_OBJECTS)
	@echo -e ""
	@echo -e "\033[1m\033[92m### Linking application\033[0m"
	$(CC) $(C_OBJECTS) -o $(APP) $(COMMON_FLAGS) $(LINK_FLAGS)
	@echo -e ""
	@echo -e ""


# Compile
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp ${C_HEADERS}
	@echo -e "\033[92m### Compiling $<\033[0m"
	@mkdir -p $(dir $@)
	$(CC) $(COMMON_FLAGS) $(COMPILE_FLAGS) $< -c -o $@


clean:
	@rm -rf $(OBJECT_DIR)
	@rm $(APP)
	@echo -e "\033[1m\033[93m### Project cleaned $<\033[0m"
	@echo -e ""
