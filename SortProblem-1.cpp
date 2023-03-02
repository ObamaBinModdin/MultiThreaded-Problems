#include "SortProblem.h"

// Private class mutex initialization.
pthread_mutex_t SortProblem::mutex = PTHREAD_MUTEX_INITIALIZER;
std::vector<int> SortProblem::array;
bool SortProblem::arraySorted = false;

SortProblem::SortProblem(int& elementsNum)
{
	array.clear();
	
	// Declaration of thread count that can be modified by the user.
	int threadCount;
	
	// Infinite loop that continues until a valid thread count is entered in terminal.
	while(1)
	{
		std::cout << "Enter the number of threads you would like to use: ";
		
		try
		{
			std::cin >> threadCount;
		}
		// If an exception is raised then return to the beginning of the loop.
		catch(...)
		{
			std::cout << "Enter a number.\n";
			continue;
		}
		
		// Only accepts positive integers for thread count.
		if (threadCount < 1)
		{
			std::cout << "Enter a positive integer.\n";
			continue;
		}
		else
		{
			break;
		}
		
		std::cout << std::endl;
	}
	
	if (threadCount > 100) threadCount = 100;
	
	// Sets the seed based on time. Lowers likelihood of getting the same seed each execution.
	srand(time(NULL));
	
	// Fills the array with random values.
	for (int index = 0; index < elementsNum; ++index)
	{	
		// Generates a number between -1000 and 1000 before pushing it into the array.
		array.push_back(-1000 + rand() % (1000 - -1000 + 1));
	}
	
	// Declares the number of threads plus one for supervisor.
	pthread_t ptid[threadCount + 1];
	
	// Prints starting array.
	SortProblem::printArray();
	
	// Create thread to check if sorted.
	pthread_create(&ptid[threadCount], 0, SortProblem::supervisor, NULL);

	// Creates threads that runs sort().
	for (int count = 0; count < threadCount; ++count)
	{
		pthread_create(&ptid[count], 0, SortProblem::sort, NULL);
	}
	
	// Join the threads.
	for (int count = 0; count < threadCount; ++count)
	{
		pthread_join(ptid[count], NULL);
	}
	
	// Join supervisor thread.
	pthread_join(ptid[threadCount], NULL);

	std::cout << std::endl;

	SortProblem::arraySorted = false;

	// Print final state of the array.
	SortProblem::printArray();
	
	// Print "Sorted!".
	std::cout << "\nSorted!\n";
	
	int userInput;
	do
	{
		std::cout << "Press the enter key to continue...\n";
		userInput = getch();
	}while(userInput != '\n' && userInput != KEY_ENTER);
}

// Balances the array based on index (Thread function).
void* SortProblem::sort(void* arg)
{
	while (SortProblem::arraySorted == false)
	{
		// Lock threads
		pthread_mutex_lock(&mutex);
	
		// Generate a random index.
		int index = rand() % array.size();
		
		int leftIndex = 0;
		
		std::cout << "[" << pthread_self() << "] index value is " << array[index] << std::endl;
		std::cout << "[" << pthread_self() << "] looking for a value greater than " << array[index] << " to the left of " << array[index] << std::endl;
		
		while (leftIndex < index)
		{
			if (array[leftIndex] > array[index])
			{
				std::cout << "[" << pthread_self() << "] is swapping " << array[index] << " and " << array[leftIndex] << std::endl;
				std::swap(array[index], array[leftIndex]);
				index = leftIndex;
					
				SortProblem::printArray();
			}
			else
			{
				leftIndex++;
			}
		}
		
		std::cout << std::endl;
	
		// Unlock threads	
		pthread_mutex_unlock(&mutex);
		
		struct timespec remaining, request = { 0, 100 };
		
		// Sleeping for 100 nanoseconds so other threads can unlock.
		nanosleep(&request, &remaining);
	}
	
	return arg;
}

// Returns true if array is sorted. Otherwise returns false.
bool SortProblem::checkSort()
{
	for (int index = 0; index < (int)array.size() - 1; ++index)
	{
		if (array[index] > array[index + 1]) return false;
	}
	
	return true;
}

// Prints array to console.
void SortProblem::printArray()
{
	for (int index = 0; index < (int)array.size(); ++index)
	{
		std::cout << array[index] << " ";
	}
	std::cout << std::endl;
}

// Supervisor thread function to check for sorted array.
void* SortProblem::supervisor(void* arg)
{
	while (arraySorted == false)
	{	
		// Lock threads
		pthread_mutex_lock(&mutex);
	
		arraySorted = SortProblem::checkSort();
		
		// Unlock threads	
		pthread_mutex_unlock(&mutex);
	}
	
	return arg;
}
