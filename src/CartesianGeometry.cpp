#include "CartesianGeometry.h"
#include <stdlib.h>

extern unsigned int ReverseFileOrdering;

double& CartesianGeometry::operator()(const int x, const int y)
{
 		if(!(originY-y >= 0)){
			cout<<"Chosen origin is outside the grid"<<endl;
		exit(4);
		}
 		if(!(originX+x >= 0)){
			cout<<"Chosen origin is outside the grid"<<endl;
		exit(4);
		}

        return Matrix::matrix[originY-y][originX+x];
}

double CartesianGeometry::operator()(const int x, const int y, bool AllowOutside)
{
	if (AllowOutside == true)
		if ((originY-y < 0 || originY-y >= getColumns()) &&  (originX+x < 0 || originX+x >= getRows()))
			return 0;
		
 		if(!(originY-y >= 0)){
			cout<<"Chosen origin is outside the grid"<<endl;
		exit(4);
		}
 		if(!(originX+x >= 0)){
			cout<<"Chosen origin is outside the grid"<<endl;
		exit(4);
		}

        return Matrix::matrix[originY-y][originX+x];
}

int CartesianGeometry::getInfX()
{
	return -(originX);
}

int CartesianGeometry::getSupX()
{
	return (Matrix::num_columns - originX - 1);
}
	
int CartesianGeometry::getInfY()
{
	return -(Matrix::num_rows - originY - 1);
}

int CartesianGeometry::getSupY()
{
	return (originY);
}

void CartesianGeometry::setOrigin(int cX, int cY)
{
	originX = cX;
	originY = cY;
}

void CartesianGeometry::InsertPotentialCircular(int x, int y, int radius, double Potential)
{
	Matrix::InsertPotentialCircular(originY - y, originX + x, radius, Potential);
}

void CartesianGeometry::InsertPotentialWireHorizontal(int x, int y, int length, double Potential)
{
	Matrix::InsertPotentialWireHorizontal(originY - y, originX + x, length, Potential);
}

void CartesianGeometry::InsertPotentialRectangular(int x, int y, int length, double Potential)
{
	Matrix::InsertPotentialRectangular(originY - y, originX + x, length, Potential);
}

void CartesianGeometry::ApplyBoundaryConfiguration(CartesianGeometry m2)
{
	for (int i = (*this).getInfX(); i <= (*this).getSupX(); i++)
		for (int j = (*this).getInfY(); j <= (*this).getSupY(); j++)
			if (!std::isnan(m2(i, j))) {
				(*this)(i,j) = m2(i, j);
			}				
}

void CartesianGeometry::ApplyFunction(double (*func)(double, double), double dx, double dy)
{
	for (int i = (*this).getInfX(); i <= (*this).getSupX(); i++)
		for (int j = (*this).getInfY(); j <= (*this).getSupY(); j++) {
			(*this)(i,j) = func(i*dx, j*dy);	
			if (std::isnan(func(i*dx, j*dy)))
				std::cout << "Nan at " << i*dx << " " << j*dy << std::endl;
		}
}

void CartesianGeometry::ApplyFunction(double (*func)(double, double), double dx, double dy, 
									  double minX, double maxX, double minY, double maxY)
{
	for (int i = (*this).getInfX(); i <= (*this).getSupX(); i++)
		for (int j = (*this).getInfY(); j <= (*this).getSupY(); j++)
			if (i*dx >= minX && i*dx <= maxX && j*dy >= minY && j*dy <= maxY)
				(*this)(i,j) = func(i*dx, j*dy);	
}

void CartesianGeometry::ReverseY()
{
	std::reverse(matrix.begin(),matrix.end());
}

void CartesianGeometry::SaveElectricField(const char* filename, int FieldLineSpacing, double Potential, double dx, double dy)
{
  ofstream EField(filename);
	
	
	double E_max = 0;
	
  	for(int y = getInfY() + 1; y + FieldLineSpacing < getSupY() - 1; y+=FieldLineSpacing)
	{
		for(int x = getInfX() + 1; x + FieldLineSpacing < getSupX() - 1; x+=FieldLineSpacing)
		{
			double E_x = -10*Potential*((*this)(x + 1, y) - (*this)(x - 1, y) )/(2.0*dx);
			double E_y = -10*Potential*((*this)(x, y + 1) - (*this)(x, y - 1) )/(2.0*dy);
			
			double E = sqrt(pow(E_x, 2) + pow(E_y, 2));
			E_max = E > E_max ? E : E_max;
		}
	}
	
	if (ReverseFileOrdering == 1 || ReverseFileOrdering == 2)
		(*this).ReverseY();
	
	//for(int y = getInfY() + 1; y + FieldLineSpacing < getSupY() - 1; y+=FieldLineSpacing)
	for(int y = getSupY() - 1; y - FieldLineSpacing > getInfY() + 1; y-=FieldLineSpacing)
	{

 		for(int x = getInfX() + 1; x + FieldLineSpacing < getSupX() - 1; x+=FieldLineSpacing)
		//for(int x = getSupX() - 1; x - FieldLineSpacing > getInfX(); x-=FieldLineSpacing)
		{
			double E_x = -100*Potential*((*this)(x + 1, y) - (*this)(x - 1, y) )/(2.0*dx);
			double E_y = -100*Potential*((*this)(x, y + 1) - (*this)(x, y - 1) )/(2.0*dy);
			
			EField << x << "\t" << y << "\t" << E_x/E_max << "\t" << E_y/E_max << endl;
			//EField << y << " " << x << "\n";
		} 
	}
	
	//EField << "hey";
}
