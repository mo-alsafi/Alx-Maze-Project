#compiler used
CC = gcc

# source files
SRCS = src/*.c

#executable name
NAME = maze

# RM + flags
RM = rm -f

# compiler flags
CFLAGS = -g -Wall -Wextra -pedantic

# Linker flags
LFLAGS = -lm -lSDL2 -lSDL2_image

# sdl flags
SDLFLAGS = `sdl2-config --cflags --libs`

all: $(SRCS)
	$(CC) $(SRCS) $(CFLAGS) $(LFLAGS) $(SDLFLAGS) -o $(NAME)

# Clean temp files and executable
clean:
	$(RM) *~ $(NAME)
