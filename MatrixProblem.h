#pragma once

#include <pthread.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <unistd.h>
#include <ncurses.h>


class MatrixProblem
{
	public:
		// Constructor to run the matrix problem and solve it.
		MatrixProblem(int& elementsNum);

	private:
		// Mutex to lock threads.
		static pthread_mutex_t mutex;
		// 2-D vector to act as the matrix.
		static std::vector<std::vector<int>> matrix;
		// Keeps track of the number of times the matrix has not changed values due to a tie.
		static int tiedStreak;
		// Flag for if the game is over.
		static bool done;
		
		// Flips value. To be called by threads.
		static void* flipper(void* arg);
		// Returns false if matrix is not all one value. True otherwise.
		static bool checkMatrix(std::vector<std::vector<int>>& matrix);
		// Prints matrix to console.
		static void printMatrix();
		// Thread function to check if the game is over.
		static void* supervisor(void * arg);
};
