/*
* Anthony Kottke
* 12 October 2018
* 
* This program will solve a system of equations containing 
* up to 100 variables. To find accurate answers, you must
* provide a file named "matrixData.txt" with all of the
* coeffecients of the variables and the results seperated by a space, and
* all of the equations on a new line. You must supply an equal number of equations
* as variables you wish to solve for. (A 5 variable system must
* have 5 equations etc.) The answers for each variable are printed
* to the screen as well as to a file named "GaussianResults.txt
*
* Generates variable answers to .01 accuracy.
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include <cmath>
#include <string.h>

using namespace std;

// reads data and puts it into 2-d array
void getData();
// ensures that for all i's matrix[i][i] is a nonzero
void formatMatrix();
// if matrix[int][int] == 0 this function will alter the equation so that
// matrix[int][int] != 0
void rowFix(int);
// runs through the matrix 2d array so that all coeffecients before
// the leading coeffecient are 0. (matrix[1][0] = 0, matrix [1][1] != 0)
void factorCoef();
// beggining with the last equation (matrix[rows-1][0 -> cols -1]
// calculates the value of the variables. Uses factorAnswers()
void getAnswers();
// More math that returns the value of the variables to getAnswers()
double factorAnswers(double, int);
// Simple function that prints the results to screen and file
// titled "GaussianResults.txt"
void supplyAnswers();

// holds the final number of rows in collumns, *NOT INDEXED FROM 0*
int rows = 0, cols = 0;
// the matrix holds the initial data and mutates the data as program runs
// final answers of the variables stored in answers.
double matrix[100][100], answers[100];


int main()
{
	getData();
	formatMatrix();
	factorCoef();
	getAnswers();
	supplyAnswers();

	return 0;	
}

void getData()
{
	// needed to handle negative numbers
	string temp;
	// used for counting collumns and rows
	int i = 0, j = 0;
	// source of data
	ifstream data;
	// opening the file stream
	data.open("matrixData.txt");
	
	// exit code of 4 if file is not opened
	if (data.fail())
	{
		exit(4);
	}

	// reads the entire file
	while(!data.eof())
	{
		// inserts next chunk of data, stoping at ' ' or '\n' into temporary string
		// using atof, converts the temporary string to float. I used this to get
		// around the issue of filestream not handling negative numbers well.
		data >> temp;
		matrix[i][j] = atof(temp.c_str());	
		// increments along the 2nd demension of the array, along the equation.
		j++;
		// peeking data for a '\n' character, signifying a new equation
		if(data.peek() == '\n')
		{
			// resets the collumns and increments the row.
			j = 0;
			i++;
		}
		
		
	}
	// sets the rows and collumns in terms of rows, possible error location
	rows = i;
	cols = rows + 1;
	data.close();
}

// see function decleration. matrix[i][i] must be nonzero for Gaussian Elimination
void formatMatrix()
{
	for(int i =0; i < rows; i++)
	{
		if(matrix[i][i] == 0)
		{
			// calls rowFix at the problem row
			rowFix(i);
		}
	}
	
}

void rowFix(int row)
{
	int b=0, c =0;
	
	// Searches for a row in matrix so that matrix[c][row] != 0
	while (b == 0)
	{
		b = matrix[c][row];
		c++;
		if(c > rows)
		{
			cout << "This system cannot be solved";
		}
	}
	// adds the two rows together and inserts into issue row
	for(int i = 0; i < cols; i++)
	{
		matrix[row][i] += matrix[b][i];
	}
	
}

// The meat of the program, eliminates all coeffecients up to the leading
// matrix [i][i]. Due to rounding, coeffecients may not be zero but will be
// extremely small e-005 and lower.
void factorCoef()
{
	int i = 0, j = 0, run =1;
	// factor needed to eliminate a coeffecient
	float z;
	
	for(run; run < rows; run++)
	{
			
		for(i; i < run; i++)
		{
			z = matrix[run][i] / matrix[i][i] * -1;
			
			for(j; j < cols; j++)
			{
				
				matrix[run][j] += z*matrix[i][j];
				
			}
			j = 0;
		}
		i = 0;
		
	}
	
	
}

// simple loop pulled from factor answers for clarity
void getAnswers()
{
	for(int i = rows ; i > 0; i--)
	{
		answers[i-1] = factorAnswers(answers[i], i);
	}
}

// finds the final answers for the variables starting with the
// final row, which should have coeffecients of 0 except for 
// the final coeffecient.
double factorAnswers(double ans, int numAns)
{
	double temp = 0;
	int ansGoal = numAns-1;
	
	for(int i = numAns; numAns < rows; numAns++)
	{
		temp += matrix[ansGoal][numAns]*answers[numAns];		
	}
	
	temp = roundf( ( (matrix[ansGoal][cols - 1] - temp) / matrix[ansGoal][ansGoal]) * 100 ) / 100;
	return temp;
}


void supplyAnswers()
{
	ofstream resWrite;
	int ansCount = 1;
	
	resWrite.open("GaussianResults.txt");
	
		if (resWrite.fail())
	{
		exit(3);
	}
	
	resWrite << "The answers to each variables are as follows:\n";
	cout << "The answers to each variables are as follows:\n";
	
	for(int i = 0; i < rows; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			if(ansCount <= rows)
			{
			
			resWrite << ansCount << ":\t" << answers[ansCount-1] << '\t';
			cout << ansCount << ":\t" << answers[ansCount-1] << '\t';
			ansCount++;
			}
		}
		resWrite << '\n';
		cout << '\n';
	}
	
	resWrite.close();
}

