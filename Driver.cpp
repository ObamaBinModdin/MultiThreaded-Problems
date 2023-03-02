#include "MatrixProblem.h"
#include "SortProblem.h"

#include <iostream>
#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>


int main(int argc, char** argv)
{
	initscr();

	keypad(stdscr, TRUE);
	
	// int variable to store the number of elements.
	int elementsNum;
	
	// If no parameter is passed in then default elementsNum to 100.
	if (argc == 1)
	{
		elementsNum = 10;
	}
	// Enters statement if one and only one parameter is passed in.
	else if (argc == 2)
	{
		// Tries to cast the parameter to an int.
		try
		{
			elementsNum = std::stoi(argv[1]);
			
			if (elementsNum < 0) throw std::invalid_argument("Number less than zero.");
		}
		// If any exception happens.
		catch(...)
		{
			clear();
			printw("Passed in value must be a positive integer.\n");
			refresh();
			getch();
			endwin();
			return 1;
		}
	}
	// Enter statement if more than one parameter is passed in.
	else
	{
		clear();
		printw("Pass in the number of elements as the only parameter.\n");
		refresh();
		getch();
		endwin();
		return 1;
	}
	
	int userInput;
	do
	{
		clear();
		printw("Press the enter key to continue...");
		userInput = getch();
	}while(userInput != '\n' && userInput != KEY_ENTER);
	clear();
	
	// Infinite loop of selecting menus and running problems until '3' is entered.
	while(1)
	{
		clear();
		printw("1: Matrix flipper.\n");
		printw("2: Array sorter.\n");
		printw("3: Exit.\n\n");
		
		
		printw("Enter an option: ");
		refresh();
	
		// User is able to enter a char.
   		char userMenuSelectionString = getch();
		
		// Exit flag for if '3' is entered.
		bool exitFlag = false;
		
		// Menu selection based on the firstr character of the string.
		switch (userMenuSelectionString)
		{
			// Start problem 1.
			case '1':
			{
				endwin();
				MatrixProblem game = MatrixProblem(elementsNum);
				initscr();
				break;
			}
			// Start problem 2.
			case '2':
			{
				endwin();
				SortProblem arrayProblem = SortProblem(elementsNum);
				initscr();
				break;
			}
			// Flip exitFlag to true.
			case '3':
			{
				clear();
				printw("Exiting...\n");
				refresh();
				sleep(1);
				for (int timer = 5; timer > 0; --timer)
				{
					clear();
					printw("Exiting in %d", timer);
					refresh();
					sleep(1);
				}
				
				exitFlag = true;
				break;
			}
			// If no valid character is entered.
			default:
			{
				clear();
				printw("%c is an invalid selection.\n", userMenuSelectionString);
				refresh();
				sleep(2);
				continue;
			}
		}
		
		// Break from loop if exitFlag is true.
		if (exitFlag) break;
	}
	
	endwin();
	
	return 0;
}
