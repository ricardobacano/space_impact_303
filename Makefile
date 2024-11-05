# Definições de compilador e flags
CC = gcc
CFLAGS = -Wall -std=c99 $(shell pkg-config --cflags allegro-5 allegro_font-5 allegro_primitives-5)
LDFLAGS = $(shell pkg-config --libs allegro-5 allegro_font-5 allegro_primitives-5)

# Arquivos-fonte e de objetos
SOURCES = AggressiveSquares.c Square.c HealthBar.c StartScreen.c Enemy.c ShooterEnemy.c Pistol.c Bullet.c Score.c
OBJECTS = $(SOURCES:.c=.o)

# Nome do executável
EXECUTABLE = AggressiveSquares

# Regras principais
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Regras para criar arquivos .o a partir dos arquivos .c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpa os arquivos gerados durante a compilação
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Limpa os arquivos objetos, mas mantém o executável
clean_obj:
	rm -f $(OBJECTS)
