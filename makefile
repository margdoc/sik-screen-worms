CXX = gcc
RM = rm -f
MKDIR_P = mkdir -p
CPPFLAGS = -Wall -Wextra -std=c++17 -O2
LDFLAGS = -lstdc++ -lm
SERVER_TARGET = screen-worms-server
CLIENT_TARGET = screen-worms-client
BUILD_DIR = build
COMMON_DIRS = Common
SERVER_DIRS = Server
CLIENT_DIRS = Client

SERVER_SRCS := $(shell find $(SERVER_DIRS) -name *.cpp)
SERVER_OBJS := $(SERVER_SRCS:%=$(BUILD_DIR)/%.o)

CLIENT_SRCS := $(shell find $(CLIENT_DIRS) -name *.cpp)
CLIENT_OBJS := $(CLIENT_SRCS:%=$(BUILD_DIR)/%.o)

COMMON_SRCS := $(shell find $(COMMON_DIRS) -name *.cpp)
COMMON_OBJS := $(COMMON_SRCS:%=$(BUILD_DIR)/%.o)

.PHONY: all clean

all: server client


client: $(CLIENT_OBJS) ${COMMON_OBJS}
	$(CXX) $(CPPFLAGS) $(CLIENT_OBJS) ${COMMON_OBJS} -o $(CLIENT_TARGET) $(LDFLAGS)


server: $(SERVER_OBJS) ${COMMON_OBJS}
	$(CXX) $(CPPFLAGS) $(SERVER_OBJS) ${COMMON_OBJS} -o $(SERVER_TARGET) $(LDFLAGS)

$(BUILD_DIR)/%.cpp.o: %.cpp directories
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

directories:
	$(MKDIR_P) $(dir ./$(BUILD_DIR)/)
	$(MKDIR_P) $(dir ./$(BUILD_DIR)/$(COMMON_DIRS)/)
	$(MKDIR_P) $(dir ./$(BUILD_DIR)/$(SERVER_DIRS)/)
	$(MKDIR_P) $(dir ./$(BUILD_DIR)/$(CLIENT_DIRS)/)

clean:
	$(RM) -r $(BUILD_DIR)
	$(RM) $(CLIENT_TARGET)
	$(RM) $(SERVER_TARGET)
