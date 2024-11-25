# Definições de compilador e flags
CC = gcc
CFLAGS = -Wall $(shell pkg-config --cflags allegro-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5)
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_primitives-5 allegro_image-5 allegro_ttf-5)

# Utiliza wildcard para encontrar todos os arquivos .c na pasta atual
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

# Nome do executável
EXECUTABLE = space_impact

# Regras principais
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Regras para criar arquivos .o a partir dos arquivos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa os arquivos gerados durante a compilacão
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Limpa os arquivos objetos, mas mantém o executável
clean_obj:
	rm -f $(OBJECTS)
