
INC_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR	:= bin
INCLUDE := -I./$(INC_DIR)

$(BIN_DIR)/main : $(BUILD_DIR)/main.o $(BUILD_DIR)/Date.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o $(BUILD_DIR)/AgendaService.o  $(BUILD_DIR)/AgendaUI.o
	@mkdir -p $(BIN_DIR)
	g++ -std=c++11 -o $@

$(BUILD_DIR)/main.o : $(SRC_DIR)/main.cpp
	@mkdir -p $(BUILD_DIR) 
	g++ -std=c++11 -c $(SRC_DIR)/main.cpp 

$(BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	g++ -std=c++11 $(INCLUDE) -c -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)
