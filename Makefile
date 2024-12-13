# Definições de compilador e flags
CC = gcc

# Flags de compilação
CFLAGS = -Wall $(shell pkg-config --cflags allegro-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5)

# Flags de linkagem
LDFLAGS =  $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5 allegro_audio-5 allegro_acodec-5) -lm

# Utiliza wildcard para encontrar todos os arquivos .c na pasta atual
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

# Nome do executável
EXECUTABLE = space_impact

# Regras principais
all: $(EXECUTABLE)

# Compilar o executável
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

# Regras para criar arquivos .o a partir dos arquivos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -g

# Limpa os arquivos gerados durante a compilação
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Limpa os arquivos objetos, mas mantém o executável
clean_obj:
	rm -f $(OBJECTS)
