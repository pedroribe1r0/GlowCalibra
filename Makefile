# Definições de variáveis
CXX = g++
CXXFLAGS = -std=c++17 -g -I/usr/include/opencv4  # Caminho para os cabeçalhos do OpenCV
LDFLAGS = -L/usr/lib `pkg-config --libs opencv4`  # Caminho para as bibliotecas do OpenCV
SRC_DIR = src
BIN_DIR = bin
INCLUDE_DIR = include
TARGET = $(BIN_DIR)/image_analysis

# Lista de arquivos fonte (.cpp)
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Geração dos objetos (.o)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Regra padrão de compilação
all: $(TARGET)

# Regra para gerar o executável final
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "Executável criado em $(TARGET)"

# Regra para compilar os arquivos fonte em objetos
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Regra de limpeza
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)
	@echo "Limpeza concluída!"

# Regra para rodar o programa (opcional)
run: $(TARGET)
	./$(TARGET)
