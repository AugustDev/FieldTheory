#ifndef __CGEOM_
#define __CGEOM_

#include "Matrix.h"
#include <algorithm> 

class CartesianGeometry : public Matrix
{
private:
int originY;
int originX;


public:
	CartesianGeometry(int rows, int columns, int oX, int oY)
	: Matrix(rows, columns, false), originY(oY), originX(oX)
	{

	}

	CartesianGeometry(int rows, int columns, int oX, int oY, bool BoundaryMatrix)
	: Matrix(rows, columns, true), originY(oY), originX(oX)
	{

	}

	/*
		Overloaded parenthesis operator, allows to call element at position (x,y). Where coordinate system is defined as:
		^ y
		|
		|____> x 
	*/
	double& operator()(const int x, const int y);
	
	double operator()(const int x, const int y, bool AllowOutside);

	/*
		Returns minimum allowed value for X component.
	*/
	int getInfX();

	/*
		Returns maximum allowed value for X component.
	*/	
	int getSupX();

	/*
		Returns minimum allowed value for Y component.
	*/
	int getInfY();

	/*
		Returns maximum allowed value for Y component.
	*/	
	int getSupY();
	
	/* 
		Specifying origin relative to the matrix element 
	*/
	void setOrigin(int cX, int cY);

	/*
		Adds a 'circular' region of constant potential centred at (x,y). Larger the radius, 
		the more accurately circle is represented.
	*/
	void InsertPotentialCircular(int x, int y, int radius, double Potential);
	
	/*
		Adds a horizontal line of constant potential starting from point (x,y) and
		having required length.
	*/
	void InsertPotentialWireHorizontal(int x, int y, int length, double Potential);
	
	/*
		Adds a square region of constant potential starting at specified point (x,y).
	*/
	void InsertPotentialRectangular(int x, int y, int side, double Potential);

	/*
		Applies boundary parameters and constrains from Boundary matrix.
		Ex: keeps potential of the cylinder constant throughout the algorithm.
	*/	
    void ApplyBoundaryConfiguration(CartesianGeometry m2);
	
	/*
		At each (weighted) point on a matrix applies a specified function at that point.
	*/
	void ApplyFunction(double (*func)(double, double), double dx, double dy);

	/*
		Applies a specified function at each point in closed interval [minX, maxX] x [minY, maxY].
	*/	
	void ApplyFunction(double (*func)(double, double), double dx, double dy, 
										  double minX, double maxX, double minY, double maxY);
										  
	void SaveElectricField(const char* filename, int FieldLineSpacing, double Potential, double dx, double dy);					

	/*
		Reverses ordering of rows of the matrix.
	*/
	void ReverseY();
};

#endif // __CGEOM_
