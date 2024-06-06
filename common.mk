# chatgpt写的

# 定义编译器
CXX := g++-14
CC:= gcc-14

CXXFLAGS := -std=c++23 -msse4.1  -O3 -I../nlohmann -Wall -I../include
CFLAGS:= -std=c17  -O3  -Wall
# 定义目标文件存放目录
BIN_DIR:=../bin

# 默认目标
.DEFAULT_GOAL :=all

# 获取所有.cpp文件
CPP_FILES := $(wildcard *.cpp)

# 获取所有.cpp文件的目标文件
TARGETS := $(patsubst %.cpp,$(BIN_DIR)/%,$(CPP_FILES))

# 默认规则，生成所有目标文件
all: $(TARGETS)





# 防止与文件同名的目标文件被删除
.PHONY: clean all

# 根据命令行参数指定的文件进行单个文件编译 并执行
%: %.cpp
	$(CXX)  $(CXXFLAGS)  $< -o $(BIN_DIR)/$@ && $(BIN_DIR)/$@ 

%: %.c
	$(CC)  $(CFLAGS)  $< -o $(BIN_DIR)/$@ && $(BIN_DIR)/$@ 

# catch2 相关
CATCH:=catch-main
$(CATCH): $(BIN_DIR)/$(CATCH).o 
	echo ""

$(BIN_DIR)/$(CATCH).o: $(CATCH).cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

benchmark: benchmark.cpp $(BIN_DIR)/$(CATCH).o
	$(CXX)  $(CXXFLAGS)  $< $(BIN_DIR)/$(CATCH).o -o $(BIN_DIR)/$@ && $(BIN_DIR)/$@ $(args)

# 清理规则，删除所有生成的目标文件
clean:
	rm -f `find $(BIN_DIR)/* -name "*" ! -name "$(CATCH).o"`
