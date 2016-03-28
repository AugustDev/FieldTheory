#include "Matrix.h"
#include <stdlib.h>

    int Matrix::getRows()
    {
        return num_rows;
    }
  
    int Matrix::getColumns()
    {
        return num_columns;
    }
  
    void Matrix::printOut()
    {
        for (unsigned int i = 0; i < matrix.size(); i++)
        {
            for (unsigned int j = 0; j < matrix[0].size(); j++) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << endl;
        }
    }
  
    void Matrix::ApplyBoundaryConfiguration(Matrix &m2)
    {
        for (unsigned int i = 0; i < matrix.size(); i++)
            for (unsigned int j = 0; j < matrix[0].size(); j++)
                if (!isnan(m2(i, j)))
                    matrix[i][j] = m2(i, j);
    }
	
	
  
    double& Matrix::operator()(const int row, const int column)
    {
        	if(!(column >= 0 && column < num_columns)){
		cout<<"Error, accessed element is outside the grid"<<endl;
		exit(4);
		}
        	if(!(row >=0 && row < num_rows)){
		cout<<"Error, accessed element is outside the grid"<<endl;
		exit(4);
		}

        return matrix[row][column];
    }
  
    void Matrix::InsertPotentialRectangular(int row, int column, int side, double Potential)
    {
        	if(!(row + side < num_rows)){
        	cout<<"Rectangular was inserted outside the grid"<<endl;
		exit(4);
		}
	   	if(!(column + side < num_columns)){
	   	cout<<"Rectangular was inserted outside the grid"<<endl;
		exit(4);
		}
  
        for (int x = row; x < row + side; x++)
            for (int y = column; y < column + side; y++)
                matrix[x][y] = Potential;
    }
  
    void Matrix::InsertPotentialWireHorizontal(int row, int column, int length, double Potential)
    {
		std::cout << "row: " << row << " column: " << column << " length: " << length << std::endl;
     		if(!(row >= 0 && column >= 0)){
		cout<<"Error, matrix row or column number is negative"<<endl;
		exit(4);
		}
     		if(!(column + length <= num_columns)){
		cout<<"Wire lenght is bigger than the grid size"<<endl;
		exit(4);
		}
	  
        for (int x = column; x < column + length; x++)
            matrix[row][x] = Potential;
    }
  
    void Matrix::InsertPotentialCircular(int row_c, int column_c, int radius, double Potential)
    {
        /* 
            row_c - row for the center of the circle 
            column_c - column for the center of the circle
        */
  
        	if(!(column_c >= 0 && row_c >= 0)){
		cout<<"Error, matrix row or column number is negative"<<endl;
		exit(4);
		}
		if(!(column_c + radius < num_columns)){
		cout<<"Portion of circle are outside of the grid"<<endl;
		exit(4);
		}
        if(!(column_c + radius < num_rows)){
		cout<<"Portion of circle are outside of the grid"<<endl;
		exit(4);
		}
  
        for (int y = row_c - radius; y <= row_c + radius; y++)
            for (int x = column_c - radius; x <= column_c + radius; x++)
                if ((column_c - x)*(column_c - x) + (row_c - y)*(row_c - y) < radius*radius)
                    matrix[y][x] = Potential;
  
  
    }
  
    double Matrix::Sum()
    {
        double sum = 0;
  
        for (int x = 0; x < num_rows; x++)
        for (int y = 0; y < num_columns; y++)
            sum += fabs(matrix[x][y]);
  
        return sum;
    }
  
    
    void Matrix::SaveToFile(const char* filename)
    {
  
        ofstream outf(filename);
  
        if (!outf)
        {
	               exit(1);
        }
  
        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_columns; j++)
            {
                outf << matrix[i][j] << "\t";
            }
            outf << "\n";
        }
    } 
	
	void Matrix::SaveToFile(std::string filename)
	{
		SaveToFile(filename.c_str());
	}
	
	void Matrix::SaveElectricField(const char* filename)
	{
		double V_x_1 = 0, V_x_2 = 0, V_y_1 = 0, V_y_2 = 0, E_x = 0, E_y = 0, E_MAX = 0, E = 0;
	int Column_Spacing = 15;

	  ofstream Numerical_Electric_Field_Map("Numerical_Electric_Field_Map.txt");
	  ofstream EField("EFieldS.txt");
	  
	  if (Numerical_Electric_Field_Map != NULL)
		{
		  ofstream Numerical_Electric_Field_Plotting_Data("Numerical_Electric_Field_Plotting_Data.txt");
		  if (Numerical_Electric_Field_Plotting_Data != NULL)
		{
		  //Numerical_Electric_Field_Map << setw(Column_Spacing);

	for (int y_Iterator = 0 + 1; y_Iterator+3 < num_rows -2; y_Iterator+=3)
			{
			  for (int x_Iterator = 0 + 1; x_Iterator+3 < num_columns - 2; x_Iterator+=3)
			{
			  V_x_1 = matrix [y_Iterator][x_Iterator - 1];
			  V_x_2 = matrix [y_Iterator][x_Iterator + 1];
			  V_y_1 = matrix [y_Iterator - 1][x_Iterator];
			  V_y_2 = matrix [y_Iterator + 1][x_Iterator];

			  E_x = -5*(V_x_1 - V_x_2)/(2*0.1);
			  E_y = -5*(V_y_1 - V_y_2)/(2*0.1);

			  EField << y_Iterator << "\t" << x_Iterator << "\t" << E_x << "\t" << E_y << endl;
			 
			  E = sqrt(pow(E_x, 2) + pow(E_y, 2));
			  if ((E > E_MAX) &&
				  (E != 1.0/0.0))
				{
				  E_MAX = E;
				}
			}
			}

		  for (int y_Iterator = 0 + 1; y_Iterator < num_rows -2; y_Iterator++)
			{
			  for (int x_Iterator = 0 + 1; x_Iterator < num_columns -2; x_Iterator++)
			{
			  V_x_1 = matrix [y_Iterator][x_Iterator - 1];
			  V_x_2 = matrix [y_Iterator][x_Iterator + 1];
			  V_y_1 = matrix [y_Iterator - 1][x_Iterator];
			  V_y_2 = matrix [y_Iterator + 1][x_Iterator];

			  E_x = 10*(V_x_1 - V_x_2)/(2*0.1);
			  E_y = 10*(V_y_1 - V_y_2)/(2*0.1);

			 
			  E = sqrt(pow(E_x, 2) + pow(E_y, 2));
			
			  if ((E_x == 1.0/0.0) ||
				  (E_x == -1.0/0.0))
				{
				  E_x = 0;
				}
			  if ((E_y == 1.0/0.0) ||
				  (E_y == -1.0/0.0))
				{
				  E_y = 0;
				}
			  Numerical_Electric_Field_Plotting_Data << setw(Column_Spacing) << x_Iterator << setw(Column_Spacing) << y_Iterator  << setw(Column_Spacing) << (E_x/E_MAX)*0.1 << setw(Column_Spacing) << (E_y/E_MAX)*0.1<< endl;
			  //E = (sqrt(pow(((V_x_1 - V_x_2)/(2*Unit_Conversion_Value)), 2) + pow(((V_y_1 - V_y_2)/(2*Unit_Conversion_Value)), 2)));
//			 Numerical_Electric_Field_Array [y_Iterator][x_Iterator] = E;
			  Numerical_Electric_Field_Map << E << setw(Column_Spacing);
			}
			  Numerical_Electric_Field_Map << endl;
			}
		}
		  else
		{
		  cout << "ERROR: The file Numerical_Electric_Field_Plotting_Data did not open sucessfully." << endl;
		}
		}
	  else 
		{
		  cout << "ERROR: The file Numerical_Electric_Field_Map did not open sucessfully." << endl;
		}
	}
	
