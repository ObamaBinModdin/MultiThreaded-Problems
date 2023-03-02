#include "MatrixProblem.h"

// Private class mutex initialization.
pthread_mutex_t MatrixProblem::mutex = PTHREAD_MUTEX_INITIALIZER;
// Private class 2-D vector that will act as the matrix.
std::vector<std::vector<int>> MatrixProblem::matrix;
// Private class initialization of a counter to track the current tied streak.
int MatrixProblem::tiedStreak = 0;
// Flag for if the game is over.
bool MatrixProblem::done = false;

// Only public method. Constructor to run and solve the matrix problem. Pass in an int of the size of the matrix squared.
MatrixProblem::MatrixProblem(int& elementsNum)
{
	// Empties the matrix.
	matrix.clear();

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
	
	std::cout << "\nStarting...\n\n\n\n";
	
	// Sets the seed based on time. Lowers likelihood of getting the same seed each execution.
	srand(time(NULL));
	
	// Initialize matrix for each value to be 1 or 0.
	for (int row = 0; row < elementsNum; ++row)
	{
		std::vector<int> matrixCol;
		
		for (int col = 0; col < elementsNum; ++col)
		{	
			matrixCol.push_back(rand() % 2);
		}
		
		matrix.push_back(matrixCol);
	}
	
	// Declares thread handler and the size (number of threads).
	pthread_t ptid[threadCount + 1];
	
	// Prints the initial matrix to console.
	MatrixProblem::printMatrix();
	
	pthread_create(&ptid[threadCount], 0, MatrixProblem::supervisor, NULL);
	
	// Creates threads that runs flipper().
	for (int count = 0; count < threadCount; ++count)
	{
		pthread_create(&ptid[count], 0, MatrixProblem::flipper, NULL);
	}
	
	// Join the threads.
	for (int count = 0; count < threadCount; ++count)
	{
		pthread_join(ptid[count], NULL);
	}
	
	pthread_join(ptid[threadCount], NULL);
	
	std::cout << std::endl;
	
	MatrixProblem::done = false;
	
	MatrixProblem::printMatrix();
	
	if (tiedStreak > 100000)
	{
		std::cout << "\nStalled game.\n";
	}
	
	// Prints that the matrix problem is done.
	std::cout << "\nDone!\n\n";
	
	int userInput;
	do
	{
		std::cout << "Press the enter key to continue...\n";
		userInput = getch();
	}while(userInput != '\n' && userInput != KEY_ENTER);
}

// Flips a index in the matrix.
void* MatrixProblem::flipper(void* arg)
{
	while(done == false)
	{
		// Generate a random row and column.
		int row = rand() % matrix.size();
		int col = rand() % matrix.size();
	
		// Set zero count to 0, corner to false, and side to false.
		int zeroCount = 0;
		bool corner = false;
		bool side = false;
	
	
		// If corner.
		if ((row == (int)matrix.size() - 1 && col == (int)matrix[row].size() - 1) || (row == (int)matrix.size() - 1 && col == 0) || (row == 0 && col == 0) || (row == 0 && col == 	(int)matrix[row].size() - 1))
		{
			corner = true;
	
			// Bottom-right
			if (row == (int)matrix.size() - 1 && col == (int)matrix[row].size() - 1)
			{
				if (matrix[(int)matrix.size() - 1][(int)matrix[row].size() - 2] == 0) zeroCount++;
			
				if (matrix[(int)matrix.size() - 2][(int)matrix[row].size() - 2] == 0) zeroCount++;
			
				if (matrix[(int)matrix.size() - 2][(int)matrix[row].size() - 1] == 0) zeroCount++;
			}
			// Bottom-left
			else if (row == (int)matrix.size() - 1 && col == 0)
			{
				if (matrix[(int)matrix.size() - 1][1] == 0) zeroCount++;
			
				if (matrix[(int)matrix.size() - 2][0] == 0) zeroCount++;
			
				if (matrix[(int)matrix.size() - 2][1] == 0) zeroCount++;
			}
			// Top-left
			else if (row == 0 && col == 0)
			{
				if (matrix[0][1] == 0) zeroCount++;
			
				if (matrix[1][1] == 0) zeroCount++;
			
				if (matrix[1][0] == 0) zeroCount++;
			}
			// Top-right
			else
			{
				if (matrix[0][(int)matrix[row].size() - 2] == 0) zeroCount++;
			
				if (matrix[1][(int)matrix[row].size() - 2] == 0) zeroCount++;
			
				if (matrix[1][(int)matrix[row].size() - 1] == 0) zeroCount++;

			}
		}
		// If side but not corner.
		else if ((row == (int)matrix.size() - 1 && col != (int)matrix[row].size() - 1 && col != 0) || (row == 0 && col != 0 && col != (int)matrix[row].size() - 1) || (row != 0 && row != (int)matrix.size() - 1 && col == 0) || (row != 0 && row != (int)matrix.size() - 1 && col == (int)matrix[row].size() - 1))
		{
			side = true;
	
			// Bottom
			if (row == (int)matrix.size() - 1 && col != (int)matrix[row].size() - 1 && col != 0)
			{
				if (matrix[row][col + 1] == 0) zeroCount++;
			
				if (matrix[row][col - 1] == 0) zeroCount++;
			
				if (matrix[row - 1][col - 1] == 0) zeroCount++;
			
				if (matrix[row - 1][col] == 0) zeroCount++;
			
				if (matrix[row - 1][col + 1] == 0) zeroCount++;
			}
			// Left
			else if (row != 0 && row != (int)matrix.size() - 1 && col == 0)
			{
				if (matrix[row - 1][col] == 0) zeroCount++;
			
				if (matrix[row - 1][col + 1] == 0) zeroCount++;
			
				if (matrix[row][col + 1] == 0) zeroCount++;
			
				if (matrix[row + 1][col + 1] == 0) zeroCount++;
			
				if (matrix[row + 1][col] == 0) zeroCount++;
			}
			// Right
			else if (row != 0 && row != (int)matrix.size() - 1 && col == (int)matrix[row].size() - 1)
			{
				if (matrix[row - 1][col] == 0) zeroCount++;
			
				if (matrix[row - 1][col - 1] == 0) zeroCount++;
			
				if (matrix[row][col - 1] == 0) zeroCount++;
			
				if (matrix[row + 1][col - 1] == 0) zeroCount++;
			
				if (matrix[row + 1][col] == 0) zeroCount++;
			}
			// Top
			else
			{
				if (matrix[row][col + 1] == 0) zeroCount++;
			
				if (matrix[row + 1][col + 1] == 0) zeroCount++;
			
				if (matrix[row + 1][col] == 0) zeroCount++;
			
				if (matrix[row + 1][col - 1] == 0) zeroCount++;
			
				if (matrix[row][col - 1] == 0) zeroCount++;
			}
		}
		// Any other location.
		else
		{
			if (matrix[row][col + 1] == 0) zeroCount++;
			
			if (matrix[row - 1][col + 1] == 0) zeroCount++;
			
			if (matrix[row - 1][col] == 0) zeroCount++;
			
			if (matrix[row - 1][col - 1] == 0) zeroCount++;
			
			if (matrix[row][col - 1] == 0) zeroCount++;
		
			if (matrix[row + 1][col - 1] == 0) zeroCount++;
		
			if (matrix[row + 1][col] == 0) zeroCount++;
		
			if (matrix[row + 1][col + 1] == 0) zeroCount++;
		}
	
		// If the index was a corner.
		if (corner == true)
		{
			// Lock threads
			pthread_mutex_lock(&mutex);
		
			// Saves current index value.
			int previous = matrix[row][col];
		
			// Determines value to flip the index based on surrounding values.
			if (zeroCount >= 2) matrix[row][col] = 0;
			else matrix[row][col] = 1;
		
			std::cout << std::endl;
		
			// A flip occurred.
			if (previous != matrix[row][col])
			{
				std::cout << "[" << pthread_self() << "] successfully flipped row: " << row + 1 << ", column: " << col + 1 << std::endl;
				MatrixProblem::printMatrix();
			}
			// No change happened.
			else
			{
				std::cout << "[" << pthread_self() << "] failed to flip row: " << row + 1 << ", column: " << col + 1 << std::endl;
			}
		
			// Unlock threads	
			pthread_mutex_unlock(&mutex);
		}
		else if (side == true)
		{
			// Lock threads
			pthread_mutex_lock(&mutex);
		
			// Saves current index value.	
			int previous = matrix[row][col];
		
			// Determines value to flip the index based on surrounding values.	
			if (zeroCount >= 3) matrix[row][col] = 0;
			else matrix[row][col] = 1;
		
			std::cout << std::endl;
		
			// A flip occurred.
			if (previous != matrix[row][col])
			{
				std::cout << "[" << pthread_self() << "] successfully flipped row: " << row + 1 << ", column: " << col + 1 << std::endl;
				MatrixProblem::printMatrix();
			}
			// No change happened.
			else
			{
				std::cout << "[" << pthread_self() << "] failed to flip row: " << row + 1 << ", column: " << col + 1 << std::endl;
			}
		
			// Unlock threads	
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			// Lock threads
			pthread_mutex_lock(&mutex);
		
			// Saves current index value.	
			int previous = matrix[row][col];
		
			// Determines value to flip the index based on surrounding values.
			if (zeroCount >= 5)matrix[row][col] = 0;
			else if (zeroCount < 4) matrix[row][col] = 1;
		
			std::cout << std::endl;
		
			// A flip occurred.
			if (previous != matrix[row][col]) 
			{
				std::cout << "[" << pthread_self() << "] successfully flipped row: " << row + 1 << ", column: " << col + 1 << std::endl;
				MatrixProblem::printMatrix();
			
				// Resets streak to 0.
				tiedStreak = 0;
			}	
			// No change happened.
			else
			{
				// Increase streak count.
				tiedStreak++;
			
				std::cout << "[" << pthread_self() << "] failed to flip row: " << row + 1 << ", column: " << col + 1 << std::endl;
			}
		
			// Unlock threads	
			pthread_mutex_unlock(&mutex);
		}
	}
	
	return arg;
}

// Check if the matrix is all one value. Returns false if not. True otherwise.
bool MatrixProblem::checkMatrix(std::vector<std::vector<int>>& matrix)
{
	for (int row = 0; row < (int)matrix.size(); ++row)
	{
		for (int col = 0; col < (int)matrix[row].size(); ++col)
		{
			if (matrix[row][col] != matrix[0][0])
			{
				return false;
			}
		}
	}
	
	return true;
}

// Prints matrix to console.
void MatrixProblem::printMatrix()
{
	for (int row = 0; row < (int)matrix.size(); ++row)
	{
		for (int col = 0; col < (int)matrix[row].size(); ++col)
		{
			std::cout << matrix[row][col] << " ";
		}
		
		std::cout << std::endl;
	}
}

// Thread function to check if the game is over.
void* MatrixProblem::supervisor(void * arg)
{
	while (MatrixProblem::done == false)
	{
		// Lock threads
		pthread_mutex_lock(&mutex);
	
		MatrixProblem::done = MatrixProblem::checkMatrix(matrix);
		
		if (tiedStreak > 100000)
		{
			MatrixProblem::done = true;
		}
		
		// Unlock threads	
		pthread_mutex_unlock(&mutex);
	}
	
	return arg;
}
