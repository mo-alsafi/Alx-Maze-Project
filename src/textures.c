#include "headers/maze.h"

/**
 * render_walls - draw walls
 * @instance: SDL instance
 * @maze: 2D maze
 * @map: xy point on the map
 * @ray_pos: xy ray position
 * @ray_dir: xy ray direction
 * @dist_to_wall: nearest distance from player to the wall
 * @x: point on the x axis
 * @side: stores 0 if wall is not on the side, 1 otherwise
*/
void render_walls(SDL_Instance *instance, int *maze, SDL_Point map,
		point ray_pos, point ray_dir, double dist_to_wall,
		int x, int side)
{
	int slice_height, draw_start, draw_end, tile_index, width, height, y;
	double wallX;
	SDL_Point tex;
	uint32_t color;

	if (!instance->textured)
	{
		SDL_GetWindowSize(instance->window, &width, &height);

		slice_height = (int)(height / dist_to_wall);

		draw_start = -slice_height / 2 + height / 2;
		if (draw_start < 0)
			draw_start = 0;

		 draw_end = slice_height / 2 + height / 2;
		 if (draw_end >= height)
		 	draw_end = height - 1;

		if (side == 0)
			SDL_SetRenderDrawColor(instance->renderer,
					200, 200, 200, 255);
		else if (side == 1)
			SDL_SetRenderDrawColor(instance->renderer,
					150, 150, 150, 255);

		SDL_RenderDrawLine(instance->renderer, x,
				draw_start, x, draw_end);
	}
	else /* textured */
	{
		slice_height = (int)(SCREEN_HEIGHT / dist_to_wall);

		draw_start = -slice_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_start < 0)
			draw_start = 0;
		
		draw_end = slice_height / 2 + SCREEN_HEIGHT / 2;
		if (draw_end >= SCREEN_HEIGHT)
			draw_end = SCREEN_HEIGHT - 1;
		
		wallX = 0;
		if (side == 0)
			wallX = ray_pos.y + dist_to_wall * ray_dir.y;
		else if (side == 1)
			wallX = ray_pos.x + dist_to_wall * ray_dir.x;
		
		tile_index = *((int *)maze + map.x * MAP_WIDTH + map.y) - 1;
		wallX -= floor(wallX);

		tex.x = (int)(wallX * (double)TEX_WIDTH);
		if (side == 0 && ray_dir.x > 0)
			tex.x = TEX_WIDTH - tex.x - 1;
		if (side == 1 && ray_dir.y < 0)
			tex.x = TEX_WIDTH - tex.x - 1;
		
		for (y = draw_start; y < draw_end; y++)
		{
			tex.y = ((((y << 1) - SCREEN_HEIGHT + slice_height)
						<< (int)log2(TEX_HEIGHT)) /
					slice_height) >> 1;

			color = tiles[tile_index][tex.x][tex.y];

			if (side == 1)
				color = (color >> 1) & 8355711;

			buffer[y][x] = color;
		}
		textured_floor_ceiling(map, ray_dir, dist_to_wall, wallX, draw_end, x, side);
	}
}

/**
 * textured_floor_ceiling - draw textured floors and ceilings
 * @map: xy point on the map
 * @ray_dir: xy ray direction
 * @dist_to_wall: nearest distance from player to the wall
 * @wallX: wall x
 * @draw_end: xy point where drawing should stop
 * @x: point on the x axis
 * @side: stores 0 if wall is not on the side, 1 otherwise
 *
 * Return: Nothing
*/
void textured_floor_ceiling(SDL_Point map, point ray_dir, double dist_to_wall,
		double wallX, int draw_end, int x, int side)
{
	point floor_wall, current_floor;
	SDL_Point floor_tex;
	double weight, currentDist;
	int y;

	if (side == 0 && ray_dir.x > 0)
	{
		floor_wall.x = map.x;
		floor_wall.y = map.y + wallX;
	}
		
	else if (side == 0 && ray_dir.x < 0)
	{
		floor_wall.x = map.x + 1.0;
		floor_wall.y = map.y + wallX;
	}
	else if (side == 1 && ray_dir.y > 0)
	{
		floor_wall.x = map.x + wallX;
		floor_wall.y = map.y;
	}
	else
	{
		floor_wall.x = map.x + wallX;
		floor_wall.y = map.y + 1.0;
	}
	
	if (draw_end < 0)
		draw_end = SCREEN_HEIGHT;
	
	for (y = draw_end + 1; y < SCREEN_HEIGHT; y++)
	{
		currentDist = SCREEN_HEIGHT / (2.0 * y - SCREEN_HEIGHT);
		weight = currentDist / dist_to_wall;
		
		current_floor.x = (
				weight * floor_wall.x + (1.0 - weight) * pos.x);
		current_floor.y = (
				weight * floor_wall.y + (1.0 - weight) * pos.y);

		floor_tex.x = (int)(current_floor.x * TEX_WIDTH) % TEX_WIDTH;
		floor_tex.y = (int)(current_floor.y * TEX_HEIGHT) % TEX_HEIGHT;

		/* add floor and ceiling textures to buffer */
		buffer[y][x] = tiles[5][floor_tex.y][floor_tex.x];
		buffer[SCREEN_HEIGHT - y][x] = tiles[4][floor_tex.y][floor_tex.x];
	}
}

/**
 * untextured_floor_ceiling - draw an untextured floor and ceiling
 * @instance: SDL instance
 *
 * Return: Nothing
*/
void untextured_floor_ceiling(SDL_Instance *instance)
{
	SDL_Rect floor;
	SDL_Rect ceiling;
	int width, height;

	SDL_GetWindowSize(instance->window, &width, &height);

	floor.x = 0, floor.y = height / 2, floor.w = width, floor.h = height / 2;
	ceiling.x = 0, ceiling.y = 0, ceiling.w = width, ceiling.h = height / 2;

	/* render ceiling */
	SDL_SetRenderDrawColor(instance->renderer, 135, 206, 253, 255);
	SDL_RenderFillRect(instance->renderer, &ceiling);

	/* render floor */
	SDL_SetRenderDrawColor(instance->renderer, 34, 139, 34, 255);
	SDL_RenderFillRect(instance->renderer, &floor);
}


/**
 * load_textures - load textures from image files
 *
 * Return: nothing
*/
void load_textures(void)
{
    SDL_Surface *textures[TEX_COUNT];
    uint8_t *pixel;
    int a, b, c;

    textures[0] = IMG_Load("./images/wall1.bmp");
    textures[1] = IMG_Load("./images/brownstone.png");
    textures[2] = IMG_Load("./images/brownstone.png");
    textures[3] = IMG_Load("./images/brownstone.png");
    textures[4] = IMG_Load("./images/ceiling.bmp");
    textures[5] = IMG_Load("./images/grass.jpg");

    for (a = 0; a < TEX_COUNT; a++)
    {
        if (!textures[a])
        {
            fprintf(stderr, "Error loading textures[%i]: %s\nExiting...\n", a, SDL_GetError());
            exit(1);
        }
    }

    for (a = 0; a < TEX_COUNT; a++)
    {
        SDL_LockSurface(textures[a]);
        for (b = 0; b < TEX_HEIGHT; b++)
        {
            for (c = 0; c < TEX_WIDTH; c++)
            {
                pixel = (uint8_t *)textures[a]->pixels + b * textures[a]->pitch + c * textures[a]->format->BytesPerPixel;
                uint32_t color = 0;

                if (textures[a]->format->BytesPerPixel == 3) { // RGB 24-bit
                    color = pixel[0] << 16 | pixel[1] << 8 | pixel[2]; // BGRA -> ARGB
                } else if (textures[a]->format->BytesPerPixel == 4) { // RGBA 32-bit
                    color = pixel[2] << 16 | pixel[1] << 8 | pixel[0] | pixel[3] << 24; // BGRA -> ARGB
                }

                tiles[a][c][b] = color; // Swap b and c to correct the rotation
            }
        }
        SDL_UnlockSurface(textures[a]);
    }

    /* free surface */
    for (a = 0; a < TEX_COUNT; a++)
    {
        SDL_FreeSurface(textures[a]);
        textures[a] = NULL;
    }
}
