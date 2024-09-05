#include "headers/maze.h"
/**
 * update_position - update player position
 * @maze: pointer to the maze map
 *
 * Return: Nothing
 */
void update_position(int *maze)
{
	const uint8_t *key = SDL_GetKeyboardState(NULL);
	double old_time, frame_time, oldDirX;
	double oldPlaneX, move_speed, rotate_speed;

	old_time = time;
	time = SDL_GetTicks();
	frame_time = (time - old_time) / 1000.0;
	move_speed = frame_time * 5.0, rotate_speed = frame_time * 3.0;

	if (key[SDL_SCANCODE_W]  || key[SDL_SCANCODE_UP])
	{ /* move forward if no wall */
		if (!*((int *)maze + (int)(pos.x + dir.x * move_speed) * MAP_WIDTH + (int)pos.y))
			pos.x += dir.x * move_speed;

		if (!*((int *)maze + (int)pos.x * MAP_WIDTH + (int)(pos.y + dir.y * move_speed)))
			pos.y += dir.y * move_speed;
	}
	else if (key[SDL_SCANCODE_S] || key[SDL_SCANCODE_DOWN])
	{ /* move backward if no wall */
		if (!*((int *)maze + (int)(pos.x - dir.x * move_speed) * MAP_WIDTH + (int)pos.y))
			pos.x -= dir.x * move_speed;

		if (!*((int *)maze + (int)pos.x * MAP_WIDTH + (int)(pos.y - dir.y * move_speed)))
			pos.y -= dir.y * move_speed;
	}
	else if (key[SDL_SCANCODE_L]) /* STRAFE left */
	{
		if (!*((int *)maze +
				(int)(pos.x - plane.x * move_speed) *
				MAP_WIDTH + (int)pos.y))
			pos.x -= plane.x * move_speed;

		if (!*((int *)maze + (int)pos.x * MAP_WIDTH +
				(int)(pos.y - plane.y * move_speed)))
			pos.y -= plane.y * move_speed;
	}
	else if (key[SDL_SCANCODE_R]) /* STRAFE right */
	{
		if (!*((int *)maze + (int)(pos.x + dir.x * move_speed) * MAP_WIDTH + (int)pos.y))
			pos.x += plane.x * move_speed;

		if (!*((int *)maze + (int)pos.x * MAP_WIDTH + (int)(pos.y + plane.y * move_speed)))
			pos.y += plane.y * move_speed;
	}

	if (key[SDL_SCANCODE_A] || key[SDL_SCANCODE_RIGHT])
	{ /* ROTATE CAMERA RIGHT */
		oldDirX = dir.x, oldPlaneX = plane.x;

		/* rotate camera direction */
		dir.x = dir.x * cos(rotate_speed) -
				dir.y * sin(rotate_speed);
		dir.y = oldDirX * sin(rotate_speed) +
				dir.y * cos(rotate_speed);

		/* rotate camera plane */
		plane.x = plane.x * cos(rotate_speed) -
				plane.y * sin(rotate_speed);
		plane.y = oldPlaneX * sin(rotate_speed) +
				plane.y * cos(rotate_speed);
	}
	else if (key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_D])
	{ /* ROTATE CAMERA LEFT */
		oldDirX = dir.x, oldPlaneX = plane.x;

		/* rotate camera direction */
		dir.x = dir.x * cos(-rotate_speed) -
				dir.y * sin(-rotate_speed);
		dir.y = oldDirX * sin(-rotate_speed) +
				dir.y * cos(-rotate_speed);

		/* rotate camera plane */
		plane.x = plane.x * cos(-rotate_speed) -
				plane.y * sin(-rotate_speed);
		plane.y = oldPlaneX * sin(-rotate_speed) + plane.y * cos(-rotate_speed);
	}
	
}

/**
 * maze_loop - check if a user quits
 * @instance: pointer to an SDL instance
 *
 * Return: 0 if a user has quit, 1 otherwise
*/
int maze_loop(SDL_Instance *instance)
{
	SDL_Event ev;
	int running = 0;
	uint32_t window_flags;

	while (SDL_PollEvent(&ev) != 0)
	{
		if (ev.type == SDL_QUIT)
			running = 1; /* quit program */

		if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
			running = 1; /*  quit program */

		/* full screen handling */
		if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_f)
		{
			window_flags = SDL_GetWindowFlags(instance->window);

			if (window_flags && SDL_WINDOW_FULLSCREEN)
				SDL_SetWindowFullscreen(instance->window, SDL_FALSE);
			else
				SDL_SetWindowFullscreen(instance->window,
						SDL_WINDOW_FULLSCREEN);
		}
	}
	return (running);
}
