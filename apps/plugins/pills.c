/*
	Author: Eric Conner
	Date: 08-27-2013
	Project: Dr. Mario - Uzebox
	Version: v1.0
*/

//#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
//#include <avr/pgmspace.h>
//#include <uzebox.h>

//#include "Title.c"
//#include "Select.c"
//#include "Game.c"

#define PLAYER1	0
#define PLAYER2	1

#define BLANK	164
#define HEART	222

#define HORIZ	0
#define VERT	1

#define FULL	2
#define HALF	1

#define LOW		1
#define MED		2
#define HI		3

#define FEVER	1
#define CHILL	2
#define OFF		3

#define BOTTLE_FIRST_COLUMN	11
#define BOTTLE_LAST_COLUMN	18
#define BOTTLE_FIRST_ROW	8
#define BOTTLE_LAST_ROW		23
#define BOTTLE_WIDTH		8
#define BOTTLE_DEPTH		16

#define MOVE_LEFT	1
#define MOVE_RIGHT	2
#define MOVE_DOWN	3
#define MOVE_UP		4
#define MOVE_SPIN	5
#define MOVE_START	6
#define MOVE_SELECT	7

#define VIRUS_LEFT_ROW		23
#define VIRUS_LEFT_COLUMN1	27
#define VIRUS_LEFT_COLUMN2	22

#define LEFT_BLUE		65
#define RIGHT_BLUE		66
#define LEFT_YELLOW		67
#define RIGHT_YELLOW	68
#define LEFT_RED		69
#define RIGHT_RED		70
#define BOTTOM_BLUE		71
#define TOP_BLUE		72
#define BOTTOM_YELLOW	73
#define TOP_YELLOW		74
#define BOTTOM_RED		75
#define TOP_RED			76
#define SINGLE_BLUE		77
#define SINGLE_YELLOW	78
#define SINGLE_RED		79

#define VIRUS_BLUE_1 	10
#define VIRUS_BLUE_2 	11
#define VIRUS_YELLOW_1 	12
#define VIRUS_YELLOW_2 	13
#define VIRUS_RED_1 	14
#define VIRUS_RED_2 	15

#define DISSAPEAR_ANIM_1	95
#define DISSAPEAR_ANIM_2	96
#define DISSAPEAR_ANIM_3	97
#define DISSAPEAR_ANIM_4	98
#define DISSAPEAR_ANIM_5	0

struct bottle {
	unsigned short space[8][16];
	unsigned short virus_count;
	bool check_for_sequences;
	unsigned short sequences_count;
	bool contains_floaters;
	unsigned short dissapear_anim;
};

struct pill {
	bool orientation;
	unsigned short length;
	unsigned short x;
	unsigned short y;
	unsigned short bottle_x;
	unsigned short bottle_y;
	unsigned short top;
	unsigned short bottom;
	unsigned short left;
	unsigned short right;
};

static bool Two_Player = false;
unsigned char difficulty_level[2];
unsigned char levelselect_level = 1;
unsigned int score[2];
unsigned int high_score = 1000;
unsigned short grab_input(unsigned short player);
unsigned short seed;
unsigned short virus_anim_counter = 0;
unsigned int PS_anim_counter = 0;
unsigned short PS_Box = 1;
unsigned short x,y,i,j,z = 0;
struct bottle bottle[2];
struct pill current_pill[2];
struct pill next_pill[2];
unsigned short gravity = 60;
bool pill_falling = 1;
unsigned short move_type = 0;
unsigned short speed = LOW;
unsigned short music = FEVER;
unsigned short auto_repeat_counter = 0;
unsigned short drop_floater_timer = 0;
bool MovePill(unsigned short player, unsigned short move_type);
unsigned short CheckColorMatch(unsigned short tile1, unsigned short tile2);

void Game1(void);
void Game2(void);
void LevelSelect(void);
void NextPill(unsigned short player);
void NewPill(unsigned short player);
void LockPill(unsigned short player);
void CheckMatching(unsigned short player);
void SplitPill(unsigned short player, unsigned short x, unsigned short y);
void DropFloaters(unsigned short player);
void DropFilledSpace(unsigned short player, unsigned short x, unsigned short y);
void DissapearAnim(unsigned short player);
void InitBottle(unsigned short player);
void AnimateVirus(void);

int main(void) {
	ClearVram();
	//SetTileTable(Main);
	SetFontTilesIndex(0);
	//DrawMap2(0, 0, Main_Title);

	SetTile(8, 20, HEART);

	score[PLAYER1] = 0;
	score[PLAYER2] = 0;

	levelselect_level = 1;

	difficulty_level[PLAYER1] = 1;
	difficulty_level[PLAYER2] = 1;

	speed = LOW;
	music = FEVER;

	while (1) {
		if (grab_input(PLAYER1) == MOVE_SELECT) {
			if (!Two_Player) {
				SetTile(8, 20, BLANK);
				SetTile(8, 22, HEART);
				while(ReadJoypad(PLAYER1) != 0);
			} else {
				SetTile(8, 20, HEART);
				SetTile(8, 22, BLANK);
				while(ReadJoypad(PLAYER1) != 0);
			}

			Two_Player = !Two_Player;
		}

		if (grab_input(PLAYER1) == MOVE_START) {
			while (ReadJoypad(PLAYER1) != 0);
			LevelSelect();
		}
	}
}

void Game1() {
	ClearVram();
	SetTileTable(Game);
	SetFontTilesIndex(64);
	DrawMap2(0, 0, Game_Map_1);
	PrintByte(27, 17, difficulty_level[PLAYER1], false);

	PrintInt(6, 9, score[PLAYER1], true);
	SetTile(7, 9, 80);
	PrintInt(6, 6, high_score, true);
	SetTile(7, 6, 80);

	if (speed == LOW) {
		DrawMap2(25, 20, LOW_Text);
		gravity = 60;
	} else if (speed == MED) {
		DrawMap2(25, 20, MED_Text);
		gravity = 40;
	} else {
		DrawMap2(26, 20, HI_Text);
		gravity = 20;
	}

	srand(seed);

	InitBottle(PLAYER1);
	NextPill(PLAYER1);
	NewPill(PLAYER1);

	i = 0;
	while (1) {
		if (GetVsyncFlag()) {
			ClearVsyncFlag();
			AnimateVirus();

			while (bottle[PLAYER1].check_for_sequences) {
				CheckMatching(PLAYER1);
			}

			if (drop_floater_timer < 10) {
				drop_floater_timer++;
			} else {
				drop_floater_timer = 0;
				if (bottle[PLAYER1].contains_floaters) {
					if (!bottle[PLAYER1].dissapear_anim) {
						DropFloaters(PLAYER1);
					}
				}
			}

			if (i < gravity) {
				if (bottle[PLAYER1].dissapear_anim) {
					DissapearAnim(PLAYER1);
				} else if (move_type = grab_input(PLAYER1)) {
					MovePill(PLAYER1, move_type);
				}
				i++;
			} else {
				MovePill(PLAYER1, MOVE_DOWN);
				i = 0;
			}

			PrintInt(6, 9, score[PLAYER1], true);
			if (score[PLAYER1] > high_score) {
				high_score = score[PLAYER1];
				PrintInt(6, 6, high_score, true);
			}

			if (bottle[PLAYER1].virus_count == 0) {
				for (x = 0; x < BOTTLE_WIDTH; x++) {
					for (y = 0; y < BOTTLE_DEPTH; y++) {
						bottle[PLAYER1].space[x][y] = 0;
						SetTile(BOTTLE_FIRST_COLUMN + x, BOTTLE_FIRST_ROW + y, bottle[PLAYER1].space[x][y]);
					}
				}
				while (1) {
					if (PS_anim_counter < 2500) {
						PS_anim_counter++;
					} else if (PS_anim_counter >= 2500) {
						PS_anim_counter = 0;
						if (PS_Box == 1) {
							DrawMap2(11, 13, Press_Start_1);
							PS_Box = 2;
						} else {
							DrawMap2(11, 13, Press_Start_2);
							PS_Box = 1;
						}
					}
					if (grab_input(PLAYER1) == MOVE_START) {
						while (ReadJoypad(PLAYER1) != 0);
						difficulty_level[PLAYER1]++;
						Game1();
					}
				}
			}
		}
	}
}

void Game2() {
	ClearVram();
	SetTileTable(Game);
	SetFontTilesIndex(59);
	DrawMap2(0, 0, Game_Map_1);

	score[PLAYER1] = 0;
	score[PLAYER2] = 0;

	while (1) {
		if (grab_input(PLAYER1) == MOVE_START) {
			while (ReadJoypad(PLAYER1) != 0);
			break;
		}
	}
	main();
}

void LevelSelect() {
	ClearVram();
	SetTileTable(Select);
	SetFontTilesIndex(0);
	DrawMap2(0,0,Select_1);

	if (!Two_Player) {
		DrawMap2(0,0,Select_1);
	} else {
		DrawMap2(0,0,Select_2);
	}

	DrawMap2(4, 6, VIRUS_LEVEL_SEL);
	PrintByte(23, 7, difficulty_level[PLAYER1], false);
	SetTile(5 + difficulty_level[PLAYER1], 9, 62);
	if (Two_Player) {
		SetTile(5 + difficulty_level[PLAYER2], 11, 63);
	}

	DrawMap2(4, 12, SPEED_LEVEL);
	SetTile(12, 15, 62);

	DrawMap2(4, 18, MUSIC_TYPE);
	DrawMap2(7, 21, MUSIC_FEVER_SEL);
	DrawMap2(14, 21, MUSIC_CHILL);
	DrawMap2(21, 21, MUSIC_OFF);

	while(1) {
		seed++;

		switch (grab_input(PLAYER1)) {
			case MOVE_DOWN:
				if (levelselect_level < 3) {
					levelselect_level++;
					while (ReadJoypad(PLAYER1) != 0);
				}
				break;
			case MOVE_UP:
				if (levelselect_level > 1) {
					levelselect_level--;
					while (ReadJoypad(PLAYER1) != 0);
				}
				break;
			case MOVE_START:
				if (!Two_Player) {
					Game1();
				} else {
					main();
				}
				break;
		}

		// Level Select
		if (levelselect_level == 1) {
			DrawMap2(4, 6, VIRUS_LEVEL_SEL);
			DrawMap2(4, 12, SPEED_LEVEL);
			DrawMap2(4, 18, MUSIC_TYPE);

			if ((grab_input(PLAYER1) == MOVE_RIGHT) && (difficulty_level[PLAYER1] < 20)) {
				difficulty_level[PLAYER1]++;
				PrintByte(23, 7, difficulty_level[PLAYER1], false);
				SetTile(4 + difficulty_level[PLAYER1], 9, 0);
				SetTile(5 + difficulty_level[PLAYER1], 9, 62);
				unsigned int count = 0;

				while (ReadJoypad(PLAYER1) != 0) {
					count++;
					if (count >= 15000 && (difficulty_level[PLAYER1] < 20)) {
						count = 0;
						difficulty_level[PLAYER1]++;
						PrintByte(23, 7, difficulty_level[PLAYER1], false);
						SetTile(4 + difficulty_level[PLAYER1], 9, 0);
						SetTile(5 + difficulty_level[PLAYER1], 9, 62);
					}
				}
			} else if ((grab_input(PLAYER1) == MOVE_LEFT) && (difficulty_level[PLAYER1] > 1)) {
				difficulty_level[PLAYER1]--;
				PrintByte(23, 7, difficulty_level[PLAYER1], false);
				SetTile(5 + difficulty_level[PLAYER1], 9, 62);
				SetTile(6 + difficulty_level[PLAYER1], 9, 0);
				unsigned int count = 0;

				while (ReadJoypad(PLAYER1) != 0) {
					count++;
					if (count >= 15000 && (difficulty_level[PLAYER1] > 1)) {
						count = 0;
						difficulty_level[PLAYER1]--;
						PrintByte(23, 7, difficulty_level[PLAYER1], false);
						SetTile(5 + difficulty_level[PLAYER1], 9, 62);
						SetTile(6 + difficulty_level[PLAYER1], 9, 0);
					}
				}
			}
		// Speed Select
		} else if (levelselect_level == 2) {
			DrawMap2(4, 6, VIRUS_LEVEL);
			DrawMap2(4, 12, SPEED_LEVEL_SEL);
			DrawMap2(4, 18, MUSIC_TYPE);

			if (speed == LOW) {
				SetTile(12, 15, 62);
				SetTile(17, 15, 0);
				SetTile(21, 15, 0);
			} else if (speed == MED) {
				SetTile(12, 15, 0);
				SetTile(17, 15, 62);
				SetTile(21, 15, 0);
			} else if (speed == HI) {
				SetTile(12, 15, 0);
				SetTile(17, 15, 0);
				SetTile(21, 15, 62);
			}

			if ((grab_input(PLAYER1) == MOVE_RIGHT) && (speed < 3)) {
				speed++;
				while (ReadJoypad(PLAYER1) != 0);
			} else if ((grab_input(PLAYER1) == MOVE_LEFT) && (speed > 1)) {
				speed--;
				while (ReadJoypad(PLAYER1) != 0);
			}
		// Music Select
		} else if (levelselect_level == 3) {
			DrawMap2(4, 6, VIRUS_LEVEL);
			DrawMap2(4, 12, SPEED_LEVEL);
			DrawMap2(4, 18, MUSIC_TYPE_SEL);

			if (music == FEVER) {
				DrawMap2(7, 21, MUSIC_FEVER_SEL);
				DrawMap2(14, 21, MUSIC_CHILL);
				DrawMap2(21, 21, MUSIC_OFF);
			} else if (music == CHILL) {
				DrawMap2(7, 21, MUSIC_FEVER);
				DrawMap2(14, 21, MUSIC_CHILL_SEL);
				DrawMap2(21, 21, MUSIC_OFF);
			} else if (music == OFF) {
				DrawMap2(7, 21, MUSIC_FEVER);
				DrawMap2(14, 21, MUSIC_CHILL);
				DrawMap2(21, 21, MUSIC_OFF_SEL);
			}

			if ((grab_input(PLAYER1) == MOVE_RIGHT) && (music < 3)) {
				music++;
				while (ReadJoypad(PLAYER1) != 0);
			} else if ((grab_input(PLAYER1) == MOVE_LEFT) && (music > 1)) {
				music--;
				while (ReadJoypad(PLAYER1) != 0);
			}
		}
	}
}

bool MovePill(unsigned short player, unsigned short move_type) {
	bool moved = 0;

	switch (move_type) {
		case MOVE_DOWN:
			if (current_pill[player].y == BOTTLE_LAST_ROW) {
				LockPill(player);
				NewPill(player);
				break;
			}

			if (current_pill[player].orientation == HORIZ) {
				if (bottle[player].space[current_pill[player].bottle_x][current_pill[player].bottle_y+1]||bottle[player].space[current_pill[player].bottle_x+1][current_pill[player].bottle_y+1]) {
					LockPill(player);
					NewPill(player);
				} else {
					Fill(current_pill[player].x,current_pill[player].y,current_pill[player].length,1,0);
					current_pill[player].y++;
					current_pill[player].bottle_y++;
					SetTile(current_pill[player].x,current_pill[player].y,current_pill[player].left);
					SetTile(current_pill[player].x+1,current_pill[player].y,current_pill[player].right);
					moved = 1;
				}
			} else {
				if (bottle[player].space[current_pill[player].bottle_x][current_pill[player].bottle_y+1]) {
					LockPill(player);
					NewPill(player);
				} else {
					Fill(current_pill[player].x,(current_pill[player].y - current_pill[player].length + 1),1,1,0);
					current_pill[player].y++;
					current_pill[player].bottle_y++;
					SetTile(current_pill[player].x,current_pill[player].y-1,current_pill[player].top);
					SetTile(current_pill[player].x,current_pill[player].y,current_pill[player].bottom);
					moved = 1;
				}
			}
			break;

		case MOVE_LEFT:
			if (current_pill[player].bottle_x == 0) {
				break;
			}

			if (current_pill[player].orientation == HORIZ) {
				if (!bottle[player].space[current_pill[player].bottle_x-1][current_pill[player].bottle_y]) {
					Fill(current_pill[player].x,current_pill[player].y,current_pill[player].length,1,0);
					current_pill[player].x--;
					current_pill[player].bottle_x--;
					SetTile(current_pill[player].x,current_pill[player].y,current_pill[player].left);
					SetTile(current_pill[player].x+1,current_pill[player].y,current_pill[player].right);
					moved = 1;
				}
			} else {
				if (!(bottle[player].space[current_pill[player].bottle_x-1][current_pill[player].bottle_y]||bottle[player].space[current_pill[player].bottle_x-1][current_pill[player].bottle_y-1])) {
					Fill(current_pill[player].x,current_pill[player].y-1,1,current_pill[player].length,0);
					current_pill[player].x--;
					current_pill[player].bottle_x--;
					SetTile(current_pill[player].x,current_pill[player].y-1,current_pill[player].top);
					SetTile(current_pill[player].x,current_pill[player].y,current_pill[player].bottom);
					moved = 1;
				}
			}
			break;

		case MOVE_RIGHT:
			if (current_pill[player].orientation == HORIZ) {
				if (current_pill[player].bottle_x == (BOTTLE_WIDTH-2)) {
					break;
				}
				if (!bottle[player].space[current_pill[player].bottle_x+2][current_pill[player].bottle_y]) {
					SetTile(current_pill[player].x,current_pill[player].y,0);
					current_pill[player].x++;
					current_pill[player].bottle_x++;
					SetTile(current_pill[player].x,current_pill[player].y,current_pill[player].left);
					SetTile(current_pill[player].x+1,current_pill[player].y,current_pill[player].right);
					moved = 1;
				}
			} else {
				if (current_pill[player].bottle_x == (BOTTLE_WIDTH-1)) {
					break;
				}
				if (!(bottle[player].space[current_pill[player].bottle_x+1][current_pill[player].bottle_y]||bottle[player].space[current_pill[player].bottle_x+1][current_pill[player].bottle_y-1])) {
					Fill(current_pill[player].x,current_pill[player].y-1,1,current_pill[player].length,0);
					current_pill[player].x++;
					current_pill[player].bottle_x++;
					SetTile(current_pill[player].x,current_pill[player].y-1,current_pill[player].top);
					SetTile(current_pill[player].x,current_pill[player].y,current_pill[player].bottom);
					moved = 1;
				}
			}
			break;

		case MOVE_SPIN:
			if (current_pill[player].orientation == HORIZ) {
				if (current_pill[player].bottle_y == 0) {
					if (!MovePill(player,MOVE_DOWN)) {
						break;
					}
				}
				if (bottle[player].space[current_pill[player].bottle_x][current_pill[player].bottle_y-1]) {
					if (!MovePill(player,MOVE_DOWN)) {
						break;
					}
				}
				Fill(current_pill[player].x,current_pill[player].y,2,1,0);
				current_pill[player].bottom = current_pill[player].left;
				current_pill[player].top=current_pill[player].right;
				current_pill[player].top+=6;
				current_pill[player].bottom+=6;
				SetTile(current_pill[player].x,current_pill[player].y-1,current_pill[player].top);
				SetTile(current_pill[player].x,current_pill[player].y,current_pill[player].bottom);
				moved = 1;
				current_pill[player].orientation = VERT;
			} else {
				if ((current_pill[player].bottle_x == BOTTLE_WIDTH-1)||(bottle[player].space[current_pill[player].bottle_x+1][current_pill[player].bottle_y]!= 0)) {
					if (!MovePill(player,MOVE_LEFT)) {
						if (bottle[player].space[current_pill[player].bottle_x-1][current_pill[player].bottle_y] == 0) {
							if (current_pill[player].bottle_x == 0) {
								break;
							}
							current_pill[player].left=current_pill[player].top;
							current_pill[player].right=current_pill[player].bottom;
							current_pill[player].left-=7;
							current_pill[player].right-=5;
							SetTile (current_pill[player].x,current_pill[player].y-1,0);
							current_pill[player].x--;
							current_pill[player].bottle_x--;
							SetTile(current_pill[player].x,current_pill[player].y,current_pill[player].left);
							SetTile(current_pill[player].x+1,current_pill[player].y,current_pill[player].right);
							current_pill[player].orientation=HORIZ;
							moved = 1;
						}
						break;
					}
				}
				Fill(current_pill[player].x,(current_pill[player].y - 1),1,2,0);
				current_pill[player].left = current_pill[player].top;
				current_pill[player].right = current_pill[player].bottom;
				current_pill[player].left -= 7;
				current_pill[player].right -= 5;
				SetTile(current_pill[player].x,current_pill[player].y,current_pill[player].left);
				SetTile(current_pill[player].x+1,current_pill[player].y,current_pill[player].right);
				current_pill[player].orientation = HORIZ;
				moved = 1;
			}
			break;
	}
	return moved;
}

void NextPill(unsigned short player) {
	do {
		z = LEFT_BLUE + (rand() % 6);
	} while ((z % 2) == 0);
	next_pill[player].left=z;

	do {
		z = LEFT_BLUE + (rand() % 6);
	} while ((z % 2) == 1);
	next_pill[player].right = z;

	next_pill[player].x = 22;
	next_pill[player].y = 7;

	SetTile(next_pill[player].x, next_pill[player].y, next_pill[player].left);
	SetTile(next_pill[player].x + 1, next_pill[player].y, next_pill[player].right);
}

void NewPill(unsigned short player) {
	if ((bottle[player].space[3][0] != 0) || (bottle[player].space[4][0] != 0)) {
		while (1) {
			if (PS_anim_counter < 2500) {
				PS_anim_counter++;
			} else if (PS_anim_counter >= 2500) {
				PS_anim_counter = 0;
				if (PS_Box == 1) {
					DrawMap2(11, 13, Game_Over_1);
					PS_Box = 2;
				} else {
					DrawMap2(11, 13, Game_Over_2);
					PS_Box = 1;
				}
			}
			if (grab_input(PLAYER1) == MOVE_START) {
				while (ReadJoypad(PLAYER1) != 0);
				main();
			}
		}
	}

	current_pill[player].x = 14;
	current_pill[player].y = BOTTLE_FIRST_ROW;
	current_pill[player].bottle_x = 3;
	current_pill[player].bottle_y = 0;
	current_pill[player].orientation = 0;
	current_pill[player].length = 2;
	current_pill[player].left = next_pill[player].left;
	current_pill[player].right = next_pill[player].right;

	SetTile(current_pill[player].x, current_pill[player].y, current_pill[player].left);
	SetTile(current_pill[player].x + 1, current_pill[player].y, current_pill[player].right);

	NextPill(player);
}

void LockPill(unsigned short player) {
	if (current_pill[player].orientation == HORIZ) {
		bottle[player].space[current_pill[player].bottle_x][current_pill[player].bottle_y]=current_pill[player].left;
		bottle[player].space[current_pill[player].bottle_x+1][current_pill[player].bottle_y]=current_pill[player].right;
	} else {
		bottle[player].space[current_pill[player].bottle_x][current_pill[player].bottle_y-1]=current_pill[player].top;
		bottle[player].space[current_pill[player].bottle_x][current_pill[player].bottle_y]=current_pill[player].bottom;
	}
	bottle[player].check_for_sequences = 1;
}

void CheckMatching(unsigned short player) {
	short unsigned int matching_spaces = 1;
	unsigned short first_column;
	unsigned short virus_left_column;
	bottle[player].check_for_sequences = 0;
	first_column = BOTTLE_FIRST_COLUMN;
	virus_left_column = VIRUS_LEFT_COLUMN1;

	for (x = 0; x < BOTTLE_WIDTH; x++) {
		for (y = 0; y < BOTTLE_DEPTH; y++) {
			if (bottle[player].space[x][y]) {
				matching_spaces = 1;
				for (j = 0; j < BOTTLE_DEPTH; j++) {
					if (y + j + 1 > BOTTLE_DEPTH)
					break;
					if (CheckColorMatch(bottle[player].space[x][y + j], bottle[player].space[x][y + j + 1])) {
						matching_spaces++;
					} else {
						if (matching_spaces >= 4) {
							for (j = 0; j < matching_spaces; j++) {
								SplitPill(player,x,y+j);
								bottle[player].contains_floaters = 1;
								if ((bottle[player].space[x][y+j] >= VIRUS_BLUE_1) && (bottle[player].space[x][y+j] <= VIRUS_RED_2)) {
									bottle[player].virus_count--;
									PrintByte(virus_left_column,VIRUS_LEFT_ROW,bottle[player].virus_count,false);
								}
								bottle[player].space[x][y+j]=DISSAPEAR_ANIM_1;
							}
							Fill(first_column+x,BOTTLE_FIRST_ROW+y,1,matching_spaces,DISSAPEAR_ANIM_1);
							bottle[player].dissapear_anim = 1;
							bottle[player].check_for_sequences = 1;
							bottle[player].sequences_count++;

							if (matching_spaces >= 4) {
								score[player] = (score[player] + 10);
							} else if (matching_spaces >= 5) {
								score[player] = (score[player] + 20);
							} else {
								score[player] = (score[player] + 30);
							}
						}
						break;
					}
				}
				matching_spaces = 1;
				for (j = 0; j < BOTTLE_WIDTH; j++) {
					if (x + j + 1 > BOTTLE_WIDTH)
					break;
					if (CheckColorMatch (bottle[player].space[x+j][y],bottle[player].space[x+j+1][y])) {
						matching_spaces++;
					} else {
						if (matching_spaces >= 4) {
							for (j = 0; j< matching_spaces; j++) {
								SplitPill(player,x+j,y);
								bottle[player].contains_floaters = 1;
								if ((bottle[player].space[x+j][y] >= VIRUS_BLUE_1) && (bottle[player].space[x+j][y] <= VIRUS_RED_2)) {
									bottle[player].virus_count--;
									PrintByte(virus_left_column,VIRUS_LEFT_ROW,bottle[player].virus_count,false);
								}
								bottle[player].space[x+j][y] = DISSAPEAR_ANIM_1;
							}
							Fill(first_column+x,BOTTLE_FIRST_ROW+y,matching_spaces,1,DISSAPEAR_ANIM_1);
							bottle[player].dissapear_anim = 1;
							bottle[player].check_for_sequences = 1;
							bottle[player].sequences_count++;

							if (matching_spaces >= 4) {
								score[player] = (score[player] + 10);
							} else if (matching_spaces >= 5) {
								score[player] = (score[player] + 20);
							} else {
								score[player] = (score[player] + 30);
							}
						}
						break;
					}
				}
			}
		}
	}
}

void SplitPill(unsigned short player, unsigned short x, unsigned short y) {
	unsigned short first_column;
	first_column=BOTTLE_FIRST_COLUMN;
	bottle[player].check_for_sequences = 0;
	switch (bottle[player].space[x][y]) {
		case SINGLE_BLUE ... SINGLE_RED:
			break;
		case LEFT_BLUE:
		case LEFT_YELLOW:
		case LEFT_RED:
			switch (bottle[player].space[x+1][y]) {
				case RIGHT_BLUE:
					bottle[player].space[x+1][y]=SINGLE_BLUE;
					SetTile(first_column+x+1,BOTTLE_FIRST_ROW+y,SINGLE_BLUE);
					break;
				case RIGHT_YELLOW:
					bottle[player].space[x+1][y]=SINGLE_YELLOW;
					SetTile(first_column+x+1,BOTTLE_FIRST_ROW+y,SINGLE_YELLOW);
					break;
				case RIGHT_RED:
					bottle[player].space[x+1][y]=SINGLE_RED;
					SetTile(first_column+x+1,BOTTLE_FIRST_ROW+y,SINGLE_RED);
					break;
			}
			break;
		case RIGHT_BLUE:
		case RIGHT_YELLOW:
		case RIGHT_RED:
			switch (bottle[player].space[x-1][y]) {
				case LEFT_BLUE:
					bottle[player].space[x-1][y]=SINGLE_BLUE;
					SetTile(first_column+x-1,BOTTLE_FIRST_ROW+y,SINGLE_BLUE);
					break;
				case LEFT_YELLOW:
					bottle[player].space[x-1][y]=SINGLE_YELLOW;
					SetTile(first_column+x-1,BOTTLE_FIRST_ROW+y,SINGLE_YELLOW);
					break;
				case LEFT_RED:
					bottle[player].space[x-1][y]=SINGLE_RED;
					SetTile(first_column+x-1,BOTTLE_FIRST_ROW+y,SINGLE_RED);
					break;
			}
			break;
		case BOTTOM_BLUE:
		case BOTTOM_YELLOW:
		case BOTTOM_RED:
			switch (bottle[player].space[x][y-1]) {
				case TOP_BLUE:
					bottle[player].space[x][y-1]=SINGLE_BLUE;
					SetTile(first_column+x,BOTTLE_FIRST_ROW+y-1,SINGLE_BLUE);
					break;
				case TOP_YELLOW:
					bottle[player].space[x][y-1]=SINGLE_YELLOW;
					SetTile(first_column+x,BOTTLE_FIRST_ROW+y-1,SINGLE_YELLOW);
					break;
				case TOP_RED:
					bottle[player].space[x][y-1]=SINGLE_RED;
					SetTile(first_column+x,BOTTLE_FIRST_ROW+y-1,SINGLE_RED);
					break;
			}
			break;
		case TOP_BLUE:
		case TOP_YELLOW:
		case TOP_RED:
			switch (bottle[player].space[x][y+1]) {
				case BOTTOM_BLUE:
					bottle[player].space[x][y+1]=SINGLE_BLUE;
					SetTile(first_column+x,BOTTLE_FIRST_ROW+y+1,SINGLE_BLUE);
					break;
				case BOTTOM_YELLOW:
					bottle[player].space[x][y+1]=SINGLE_YELLOW;
					SetTile(first_column+x,BOTTLE_FIRST_ROW+y+1,SINGLE_YELLOW);
					break;
				case BOTTOM_RED:
					bottle[player].space[x][y+1]=SINGLE_RED;
					SetTile(first_column+x,BOTTLE_FIRST_ROW+y+1,SINGLE_RED);
					break;
			}
		break;
	}
}

unsigned short CheckColorMatch(unsigned short tile1, unsigned short tile2) {
	switch (tile1) {
		case LEFT_BLUE ... RIGHT_BLUE:
		case BOTTOM_BLUE ... TOP_BLUE:
		case SINGLE_BLUE:
		case VIRUS_BLUE_1 ... VIRUS_BLUE_2:
			switch (tile2) {
				case LEFT_BLUE ... RIGHT_BLUE:
				case BOTTOM_BLUE ... TOP_BLUE:
				case SINGLE_BLUE:
				case VIRUS_BLUE_1 ... VIRUS_BLUE_2:
					return 1;
			}
			break;
		case LEFT_YELLOW ... RIGHT_YELLOW:
		case BOTTOM_YELLOW ... TOP_YELLOW:
		case SINGLE_YELLOW:
		case VIRUS_YELLOW_1 ... VIRUS_YELLOW_2:
			switch (tile2) {
				case LEFT_YELLOW ... RIGHT_YELLOW:
				case BOTTOM_YELLOW ... TOP_YELLOW:
				case SINGLE_YELLOW:
				case VIRUS_YELLOW_1 ... VIRUS_YELLOW_2:
					return 1;
			}
			break;
		default :
			switch (tile2) {
				case LEFT_RED ... RIGHT_RED:
				case BOTTOM_RED ... TOP_RED:
				case SINGLE_RED:
				case VIRUS_RED_1 ... VIRUS_RED_2:
					return 1;
			}
		break;
	}
	return 0;
}

void DropFloaters(unsigned short player) {
	bottle[player].contains_floaters = 0;
	for (y=BOTTLE_DEPTH-2; y > 0; y--) {
		for (x = 0; x< BOTTLE_WIDTH; x++) {

			switch (bottle[player].space[x][y]) {
				case VIRUS_BLUE_1 ... VIRUS_RED_2:
					break;

				case 0:
					break;
				case SINGLE_BLUE ... SINGLE_RED:
					if (bottle[player].space[x][y+1] == 0) {
						DropFilledSpace(player,x,y);
						bottle[player].contains_floaters = 1;
					}
					break;
				case LEFT_BLUE:
				case LEFT_RED:
				case LEFT_YELLOW:
					if((bottle[player].space[x][y+1] == 0) && (bottle[player].space[x+1][y+1] == 0)) {
						DropFilledSpace(player,x,y);
						DropFilledSpace(player,x+1,y);
						bottle[player].contains_floaters = 1;
					}
					break;

				case BOTTOM_BLUE:
				case BOTTOM_RED:
				case BOTTOM_YELLOW:
					if (bottle[player].space[x][y+1] == 0) {
						DropFilledSpace(player,x,y);
						DropFilledSpace(player,x,y-1);
						bottle[player].contains_floaters = 1;
					}
				break;
			}
		}
	}
	if (!bottle[player].contains_floaters) {
		CheckMatching(player);
		bottle[player].sequences_count = 0;
	}
}

void DropFilledSpace(unsigned short player, unsigned short x, unsigned short y) {
	unsigned short first_column;
	first_column=BOTTLE_FIRST_COLUMN;
	bottle[player].space[x][y+1]=bottle[player].space[x][y];
	SetTile(first_column+x,BOTTLE_FIRST_ROW+y+1,bottle[player].space[x][y+1]);
	bottle[player].space[x][y] = 0;
	SetTile(first_column+x,BOTTLE_FIRST_ROW+y,0);
}

void DissapearAnim(unsigned short player) {
	bottle[player].dissapear_anim = 0;
	unsigned short first_column;
	first_column = BOTTLE_FIRST_COLUMN;
	for (x = 0; x< BOTTLE_WIDTH; x++) {
		for (y = 0; y< BOTTLE_DEPTH; y++) {
			if ((bottle[player].space[x][y] >= DISSAPEAR_ANIM_1) && (bottle[player].space[x][y] <= DISSAPEAR_ANIM_3)) {
				bottle[player].space[x][y]++;
				SetTile(first_column+x,BOTTLE_FIRST_ROW+y,bottle[player].space[x][y]);
				bottle[player].dissapear_anim = 1;
			} else if (bottle[player].space[x][y] == DISSAPEAR_ANIM_4) {
				bottle[player].space[x][y] = 0;
				SetTile(first_column+x,BOTTLE_FIRST_ROW+y,bottle[player].space[x][y]);
				bottle[player].dissapear_anim = 1;
			}
		}
	}
}

void InitBottle(unsigned short player) {
	for (x = 0; x < BOTTLE_WIDTH; x++) {
		for (y = 0; y < BOTTLE_DEPTH; y++) {
			bottle[player].space[x][y] = 0;
		}
	}

	bottle[player].check_for_sequences = 0;
	bottle[player].virus_count = 0;
	bottle[player].contains_floaters = 0;
	bottle[player].sequences_count = 0;
	bottle[player].dissapear_anim = 0;

	z = 4 * difficulty_level[player];

	while (bottle[player].virus_count < z) {
		x = rand() % BOTTLE_WIDTH;
		y = (5 + (rand() % (BOTTLE_DEPTH - 5)));

		if (!bottle[player].space[x][y]) {
			bottle[player].space[x][y] = VIRUS_BLUE_1 + (rand() % 5);
			bottle[player].virus_count++;
			SetTile(BOTTLE_FIRST_COLUMN + x, BOTTLE_FIRST_ROW + y, bottle[player].space[x][y]);
		}
	}
}

void AnimateVirus() {
	if (virus_anim_counter < 15) {
		virus_anim_counter++;
	} else {
		virus_anim_counter = 0;
		for (x = 0; x < BOTTLE_WIDTH; x++) {
			for (y = 0; y < BOTTLE_DEPTH; y++) {
				switch (bottle[PLAYER1].space[x][y]) {
					case VIRUS_BLUE_1:
					case VIRUS_YELLOW_1:
					case VIRUS_RED_1:
						bottle[PLAYER1].space[x][y]++;
						SetTile(BOTTLE_FIRST_COLUMN + x,BOTTLE_FIRST_ROW + y,bottle[PLAYER1].space[x][y]);
						SetTile(BOTTLE_FIRST_COLUMN + x,BOTTLE_FIRST_ROW + y,bottle[PLAYER1].space[x][y]);
						break;
					case VIRUS_BLUE_2:
					case VIRUS_YELLOW_2:
					case VIRUS_RED_2:
						bottle[PLAYER1].space[x][y]--;
						SetTile(BOTTLE_FIRST_COLUMN + x,BOTTLE_FIRST_ROW + y,bottle[PLAYER1].space[x][y]);
						SetTile(BOTTLE_FIRST_COLUMN + x,BOTTLE_FIRST_ROW + y,bottle[PLAYER1].space[x][y]);
						break;
				}
			}
		}
	}
}

unsigned short grab_input(unsigned short player) {
	static bool holdKey[2];
	unsigned int c = ReadJoypad(player);

	if(!(c&BTN_A)) holdKey[player] = false;

	if (auto_repeat_counter < 4) {
		auto_repeat_counter++;
		return 0;
	}

	auto_repeat_counter = 0;

	if (c&BTN_A && holdKey[player] == false) {
		holdKey[player] = true;
		return MOVE_SPIN;
	} else if (c&BTN_RIGHT) {
		return MOVE_RIGHT;
	} else if (c&BTN_LEFT) {
		return MOVE_LEFT;
	} else if (c&BTN_START) {
		return MOVE_START;
	} else if (c&BTN_SELECT) {
		return MOVE_SELECT;
	} else if (c&BTN_DOWN) {
		return MOVE_DOWN;
	} else if (c&BTN_UP) {
		return MOVE_UP;
	}
	return 0;
}
