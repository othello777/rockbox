/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 Björn Stenberg
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/

/* welcome to the example rockbox plugin */

/* mandatory include for all plugins */
#include "plugin.h"
#include "string.h"
#define AMP 32000

//characters
static char Avatar;
static char Avatar2;
static char BonusPoint;
static char Backdrop;
static char Block;

//static Random random1 = new Random(DateTime.Now.Millisecond);
//static Random random2 = new Random(DateTime.Now.Second);
//static Write write = new Write();
//static GameMusic music = new GameMusic();
static char *Stringbuilder; //Stringbuilder = new StringBuilder();
//static KeyCode PressedKey = KeyCode.None;
//static KeyCode JustPressedKey = KeyCode.None;
static char *WriteBoard;
const char *Version = "1.5.2 - fps";
const char *Title = "DodgeBlock " + Version;
static int MaxMode = 4;
static char *settingslocation;
static char *SecretCode = "You have not died yet.";
static bool loadsuccess;
static bool FlasherFlipFlop;
static bool PowerFliper;
static bool MuteMusic;
static bool MuteSfx;
static bool Refresh;
static bool GodMode;
static bool Quitting;
static bool TwoPlayerMode;
static bool AdminPlayer2;
static bool FullScreen;
static bool ColorMode;
static bool raadmodetimerding;
static bool IsHolloween;
static bool IsThanksgiving;
static bool IsChristmas;
static bool UseNewControls;
static bool AllowInternet;
static bool UseCheckpoints;
static int BoardSize;
static int ScrResX;
static int ScrResY;
static int FlasherCounter;
static int TimerCounter;
static int ScoreFlashTimer;
static int explosiontimer;
static int snowflaketimer;
static int timer10;
static int timer100;
static int timer300;
static int MusicSelector;
//static ShotC shotc = new ShotC();
//static Bench BenchFPS = new Bench();
//static CustomModeC customModeC;
static int Ammo;
static int Shields;
static int Mode;
static int Score;
static int HighScore;
static int Tick;
static int BlockTickBase;
static int BlockTick;
static int BlockTickCounter;
static int CurveTimer;
static int GameWidth;
static int GameHeight;
static int Position;
static int Position2;
static int OldPosition;
static int OldPosition2;
static char BackBoard[][];
static char UseBoard[][];

static int GetRand(int from, int to)
{
	return from + (int)(rb->rand * ((to - from) + RAND_MAX));
}

static int GetRand2(int from, int to)
{
	return GetRand(from, to);
}

static void TextBoxWriteLine(char *string)
{

}

static void DoSleep(int ms)
{
	rb->sleep(HZ * ms / 1000);
}

static void IsKeyDown(int key)
{

}


static void Game()
{
	TextBoxWriteLine("Start!");
	//init
	TextBoxWriteLine("Initializing");
	Initialize();
	TextBoxWriteLine("Setting Colors");
	InitConsoleColors();
	BlueBackground(false);

	ImportSettings();

	InternetConnect();

	//Start Screen
	StartScreen();

	//if (MuteMusic)
	//	music.PauseMusic();

	if (Mode > MaxMode)
	{
		Tick = 10;
		Avatar = '%';
		Avatar2 = '$';
		Backdrop = 'O';
		BonusPoint = '.';
	}



	Run();



	TextBoxWriteLineRtf("\nQuitting...");

	/*if (MuteMusic == false)
		music.TerminateMusic();
	else
	{
		music.ResumeMusic();
		music.TerminateMusic();
	}

	DiscordDB.die();*/

	DoSleep(1000);
	CloseThis();


}



static void Initialize()
{
	Avatar = 'O';
	Avatar2 = 'S';
	BonusPoint = '$';
	Backdrop = '.';
	Block = '%';

	Score = 0;

	Mode = 0;
	Ammo = 0;
	Shields = 0;
	MusicSelector = 0;
	FlasherCounter = 0;
	TimerCounter = 0;
	ScoreFlashTimer = 0;
	explosiontimer = 0;
	timer10 = 0;
	timer100 = 0;
	timer300 = 0;
	snowflaketimer = 0;
	HighScore = 1530;
	WriteBoard = "";
	settingslocation = "Settings.txt";
	if (GetRand2(0, 10) % 2 == 1)
		PowerFliper = true;
	else PowerFliper = false;
	MuteMusic = false;
	MuteSfx = false;
	Refresh = true;
	GodMode = false;
	Quitting = false;
	TwoPlayerMode = false;
	AdminPlayer2 = false;
	FullScreen = false;
	ColorMode = true;
	raadmodetimerding = false;
	UseNewControls = true;
	AllowInternet = false;
	UseCheckpoints = true;
	BoardSize = 40;
	ScrResX = 1920;
	ScrResY = 1080;
	Tick = 60;
	BlockTickBase = 6;
	BlockTick = BlockTickBase;
	BlockTickCounter = 0;
	CurveTimer = 0;
	GameWidth = 12;
	GameHeight = 7;
	Position = 6;
	Position2 = 7;
	OldPosition = 6;
	OldPosition2 = 7;
	//BackBoard = new char[16, 16];
	//write.DestinationFile = settingslocation;

	//Figure out if it is a holiday
	/*if (HolidayTest.IsHoloween())
	{
		IsHolloween = true;
	}
	else if (HolidayTest.IsThanksgiving())
	{
		IsThanksgiving = true;
	}
	else if (HolidayTest.IsChristmas())
	{
		IsChristmas = true;
	}
	else
	{
		IsHolloween = false;
		IsThanksgiving = false;
		IsChristmas = false;
	}

	customModeC = new CustomModeC();
*/
	//Generate Reference
	for (int i = 0; i < GameHeight; i++)
	{
		for (int j = 0; j < GameWidth; j++)
		{
			BackBoard[j, i] = '.';
		}
	}
	UseBoard = BackBoard;//.Clone() as char[,];

	//DiscordDB.DoesWork = false;//= DiscordDB.DiscordInit();

	GameInitialized();

	//DiscordDB.UpdatePresence();
}

static void ImportSettings()
{
	/*TextBoxWriteLine("Importing Settings From " + settingslocation);
	try
	{
		string[] Settings = File.ReadAllLines(settingslocation);

		MuteMusic = Boolconvert(Settings[5]);
		MuteSfx = Boolconvert(Settings[7]);
		GodMode = Boolconvert(Settings[9]);
		FullScreen = Boolconvert(Settings[11]);
		AllowInternet = Boolconvert(Settings[13]);
		AdminPlayer2 = Boolconvert(Settings[21]);
		customModeC.BoardX = Convert.ToInt32(Settings[15]);
		customModeC.BoardY = Convert.ToInt32(Settings[17]);
		UseNewControls = Boolconvert(Settings[19]);
		Mode = Convert.ToInt32(Settings[23]);
		customModeC.Tick = Convert.ToInt32(Settings[29]);
		MusicSelector = Convert.ToInt32(Settings[37]);
		UseCheckpoints = Boolconvert(Settings[39]);

		music.init();
		customModeC.SettingsChanged();*/

		if (IsHolloween || IsChristmas)
		{
			Mode = 4;
		}

		if (FullScreen)
		{
			TextBoxWriteLine("Fullscreening");
			Fullscreen(true);
		}

		loadsuccess = true;
		ImportHighScore();

		TwoPlayerMode = false;
		ColorMode = true;
		ConvertTwoPlayerMode();


	/*}
	catch (Exception ex)
	{
		TextBoxWriteLine("Could not import settings from " + settingslocation + " " + ex);
		TextBoxWriteLine("Press any key to continue...");
		Console.Beep();
		Console.Beep();
		loadsuccess = false;
		////Console.ReadKey();
		DoSleep(2000);
	}*/

}

static void InternetConnect()
{/*
	//See if we are allowed to use Internet
	if (!AllowInternet)
	{
		bool NotAnswered = true;
		while (NotAnswered)
		{
			TextBoxReplace(
				"Would you like to allow DodgeBlock \n" +
				"to connect to dodgeblock.cf to\n" +
				"retrieve basic information?\n \n" +
				"Left = Yes | Right = No");

			if (IsKeyDown(2))
			{
				write.ToThisTxt(13, "1");
				AllowInternet = true;
				NotAnswered = false;
			}
			if (IsKeyDown(3))
			{
				return;
			}
			DoSleep(150);
		}
	}

	TextBoxWriteLine("Connecting to dodgeblock.cf");

	bool Newer = false;
	try
	{
		//fix https error
		ServicePointManager.SecurityProtocol = (SecurityProtocolType)3072;
		ServicePointManager.SecurityProtocol = SecurityProtocolType.Ssl3 | (SecurityProtocolType)(0xc0 | 0x300 | 0xc00);

		//Get Website HTML
		string htmlCode;
		using (WebClient client = new WebClient()) // WebClient class inherits IDisposable
		{
			htmlCode = client.DownloadString("https://dodgeblock.cf/index.html");
		}

		//Calculate if there is a "Newer" version
		int first = htmlCode.IndexOf("<!--$#$-->");
		int last = htmlCode.IndexOf("<!--#$#-->");
		first += 10;
		string DBVER = htmlCode.Substring(first, last - first);


		if (int.Parse(DBVER.Substring(0, 1)) > int.Parse(Version.Substring(0, 1)))
		{
			Newer = true;
		}

		if (int.Parse(DBVER.Substring(0, 1)) >= int.Parse(Version.Substring(0, 1)))
		{
			if (int.Parse(DBVER.Substring(2, 1)) > int.Parse(Version.Substring(2, 1)))
			{
				Newer = true;
			}
		}

		if (int.Parse(DBVER.Substring(0, 1)) >= int.Parse(Version.Substring(0, 1)))
		{
			if (int.Parse(DBVER.Substring(2, 1)) >= int.Parse(Version.Substring(2, 1)))
			{
				if (int.Parse(DBVER.Substring(4, 1)) > int.Parse(Version.Substring(4, 1)))
				{
					Newer = true;
				}
			}
		}
	}
	catch(WebException)
	{
		TextBoxWriteLine("Failed to connect");
	}
	catch (Exception)
	{
		TextBoxWriteLine("Failed to try to connect");
	}

	//Send Reaction
	bool NotReturned = false;
	if (Newer)
		NotReturned = true;
	while (NotReturned)
	{
		TextBoxReplace(
			"A newer version of DodgeBlock is \n" +
			"available from the website (dodgeblock.cf) \n \n" +
			"Press Down to Continue");

		if (IsKeyDown(1))
		{
			NotReturned = false;
		}
		DoSleep(150);
	}*/
}

static void InitConsoleColors()
{
	//StringBuilderSetBackgroundColor = ConsoleColor.Black;
	//StringBuilderSetColor = ConsoleColor.White;
}



static void Run(){
	//Run
	while (Refresh){

		TestKeydowns();

		PositionPlayers();

		/*if (ScoreFlashTimer <= TimerCounter && ScoreFlashTimer > TimerCounter - 10 &&
				ColorMode == true && TimerCounter > 20)
			InitStringBuilder(ConsoleColor.Magenta);
		else
			InitStringBuilder(ConsoleColor.White);
*/
		BlockHandling();


		WriteBoard = Stringbuilder;

		DisplayAndDelay();

		//DiscordDB.Update();
		DoSleep(Tick);
	}
}



static void TestKeydowns()
{
	//NativeKeyboard.Update();

	//Test Keydowns
	if (IsKeyDown(2))
	{
		OldPosition = Position;
		Position = Position - 1;
		if (Position < 0)
			Position = 0;
	}
	if (IsKeyDown(3))
	{
		OldPosition = Position;
		Position = Position + 1;
		if (Position > GameWidth - 1)
			Position = GameWidth - 1;
	}
	if (IsKeyDown(0))
	{
		/*if (!shotc.IsAlive && Ammo > 0 && shotc.ShootCooldown == 0)
		{
			Ammo -= 1;

			shotc.Y = 0;
			shotc.X = Position;
			shotc.IsAlive = true;

			shotc.ShootCooldown = 4;
		}*/
	}

	if (IsKeyDown(1))
	{
		if (Quitting == false)
		{
			//NewSettingsMenu.SettingsMenu();
		}

	}
}



static void PositionPlayers()
{
	//Position
	UseBoard[Position, 0] = 'O';
	if (OldPosition != Position && UseBoard[OldPosition, 0] == 'O')
		UseBoard[OldPosition, 0] = '.';
	//position player2 (if using)
	if (TwoPlayerMode)
	{
		if (Position == Position2)
		{
			UseBoard[Position, 0] = '@';
		}
		else
		{
			UseBoard[Position2, 0] = 'S';
		}
		if (OldPosition2 != Position2 && OldPosition2 != Position
			&& UseBoard[OldPosition2, 0] == 'S')
			UseBoard[OldPosition2, 0] = '.';

	}

	//Design Shield (Shell)
	if (Shields > 0)
	{
		if (UseBoard[Position, 1] == '.' || UseBoard[Position, 1] == '%' || UseBoard[Position, 1] == '-')
		{

			UseBoard[Position, 1] = '-';

		}

		if (OldPosition != Position && UseBoard[OldPosition, 1] == '-')
			UseBoard[OldPosition, 1] = '.';
	}
}

//Display&Delay
static void DisplayAndDelay()
{
	//Display
	WriteToScreen();
	TextBoxReplaceRtf(WriteBoard);
	//BenchFPS.OnMapUpdated();

	//Delay
	if (GodMode == false)
		Score += 1;
	TimerCounter += 1;
	UpdateDificultyCurve();
	if (Score > HighScore)
		HighScore = Score;
}

/*static ConsoleColor StringBuilderSetColor
{
	set
	{

		switch (value)
		{
			case ConsoleColor.Black:
				Stringbuilder += ("\cf0");
				break;
			case ConsoleColor.White:
				Stringbuilder += ("\cf1");
				break;
			case ConsoleColor.Green:
				Stringbuilder += ("\cf2");
				break;
			case ConsoleColor.Yellow:
				Stringbuilder += ("\cf3");
				break;
			case ConsoleColor.Magenta:
				Stringbuilder += ("\cf4");
				break;
			case ConsoleColor.Red:
				Stringbuilder += ("\cf5");
				break;
			case ConsoleColor.Blue:
				Stringbuilder += ("\cf6");
				break;
			case ConsoleColor.Cyan:
				Stringbuilder += ("\cf7");
				break;
			case ConsoleColor.DarkMagenta:
				Stringbuilder += ("\cf8");
				break;
			case ConsoleColor.DarkGreen:
				Stringbuilder += ("\cf9");
				break;
			case ConsoleColor.DarkGray:
				Stringbuilder += ("\cf10");
				break;
			default:
				Stringbuilder += ("\cf1");
				break;

		}


	}
}

static ConsoleColor StringBuilderSetBackgroundColor
{
	set
	{

		switch (value)
		{
			case ConsoleColor.Black:
				Stringbuilder += ("\highlight0");
				break;
			case ConsoleColor.White:
				Stringbuilder += ("\highlight1");
				break;
			case ConsoleColor.Magenta:
				Stringbuilder += ("\highlight4");
				break;
			case ConsoleColor.Green:
				Stringbuilder += ("\highlight2");
				break;
			case ConsoleColor.Yellow:
				Stringbuilder += ("\highlight3");
				break;
			case ConsoleColor.Red:
				Stringbuilder += ("\highlight5");
				break;
			case ConsoleColor.Blue:
				Stringbuilder += ("\highlight6");
				break;
			case ConsoleColor.Cyan:
				Stringbuilder += ("\highlight7");
				break;
			default:
				Stringbuilder += ("\highlight0");
				break;

		}


	}
}*/

static void InitStringBuilder()
{
	//prepare Stringbuilder
	Stringbuilder = "";
	Stringbuilder +=  ("\fs" + BoardSize);
	//StringBuilderSetColor = color;
	/*if (Mode == 4 && customModeC.CustomModeAble)
		Stringbuilder += ("        Score:" + Score +  @" \line\line "
		   + "                     " + Math.Round(BenchFPS.DoGetFps(), 5) + "FPS");
	else*/
		Stringbuilder += ("        Score:" + Score + " \line   High Score:" + HighScore + " \line "
		   );//+ "                     " + Math.Round(BenchFPS.DoGetFps(), 5) + "FPS");
}



static void WriteToScreen()
{
	int Margin = 5;
	char ** BoardToSide =
	{
		"Player1 Arrow Keys",
		"Left&Right = Move",
		"Down = Settings",
		"",
		"",
		"",
		""
	};
	if (Mode == 1)
		BoardToSide[3] = "Up = Shoot";

	if (TwoPlayerMode)
	{
		BoardToSide[4] = "Player2 WSAD";
		BoardToSide[5] = "A&D = Move";
		BoardToSide[6] = "S = Settings";
	}

	InitConsoleColors();

	for (int i = GameHeight - 1; i >= 0; i--)
	{
		if (Score == HighScore && !(Mode == 4 /*&& customModeC.CustomModeAble*/) && Flasher() ||
			ScoreFlashTimer <= TimerCounter && ScoreFlashTimer > TimerCounter - 10 && ColorMode == true && TimerCounter > 20 ||
			CheckPointFlasher())
		{
			//StringBuilderSetColor = ConsoleColor.Yellow;

			/*if (ScoreFlashTimer <= TimerCounter && ScoreFlashTimer > TimerCounter - 10 && ColorMode == true && TimerCounter > 20)
				StringBuilderSetColor = ConsoleColor.Magenta;

			if(CheckPointFlasher())
				StringBuilderSetColor = ConsoleColor.Cyan;*/
			Stringbuilder += ("\line  | ");
			InitConsoleColors();
		}
		else
			Stringbuilder += ("\line    ");

		for (int j = 0; j < GameWidth; j++)
		{
			//StringBuilderSetBackgroundColor = UseBoard[j, i].BackColor;

			//StringBuilderSetColor = UseBoard[j, i].Color;
			Stringbuilder += (UseBoard[j, i]);
		}

		Stringbuilder += ("\highlight0 ");

		if (Score == HighScore && !(Mode == 4/* && customModeC.CustomModeAble*/) && Flasher() ||
			ScoreFlashTimer <= TimerCounter && ScoreFlashTimer > TimerCounter - 10 && TimerCounter > 20 ||
			CheckPointFlasher())
		{
			/*StringBuilderSetColor = ConsoleColor.Yellow;
			Console.BackgroundColor = ConsoleColor.Black;

			if (ScoreFlashTimer <= TimerCounter && ScoreFlashTimer > TimerCounter - 10 && TimerCounter > 20)
				StringBuilderSetColor = ConsoleColor.Magenta;

			if (CheckPointFlasher())
				StringBuilderSetColor = ConsoleColor.Cyan;*/

			Stringbuilder += ("  |");
		}
		else
		{
			Stringbuilder += ("  ");
		}
		InitConsoleColors();

		for (int m = 0; m < Margin; m++)
		{
			Stringbuilder += (" ");
		}
		if (GameHeight - 1 - i < sizeof BoardToSide / sizeof BoardToSide[0])
			Stringbuilder += (BoardToSide[GameHeight - 1 - i]);



	}

	BoardAppend();

	WriteBoard = Stringbuilder;
}


static bool Flasher()
{
	if (TimerCounter >= FlasherCounter + 10)
	{
		FlasherCounter = TimerCounter;
		if (FlasherFlipFlop)
			FlasherFlipFlop = false;
		else
			FlasherFlipFlop = true;
	}

	return FlasherFlipFlop;
}

static void FallingObjectTimers()
{
	if (TimerCounter >= timer10 + 10)
	{
		timer10 = TimerCounter;
	}

	if (TimerCounter >= timer100 + 100)
	{
		timer100 = TimerCounter;
	}

	if (TimerCounter >= timer300 + 300)
	{
		timer300 = TimerCounter;
	}
}


static void UpdateDificultyCurve()
{
	CurveTimer += 1;
	if (CurveTimer >= 250 && BlockTick > 0)
	{
		BlockTick -= 1;
		CurveTimer = 0;
	}
}

static void StringBuilderBuild()
{

	//Turn Arrays into a String
	for (int i = GameHeight - 1; i >= 0; i--)
	{
		if (ColorMode)
		{
			if (Score == HighScore && Flasher() || ScoreFlashTimer <= TimerCounter &&
				ScoreFlashTimer > TimerCounter - 10 && TimerCounter > 20)
			{

				Stringbuilder += ("\line  | ");

			}
			else
				Stringbuilder += ("\line    ");
		}
		else
		{
			if (Score == HighScore && Flasher() || ScoreFlashTimer <= TimerCounter &&
				ScoreFlashTimer > TimerCounter - 10 && TimerCounter > 20)
			{

				Stringbuilder += ("\n  | ");

			}
			else
				Stringbuilder += ("\n    ");
		}
		for (int j = 0; j < GameWidth; j++)
		{
			Stringbuilder += (UseBoard[j, i]);
		}

		if (Score == HighScore && Flasher() || ScoreFlashTimer <= TimerCounter &&
			ScoreFlashTimer > TimerCounter - 10 && TimerCounter > 20)
		{

			Stringbuilder += (" |  ");

		}

	}

	WriteBoard = Stringbuilder;
}

static void BoardAppend()
{
	if (Mode == 1 /*|| Mode == 4 && !customModeC.CustomModeAble*/)
	{
		//StringBuilderSetColor = ConsoleColor.White;

		Stringbuilder += ("\line\line Ammo = " + Ammo + " Shields = " + Shields );

		InitConsoleColors();
	}
}

static void DeathScreen(int Place)
{
	void deathinit()
	{
		Stringbuilder = "";
		Stringbuilder += ("\fs" + BoardSize);
		//StringBuilderSetColor = ConsoleColor.Red;
		Stringbuilder += ("\line\line ");
	}
	if (ColorMode)
	{
		deathinit();
		UseBoard[Place, 0] = '#';
		StringBuilderBuild();
		TextBoxReplaceRtf(WriteBoard);
		DoSleep(700);

		deathinit();
		UseBoard[Place, 0] = '@';
		StringBuilderBuild();
		TextBoxReplaceRtf(WriteBoard);
		DoSleep(700);

		deathinit();
		UseBoard[Place, 0] = 'X';
		StringBuilderBuild();
		TextBoxReplaceRtf(WriteBoard);
		DoSleep(1400);
	}
	else
	{
		UseBoard[Place, 0] = '#';
		StringBuilderBuild();
		TextBoxReplace(WriteBoard);
		DoSleep(700);

		InitStringBuilder();//ConsoleColor.White);
		UseBoard[Place, 0] = '@';
		StringBuilderBuild();
		TextBoxReplace(WriteBoard);
		DoSleep(700);

		InitStringBuilder();//ConsoleColor.White);
		UseBoard[Place, 0] = 'X';
		StringBuilderBuild();
		TextBoxReplace(WriteBoard);
		DoSleep(1400);
	}
}

//ye old code
/*static void ChangeResolution(int ResolutionX, int ResolutionY)
{
	string FileName = @"c:\qres\qres.exe";
	string FileName2 = @"qres\qres.exe";
	try
	{
		Process proc = new Process();

		proc.StartInfo.FileName = FileName; // put full path in here
		proc.StartInfo.Arguments = ("/x " + ResolutionX + " /y " + ResolutionY).ToString(); // say
		proc.Start();
		proc.WaitForExit();
	}
	catch (System.ComponentModel.Win32Exception)
	{
		try
		{
			Process proc = new Process();

			proc.StartInfo.FileName = FileName2; // put full path in here
			proc.StartInfo.Arguments = ("/x " + ResolutionX + " /y " + ResolutionY).ToString(); // say
			proc.Start();
			proc.WaitForExit();
		}
		catch (System.ComponentModel.Win32Exception)
		{


			TextBoxWriteLine("Fullscreen Mode Error\nUnable to locate " + FileName + " or " + FileName2 +
				"\nPlease Specify the location of QRes.exe");
			TextBoxWriteLine("Press any key to continue...");
			Console.Beep();
			Console.Beep();
			//Console.ReadKey();
			FullScreen = false;
		}
	}
}*/

static void BlockHandling()
{
	//=======V====Falling=Objects====V======

	//H axis
	for (int i = 0; i < GameHeight; i++)
	{
		//W axis
		for (int j = 0; j < GameWidth; j++)
		{
			//only on block tick
			if (BlockTickCounter == 0)
			{
				if (UseBoard[j, i] == '$' || UseBoard[j, i] == '~' || UseBoard[j, i] == '^')
				{
					//testfor Bonus
					if (UseBoard[j, i] == '$')
					{
						UseBoard[j, i] = '.';

						//if not on bottom row
						if (i != 0)
						{
							UseBoard[j, i - 1] = '$';

							if ((GodMode == false) && (j == Position && i == 1 || TwoPlayerMode == true && j == Position2 && i == 1))
							{
								Score += 50;

								ScoreFlashTimer = TimerCounter;
							}
						}
					}

					//testfor PowerUp
					if (UseBoard[j, i] == '^')
					{
						UseBoard[j, i] = '.';

						//if not on bottom row
						if (i != 0)
						{
							UseBoard[j, i - 1] = '^';

							if ((GodMode == false) && (j == Position && i == 1 || TwoPlayerMode == true && j == Position2 && i == 1))
							{
								Ammo += 3;
							}
						}
					}
					if (UseBoard[j, i] == '~')
					{
						UseBoard[j, i] = '.';

						//if not on bottom row
						if (i != 0)
						{
							UseBoard[j, i - 1] = '~';

							if ((GodMode == false) && (j == Position && i == 1 || TwoPlayerMode == true && j == Position2 && i == 1))
							{
								Shields += 1;
							}
						}
					}

					/*if (UseBoard[j, i] is HalloweenBomb)
					{
						UseBoard[j, i] = '.';

						//if not on bottom row
						if (i != 0)
						{
							UseBoard[j, i - 1] = new HalloweenBomb();

							if ((j == Position && i == 1 || TwoPlayerMode == true && j == Position2 && i == 1))
							{
								for (int z = 0; z < GameHeight; z++)
								{
									for (int l = 0; l < GameWidth; l++)
									{
										if (UseBoard[l, z] == '%')
										{
											UseBoard[l, z] = '.';
										}
									}
								}
								explosiontimer = 5;
								OrangeBackground(true);
							}
						}
					}

					if (UseBoard[j, i] is Snowflake)
					{
						UseBoard[j, i] = '.';

						//if not on bottom row
						if (i != 0)
						{
							UseBoard[j, i - 1] = new Snowflake();

							if ((j == Position && i == 1 || TwoPlayerMode == true && j == Position2 && i == 1))
							{
								/*for (int z = 0; z < GameHeight; z++)
								{
									for (int l = 0; l < GameWidth; l++)
									{
										if (UseBoard[l, z] == '%')
										{
											UseBoard[l, z] = '.';
										}
									}
								}
								snowflaketimer = 50;
							}
						}
					}*/



					//move blocks
					if (UseBoard[j, i] == '%' || raadmodetimerding)//testforblock
					{
						UseBoard[j, i] = '.';

						if (i != 0)//if not on bottom row
						{

							if (j == Position && i == 2 && Shields > 0)
							{
								Shields -= 1;

								if (Shields == 0 && UseBoard[j, i - 1] == '~')
								{
									UseBoard[j, i - 1] = '.';
								}
							}
							else
								UseBoard[j, i - 1] = '%';

							//would a player die? then kill them if they are not god.
							if ((GodMode == false) && (j == Position && i == 1 ||
								TwoPlayerMode == true && j == Position2 && i == 1) || raadmodetimerding)
							{
								OnDeath(j);
							}

						}
					}
				}
				//DiscordDB.UpdatePresence();
			}//End of blocktick
		}
	}

	//add falling objects
	if (BlockTickCounter == 0)
	{
		//add Blocks or raaadmodethingies
		if (Mode > MaxMode)
			UseBoard[GetRand(0, GameWidth), GameHeight - 1] = '$';
		else
		{
			if (Mode == 4 && false)//customModeC.CustomModeAble)
			{
				/*for(int i = 0; customModeC.Blocks > i; i++)
				{
					UseBoard[GetRand(0, GameWidth), GameHeight - 1] = '%';
				}*/
			}
			else
			{
				if (snowflaketimer > 0)
					UseBoard[GetRand(0, GameWidth), GameHeight - 1] = '$';
				else
					UseBoard[GetRand(0, GameWidth), GameHeight - 1] = '%';

				if (Mode == 2)
				{
					UseBoard[GetRand(0, GameWidth), GameHeight - 1] = '%';
					UseBoard[GetRand(0, GameWidth), GameHeight - 1] = '%';
				}
			}
		}
	}



	//Power-Up stuff
	if (snowflaketimer > 0)
	{
		snowflaketimer -= 1;
	}
	if (explosiontimer > 0)
	{
		explosiontimer -= 1;
		if (explosiontimer == 1)
		{
			for (int z = 0; z < GameHeight; z++)
			{
				for (int l = 0; l < GameWidth; l++)
				{
					if (UseBoard[l, z] == '%')
					{
						UseBoard[l, z] = '.';
					}
				}
			}
			OrangeBackground(false);
		}
	}
	FallingObjectTimers();
	if (Mode == 1 /*|| Mode == 4 && !(Mode == 4 && customModeC.CustomModeAble)*/)
	{
		if (timer100 == TimerCounter && TimerCounter != 0)//add bonuspoints
			UseBoard[GetRand2(0, GameWidth), GameHeight - 1] = '$';


		if (CurveTimer == 0 && TimerCounter != 0)//add superpower
		{
			/*if (Mode == 4 && IsHolloween)
			{
				UseBoard[GetRand2(0, GameWidth), GameHeight - 1] = new HalloweenBomb();
			}
			if (Mode == 4 && IsChristmas)
			{
				UseBoard[GetRand2(0, GameWidth), GameHeight - 1] = new Snowflake();
			}
			else if (PowerFliper)
			{
				UseBoard[GetRand2(0, GameWidth), GameHeight - 1] = '^';
				PowerFliper = false;
			}
			else if (!PowerFliper)
			{
				UseBoard[GetRand2(0, GameWidth), GameHeight - 1] = '~';
				PowerFliper = true;
			}
		}


		//Move Shot
		/*if (shotc.IsAlive)
		{
			//kill block that is on shot
			if (UseBoard[shotc.X, shotc.Y] == '%' && shotc.IsAlive)
			{
				shotc.IsAlive = false;
				UseBoard[shotc.X, shotc.Y] = '.';
			}

			//move shot
			if (shotc.Y != 0)
				UseBoard[shotc.X, shotc.Y] = '.';
			if (shotc.Y < GameHeight - 1 && shotc.IsAlive)
			{
				shotc.Y += 1;

				//kill block that shot moved onto
				if (UseBoard[shotc.X, shotc.Y] == '%')
				{
					shotc.IsAlive = false;
					UseBoard[shotc.X, shotc.Y] = '.';
				}

				if (shotc.IsAlive)
					UseBoard[shotc.X, shotc.Y] = new Shot();
			}
			else
				shotc.IsAlive = false;
		}
		if (shotc.ShootCooldown > 0)
		{
			shotc.ShootCooldown -= 1;
		}*/
	}
	if (TimerCounter == 1500 && Mode > MaxMode)
	{
		raadmodetimerding = true;
	}

	//Update Block Tick
	if (BlockTickCounter > 0)
		BlockTickCounter = BlockTickCounter - 1;
	else
		BlockTickCounter = BlockTick;


}   //=====/\===================/\======

static void OnDeath(int j)
{
	/*music.PauseMusic();

	if (MuteSfx == false)
		music.DieNoise();

	// log highscore
	if (loadsuccess && Score == HighScore && !(Mode == 4 && customModeC.CustomModeAble))
	{
		if (Mode == 0)
			write.ToThisTxt(1, HighScore.ToString());
		else if (Mode == 1)
			write.ToThisTxt(25, HighScore.ToString());
		else if (Mode == 2)
			write.ToThisTxt(33, HighScore.ToString());
		else if (Mode == 3)
			write.ToThisTxt(3, HighScore.ToString());
		else if (Mode == 4 && IsHolloween)
			write.ToThisTxt(27, HighScore.ToString());
		else if (Mode == 4 && IsChristmas)
			write.ToThisTxt(35, HighScore.ToString());
	}*/

	// setting deathscreen
	void OnPostDeathScreen()
	{
		Stringbuilder = "";

		/*if (IsHolloween)
		{
			StringBuilderSetColor = ConsoleColor.DarkMagenta;
			Stringbuilder += ("\fs" + BoardSize + " ");
		}
		else if (IsThanksgiving)
		{
			StringBuilderSetColor = ConsoleColor.DarkGray;
			Stringbuilder += ("\fs" + BoardSize + " ");
		}
		else
		{
			StringBuilderSetColor = ConsoleColor.Cyan;
			Stringbuilder += ("\fs" + BoardSize + " ");
		}*/
	}
	// add endline
	void LineBreak()
	{
		if (ColorMode)
			Stringbuilder += ("\line ");
		else
			Stringbuilder += ("\n");
	}

	// kill the bloody loser
	if (TwoPlayerMode)
	{
		if (j == Position && j == Position2)
		{
			DeathScreen(Position);
			OnPostDeathScreen();
			Stringbuilder += ("Both Players Died!");
		}
		else if (j == Position2)
		{
			DeathScreen(Position2);
			OnPostDeathScreen();
			Stringbuilder += ("Player2 Died! (" + Avatar2 + ")");
		}
		else
		{
			DeathScreen(Position);
			OnPostDeathScreen();
			Stringbuilder += ("Player1 Died! (" + Avatar + ")");
		}
	}
	else
	{
		DeathScreen(Position);
		OnPostDeathScreen();
		Stringbuilder += ("You Died!");
	}
	LineBreak();



	// add the results
	Stringbuilder += ("Your Score: " + Score);
	LineBreak();
	if (Score >= HighScore)
	{
		Stringbuilder += ("*New High Score!*");
		/*if (ColorMode)
			StringBuilderSetColor = ConsoleColor.Yellow;

		if (IsHolloween)
		{
			StringBuilderSetColor = ConsoleColor.DarkMagenta;
		}
		else if (IsThanksgiving)
		{
			StringBuilderSetColor = ConsoleColor.DarkGray;
		}
		else
		{
			StringBuilderSetColor = ConsoleColor.Cyan;
		}*/
		LineBreak();
	}
	else
	{
		Stringbuilder += ("High Score: " + HighScore);
		LineBreak();

	}



	Stringbuilder += ("Playing: ");
	if (Mode == 0)
	{
		Stringbuilder += ("Classic Mode");
	}
	else if (Mode == 1)
	{
		Stringbuilder += ("Normal Mode");
	}
	else if (Mode == 2)
	{
		Stringbuilder += ("Expert Mode");
	}
	else if (Mode == 3)
	{
		Stringbuilder += ("Two Player Mode");
	}
	else if (Mode == 4 && IsHolloween)
	{
		Stringbuilder += ("Halloween Event");
	}
	else if (Mode == 4 && IsChristmas)
	{
		Stringbuilder += ("Winter Event");
	}
	else if (Mode == 4)
	{
		Stringbuilder += ("Custom Mode");
	}
	else
	{
		Stringbuilder += ("raaad mode");
	}
	if((Mode == 0 || Mode == 2) && UseCheckpoints)
	{
		if (Score > 500)
			LineBreak();
		if (Score > 1000)
			Stringbuilder += ("Checkpoint Achieved: 1000");
		else if (Score > 500)
			Stringbuilder += ("Checkpoint Achieved: 500");
	}

	LineBreak();
	LineBreak();
	Stringbuilder += ("Press Up to Continue");
	LineBreak();
	Stringbuilder += ("Press Down to Quit");

	WriteBoard = Stringbuilder;
	if (ColorMode)
		TextBoxReplaceRtf(WriteBoard);
	else
		TextBoxReplace(WriteBoard);


	if (loadsuccess)
	{
		char *modeAppend;
		if (Mode == 0)
			modeAppend = "C";
		else if (Mode == 1)
			modeAppend = "N";
		else if (Mode == 2)
			modeAppend = "H";
		else if (Mode == 3)
			modeAppend = "T";
		else if (Mode == 4 && (IsHolloween))
			modeAppend = "HW-EVNT";
		else if (Mode == 4 && (IsChristmas))
			modeAppend = "CHR-EVNT";
		//else if (Mode == 4 && (Mode == 4 || customModeC.CustomModeAble))
		//	modeAppend = "CSTM";
		else
			modeAppend = "R";
		//write.ToThisTxt(31, SecretCode);
		GameInitialized();
	}
	else
	{
		SecretCode = "WARNING: Settings.txt not loaded correctly. Please ensure that Settings.txt and Dodgeblock.exe are in the same directory and not zipped!";
		GameInitialized();
	}

	//DiscordDB.UpdatePresence();

	bool go = true;
	while (go)
	{
		if (IsKeyDown(0))
		{
			go = false;
		}
		if (IsKeyDown(1))
		{
			go = false;
			Refresh = false;
			Quitting = true;


		}
		DoSleep(80);
	}

	int savescore = TimerCounter;
	ResetGame();
	DoCheckPoints(savescore);

	//if (MuteMusic == false)
	//	music.ResumeMusic();
	InitConsoleColors();
}

static void ResetGame()
{
	Tick = 60;
	if (Mode > MaxMode)
		Tick = 10;
	if (Mode < 4)
		snowflaketimer = 0;
	Score = 0;
	Ammo = 0;
	Shields = 0;
	CurveTimer = 0;
	GameWidth = 12;
	GameHeight = 7;
	BlockTick = BlockTickBase;
	for (int i = 0; i < GameHeight; i++)
	{
		for (int j = 0; j < GameWidth; j++)
		{
			BackBoard[j, i] = '.';
		}
	}
	//UseBoard = BackBoard.Clone() as char[,];
	TimerCounter = 0;
	ScoreFlashTimer = 0;
	timer10 = 0;
	timer100 = 0;
	timer300 = 0;
	raadmodetimerding = false;

	//customModeC.SettingsChanged();
}

static void DoCheckPoints(int savescore)
{
	if ((Mode == 0 || Mode == 2) && UseCheckpoints)
	{
		if (savescore >= 1000)
		{
			Score = 1000;
			TimerCounter = 1000;
			BlockTick = BlockTickBase - 4;
		}
		else if (savescore >= 500)
		{
			Score = 500;
			TimerCounter = 500;
			BlockTick = BlockTickBase - 2;
		}
	}
}

static bool CheckPointFlasher()
{
	if ((Mode == 0 || Mode == 2) && UseCheckpoints)
		if ((Score >= 1000 && Score <= 1010) || (Score >= 500 && Score <= 510))
			return true;
	return false;
}

//also ye old code
/*static void SettingsMenu()
{
	TextBoxReplace("");
	music.PauseMusic();

	Console.BackgroundColor = ConsoleColor.DarkBlue;
	Console.ForegroundColor = ConsoleColor.White;

	TextBoxWriteLine(
"###########Settings###########\n" +
"#                            #\n" +
"#    Right = Mute/Unmute     #\n" +
"#    Left =  Quit & Quit     #\n" +
"#    Up = Return to Game     #\n" +
"#                            #\n" +
"##############################\n");
	int ButtonCooldown = 0;
	bool NotReturned = true;

	while (NotReturned)
	{
		if (IsKeyDown(2))
		{
			Refresh = false;
			NotReturned = false;
			InitConsoleColors();
			Console.ForegroundColor = ConsoleColor.Red;
		}

		if (IsKeyDown(3))
		{
			if (MuteMusic == true && ButtonCooldown == 0)
			{
				TextBoxWriteLine("Unmuted");
				MuteMusic = false;
				ButtonCooldown = 5;
			}
			else if (MuteMusic == false && ButtonCooldown == 0)
			{
				TextBoxWriteLine("Muted");
				MuteMusic = true;
				ButtonCooldown = 5;
			}

		}

		if (IsKeyDown(0))
		{
			NotReturned = false;
			InitConsoleColors();
		}

		if (ButtonCooldown > 0)
			ButtonCooldown = ButtonCooldown - 1;

		System.Threading.DoSleep(60);
	}
	if (MuteMusic == false)
		music.ResumeMusic();
	TextBoxReplace("");

}*/

static void StartScreen()
{
	TextBoxWriteLine("CONGRATS! starting.");
	DoSleep(1000);

	if(IsChristmas)
		Cutscene();

	bool NotStarted = true;
	while (NotStarted)
	{
		if (IsHolloween)
		{
			TextBoxReplace("        Version " + Version + "  \n" +
		"################################\n" +
		"#.._____.....Happy......_____..#\n" +
		"#./.....\\.@@@@@@@@@@@@ /.....\\.#\n" +
		"#|.0...0.|@ Press Up @|.0...0.|#\n" +
		"#.\\..A../.@@@@@@@@@@@@.\\..A../.#\n" +
		"#..|||||...Halloween!...|||||..#\n" +
		"################################\n" +
		"\n       (c) othello7 2020");
		}
		/*else if (IsThanksgiving)
		{
			TextBoxReplace("        Version " + Version + "  \n" +
		"################################\n" +
	    "#.|\....Happy.......@ Press Up #" + "\n" +
		"#.| `-._,=''' =,....@@@@@@@@@@@#\n" +
	    "#.\    //o.-)-.\.Thanksgiving!.#" + "\n" +
	    "#..\   ||/:/:\:\-..............#" + "\n" +
	    "#...`. \\\:\:/:/ o_),..........#" + "\n" +
	    "#.....`->>> 8888 >; (_)o.......#" + "\n" +
		"################################\n" +
		"\n       (c) othello7 2020");

		}
		else if (IsChristmas)
		{
			TextBoxReplace("        Version " + Version + "  \n" +
		@"################################" + "\n" +
		@"#..._._................../*\...#" + "\n" +
		@"#..(_X_)..@@@@@@@@@@@@..// \\..#" + "\n" +
		@"#.[  |  ].@ Press Up @./// \\\.#" + "\n" +
		@"#.[--+--].@@@@@@@@@@@@//// \\\\#" + "\n" +
		@"#.[  |  ]................| |...#" + "\n" +
		@"################################" + "\n" +
		"\n       (c) othello7 2020");
		}*/
		else
		{
			TextBoxReplace("        Version " + Version + "  \n" +
					"################################" + "\n" +
					"#..............................#" + "\n" +
					"#.........@@@@@@@@@@@@.........#" + "\n" +
					"#.........@ Press Up @.........#" + "\n" +
					"#.........@@@@@@@@@@@@.........#" + "\n" +
					"#..............................#" + "\n" +
					"################################" + "\n" +
					"\n    (c) othello7 2019-2021");
			//Start Screen
			//Animations.PlayAnimatedStartScreen();
		}

		if (IsKeyDown(0))
		{
			NotStarted = false;
		}
		DoSleep(150);
	}
	TextBoxReplace("");
	//music.PlayMusic();
}

static void ConvertTwoPlayerMode()
{
	if (Mode == 3)
		TwoPlayerMode = true;
	else
		TwoPlayerMode = false;
}

static char* Boolconvert(bool setting)
{
	char *str;
	if (setting)
		str = "1";
	else
		str = "0";
	return str;
}
static bool Boolconvert(char* setting)
{
	bool boolean;
	if (setting == "1")
		boolean = true;
	else
		boolean = false;
	return boolean;
}
static void ImportHighScore()
{/*
	if (loadsuccess)
	{
		try
		{
			string[] Settings = File.ReadAllLines(settingslocation);

			if (Mode == 0)
			{
				HighScore = Convert.ToInt32(Settings[1]);
			}
			else if (Mode == 1)
			{
				HighScore = Convert.ToInt32(Settings[25]);
			}
			else if (Mode == 2)
			{
				HighScore = Convert.ToInt32(Settings[33]);
			}
			else if (Mode == 3)
			{
				HighScore = Convert.ToInt32(Settings[3]);
			}
			else if (Mode == 4 && IsHolloween)
			{
				HighScore = Convert.ToInt32(Settings[27]);
			}
			else if (Mode == 4 && IsChristmas)
			{
				HighScore = Convert.ToInt32(Settings[35]);
			}

		}
		catch (Exception ex)
		{
			TextBoxWriteLine("Could not import settings from " + settingslocation + " " + ex);
			Console.Beep();
			Console.Beep();
			DoSleep(777);
		}
	}*/
}

//AGAIN whats with all this elden code?
/*static string Code(string Input)
{
	char[] charArray = Input.ToCharArray();

	int[] intarray = new int[charArray.Length];

	for (int i = 0; i < charArray.Length; i++)
	{
		intarray[i] = (int)charArray[i] + 17;

		charArray[i] = (char)intarray[i];
	}

	string b = new string(charArray);
	return b;
}*/

static void Cutscene()
{/*
	ZoomTextBox(ZoomTextBox() / 1.5f, true);


	int FrameCounter = 0;
	bool NotSkipped = true;
	while (NotSkipped)
	{
		TextBoxReplace(Playmation[FrameCounter] +
	"\nAnimated by The King of Ducks \nPress any (arrow) key to skip");

		if (FrameCounter == Playmation.Count - 1)
			NotSkipped = false;
		else
			FrameCounter = FrameCounter + 1;

		if (IsKeyDown(0) ||
			IsKeyDown(1) ||
			IsKeyDown(2) ||
			IsKeyDown(3))
		{
			NotSkipped = false;
		}
		if (FrameCounter < 21)
			DoSleep(200);
		if (FrameCounter < 33)
			DoSleep(100);
		DoSleep(100);
	}
	ZoomTextBox(ZoomTextBox() * 1.5f);*/
}

/* this is
 *the plugin entry point */
enum plugin_status plugin_start(const void* parameter)
{
    /* if you don't use the parameter, you can do like
       this to avoid the compiler warning about it */
    (void)parameter;
    //rb->keyclick_click();
    Game();

    /* "rb->" marks a plugin api call. Rockbox offers many of its built-in
     * functions to plugins */
    /* now go ahead and have fun! */
    rb->splash(HZ*2, "Hello Rockbox~!");

    /* tell Rockbox that we have completed successfully */
    return PLUGIN_OK;
}
