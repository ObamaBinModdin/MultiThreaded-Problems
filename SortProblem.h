#pragma once

#include <pthread.h>
#include <iostream>
#include <vector>
#include <time.h>
#include <ncurses.h>

class SortProblem
{
	public:
		SortProblem(int& elementsNum);
	
	private:
		// Mutex to lock threads.
		static pthread_mutex_t mutex;
		// Deque to hold the array.
		static std::vector<int> array;
		// Bool to check that acts as a flag if array was sorted.
		static bool arraySorted;
		
		// Thread function to throw lower or equal values to the index value to the left and throws greater values to the right.
		static void* sort(void* arg);
		// Returns true if array is sorted. Otherwise returns false.
		static bool checkSort();
		// Prints the array to console.
		static void printArray();
		// Supervisor thread function to check for sorted array.
		static void* supervisor(void* arg);
};
