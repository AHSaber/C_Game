#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <curses.h>
#include "bullet.c"
#include "player.c"
#include "display.c"

int main () 
{
	//create window, s.t. key inputs are allowed but not displayed
	initscr();
	cbreak();
	noecho();
	curs_set(FALSE);
	keypad(stdscr,TRUE);
	halfdelay(1);

	//max x and y coordinates to deal with initial positioning
	int max_x=0;
	int max_y=0;
	getmaxyx(stdscr,max_y,max_x);


	//array of all players, first player is always the user
	int num_enemies=1;
	int num_friendlies=1;
	Player ** players[num_friendlies+num_enemies];


	//Initializing Player inside of the grid
	Player player_1;
	player_1.x_pos=max_x/2;
	player_1.y_pos=(max_y-2);
	player_1.character="^";
	player_1.friendly=TRUE;
	player_1.ammo_size=5;
	player_1.alive=TRUE;
	init_ammo(player_1.ammo_size,&player_1);

	players[0]=&player_1;

	//Initializing a single enemy inside of the grid
	Player enemy_1;
	enemy_1.x_pos=max_x/2;
	enemy_1.y_pos=0;
	enemy_1.character="V";
	enemy_1.friendly=FALSE;
	enemy_1.ammo_size=5;
	enemy_1.alive=TRUE;
	init_ammo(enemy_1.ammo_size,&enemy_1);

	players[1]=&enemy_1;


	//key variable stores current key pressed
	wchar_t key;


	Bullet * temp_bullet=malloc(sizeof(Bullet));
	temp_bullet->x_pos = max_x-1;
	temp_bullet->y_pos = max_y -1;
	temp_bullet->character= "f";
	temp_bullet->is_shot=TRUE;
	Player * temp_player = malloc(sizeof(Player));
	temp_player->ammo=&temp_bullet;
	temp_player->ammo_size=1;
	temp_player->friendly=TRUE;


	//start game
	for(;;){
		//refresh max x and y to deal with screen resizing
		getmaxyx(stdscr,max_y,max_x);


		//UPDATE positioning of characters and bullets every 1/2 second
		if((key=getch()) != ERR)
		{
			switch(key)
			{
				case KEY_LEFT:
					if(player_1.x_pos > 1)
					{
						player_1.x_pos-=1;
					}
					break;
				case KEY_RIGHT:
					if(player_1.x_pos < (max_x -1))
					{
						player_1.x_pos+=1;
					}
					break;
				case KEY_UP:
					shoot(&player_1);
					break;
			}
		}
		update_bullets(&player_1,max_y,max_x);
		update_bullets(&enemy_1,max_y,max_x);



		//go through all enemies to check if they've already been shot if so make them dead
		make_dead(players+num_friendlies,num_enemies);


		//if enemies are shot 
		is_enemy_shot(players,num_enemies,num_friendlies);



		//DISPLAY updated positions of a players, enemies and bullets
		clear();
		display_players(num_friendlies+num_enemies,players);
		display_ammo(&player_1,max_y,max_x);
		refresh();

	}
	return 0;
}








