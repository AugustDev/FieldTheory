#ifndef __MATRIX_
#define __MATRIX_

#include <iostream>
#include <vector>
#include <assert.h>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;

class Matrix
{
protected:
	vector< vector<double> > matrix;
	int num_rows;
	int num_columns;

public:
	Matrix(int rows, int columns, bool BoundaryMatrix = false)
	{
		num_rows = rows;
		num_columns = columns;

		/* Initializing two dimensional array (matrix) */
		for (int i = 0; i < rows; i++) {
			if (BoundaryMatrix == true)
				matrix.push_back(vector<double>(columns, sqrt(-1)));
			else
				matrix.push_back(vector<double>(columns, 0));
		}
	}

	/*
		Returns number of rows of the matrix.
	*/
	int getRows();

	/*
		Returns number of columns of the matrix.
	*/
	int getColumns();

	/*
		Prints out matrix on screen. Useful for testing only.
	*/
	void printOut();

	/*
		Applies boundary parameters and constrains from Boundary matrix.
		Ex: keeps potential of the cylinder constant throughout the algorithm.
	*/
	void ApplyBoundaryConfiguration(Matrix &m2);

	/*
		Overloaded parenthesis operator, allows to call matrix elements as A(a,b).
	*/
	double& operator()(const int row, const int column);

	/*
		Adds a square region of constant potential starting at specified row and column.
		InsertPotentialRectangular(2, 3, 5, 0) - Will add a region of 0 potential starting from
		row 2, column 3, ending with row 2+5=7, column 3+5=8.
	*/
	void InsertPotentialRectangular(int row, int column, int side, double Potential);

	/*
		Adds a horizontal line of constant potential starting from specified row and column and
		having required length.
		Infinite wire for matrix A at row R and potential V would be represented as: 
		A.InsertPotentialWireHorizontal(R, 0, A.getColumns(), V);
	*/
	void InsertPotentialWireHorizontal(int row, int column, int length, double Potential);

	/*
		Adds a 'circular' region of constant potential centred at (row_c, column_c). Larger the radius, 
		the more accurately circle is represented.
	*/
	void InsertPotentialCircular(int row_c, int column_c, int radius, double Potential);

	/*
		Gives a sum of matrix elements.
	*/
	double Sum();

	/*
		Saves matrix elements to a file.
	*/
	void SaveToFile(const char* filename);
	
	void SaveToFile(std::string filename);
	
	/*
		Allows direct access to matrix vector. Useful in importing raw data files.
	*/
	
	friend void AccessMatrix(Matrix &m);
	
	void SaveElectricField(const char* filename);
	
	void SaveElectricField(std::string filename);
};


#endif // __MATRIX_
