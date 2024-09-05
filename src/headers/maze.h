#ifndef MAZE_H
#define MAZE_H

#include "structs.h"

/* global variables */

extern uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH];
extern uint32_t tiles[TEX_COUNT][TEX_HEIGHT][TEX_WIDTH];
extern point pos;
extern point dir;
extern point plane;
extern double time; /* current frametime */

/* function prototypes */
/* init.c */

int initialize_maze(SDL_Instance *instance);
void destroy_maze(SDL_Instance *instance);
void update_renderer(SDL_Instance *instance);

/* textures.c */

void untextured_floor_ceiling(SDL_Instance *instance);
void render_walls(SDL_Instance *instance, int *maze,
		SDL_Point map, point ray_pos, point ray_dir,
		double dist_to_wall, int x, int side);

void textured_floor_ceiling(SDL_Point map, point ray_dir,
		double dist_to_wall, double wallX, int draw_end, int x, int side);


/* maps.c */

int *read_map(char *filename);
void load_textures(void);

/* events.c */

void update_position(int *maze);
int maze_loop(SDL_Instance *instance);

/* raycast.c */

void ray_cast(SDL_Instance *instance, int *maze_map);

/* get_opts.c */

char *handle_options(SDL_Instance *instance, int argc, char **argv);
void print_help(char **argv);

#endif /* MAZE_H */
