#include <iostream>
#include <math.h>
#include <chrono>
#include <fstream>
#include <sstream>
#include <stdlib.h>


#include "Constants.h"
#include "Matrix.h"
#include "CartesianGeometry.h"
#include "FunctionSet.h"
#include "Functions.h"
#include "Configuration.h"

using namespace std;          


int main(int argc, char** argv)
{

unsigned int COMPUTATION_TYPE = 0;
enum ComputationType
{
	CONFIG_FILE,
	RAW_FILE,
	PICTURE
};

/*
	Parsing input arguments.
*/
if (argc < 2)
	std::cout << "Missing input arguments." << std::endl;

for (int i = 1; i < argc; i++)
{
	if (std::string(argv[i]) == std::string("-type"))
	{
		if (std::string(argv[i+1]) == std::string("config"))
			COMPUTATION_TYPE = CONFIG_FILE;
		if (std::string(argv[i+1]) == std::string("raw"))
			COMPUTATION_TYPE = RAW_FILE;
		if (std::string(argv[i+1]) == std::string("picture"))
			COMPUTATION_TYPE = PICTURE;
	}
	if (std::string(argv[i]) == std::string("-file"))
	{
		FILE_NAME = std::string(argv[i+1]);
	}
	if (std::string(argv[i]) == std::string("-loops"))
	{
		CONFIG_LOOPS = atof(std::string(argv[i+1]).c_str());
	}
	if (std::string(argv[i]) == std::string("-eps"))
	{
		CONFIG_EPS = atof(std::string(argv[i+1]).c_str());
	}
	if (std::string(argv[i]) == std::string("-reverse"))
	{
		ReverseFileOrdering = atoi(std::string(argv[i+1]).c_str());
	}
}
	
if (LOAD_FILE)
	LoadConfigurationFile(FILE_NAME);
	

	/***************************
		Start of Configuration
	***************************/

	/* 
	   Number of of grid points in x,y direction. 
	   For odd sizes center is at (sizeY/2, sizeX/2)
	*/
 
 	/* Spacing parameters */
//	double dx = 0.2;
	//double dy = 0.2;
	//double dt = 0.01;//
	
	
	/* Contains information about objects that creates electric field */
	CartesianGeometry BoundaryConfigurationG(CONFIG_SIZE_X, 
											 CONFIG_SIZE_Y, 
											 CONFIG_ORIGIN_X, 
											 CONFIG_ORIGIN_Y, 
											 true);

	/* Stores computation and temporary data */
	CartesianGeometry PotentialG(CONFIG_SIZE_X, 
								 CONFIG_SIZE_Y, 
								 CONFIG_ORIGIN_X, 
								 CONFIG_ORIGIN_Y);
								 
	CartesianGeometry PotentialG_new(CONFIG_SIZE_X, 
									 CONFIG_SIZE_Y, 
									 CONFIG_ORIGIN_X, 
									 CONFIG_ORIGIN_Y);
			
	/* Prints current configuration */
	printConfig();
	
	/* Importing objects from configuration file */
	if (COMPUTATION_TYPE == CONFIG_FILE)
		ImportObjects(BoundaryConfigurationG, FILE_NAME);
	else if (COMPUTATION_TYPE == RAW_FILE)
		ImportRawData(BoundaryConfigurationG, PotentialG, PotentialG_new, FILE_NAME);
		
	if (ReverseFileOrdering == 1)
		BoundaryConfigurationG.ReverseY();
	
	/* Function data */

	CartesianGeometry FunctionContainer(CONFIG_SIZE_X, CONFIG_SIZE_Y, CONFIG_ORIGIN_X, CONFIG_ORIGIN_Y + CS_d/CONFIG_DX);
	FunctionContainer.ApplyFunction(CylinderSystem, CONFIG_DX, CONFIG_DY);
	FunctionContainer.SaveToFile("Function.txt");
	
	


	/* Actual algorithm */
	PotentialG.ApplyBoundaryConfiguration(BoundaryConfigurationG);
	
	/* Keeping computational time */
	auto start = std::chrono::system_clock::now();
	
	std::cout << "LengthX: " << CONFIG_LENGTH_X << std::endl;
	std::cout << "LengthY: " << CONFIG_LENGTH_Y << std::endl;

	/*
		Temporary data used in calculations 
	*/
	double temp1 = 0, temp2 = 0;
	int rows = PotentialG.getRows();
	int columns = PotentialG.getColumns();
	int counter = 0;
	
	do {
		/* Keeping track of number of loops */
		counter++;
		
		if (counter >= CONFIG_LOOPS)
			break;
		
		temp1 = PotentialG.Sum();
		/*
		for (int u = PotentialG.getInfX() + 1; u < PotentialG.getSupX(); u++)
		{
			for (int v = PotentialG.getInfY() + 1; v < PotentialG.getSupY(); v++)
			{
				PotentialG_new(u, v) = PotentialG(u, v) + CONFIG_DT / (CONFIG_DX * CONFIG_DY) * (PotentialG(u+1, v) - 4*PotentialG(u,v) + PotentialG(u-1, v) + PotentialG(u, v+1) + PotentialG(u, v-1) );
			}
		}
		*/
		
		// This is more efficient algorithm
		double beta = CONFIG_DX / CONFIG_DY;
		double denom = 2*(1+beta*beta);
		
		
		for (int i = PotentialG.getInfX() + 1; i < PotentialG.getSupX(); i++)
		{
			for (int j = PotentialG.getInfY() + 1; j < PotentialG.getSupY(); j++)
			{
				//std::cout << "[" << i << " " << j << "] = "  << PotentialG_new(i,j) << std::endl;
				PotentialG_new(i,j)=(PotentialG(i-1,j)+PotentialG(i+1,j)+beta*beta*(PotentialG(i,j-1)+PotentialG(i,j+1)))/denom; 
				//PotentialG_new(i, j) = PotentialG(i, j) + CONFIG_DT / (CONFIG_DX * CONFIG_DY) * (PotentialG(i+1, j) - 4*PotentialG(i,j) + PotentialG(i-1, j) + PotentialG(i, j+1) + PotentialG(i, j-1) );
			}
		}		
	
		
		// Need to keep track of potential field in previous time.
		PotentialG_new.ApplyBoundaryConfiguration(BoundaryConfigurationG);
		PotentialG = PotentialG_new;
		
		temp2 = PotentialG.Sum();
		
		/*
		if (!(counter % 100) && counter < 500)
			PotentialG.SaveToFile(to_string(counter));
			
		if (!(counter % 500))
			PotentialG.SaveToFile(to_string(counter));
		*/
			
	} while (fabs(temp2-temp1)/(rows*columns) > CONFIG_EPS);

	
	PotentialG.SaveToFile("Potential.txt");
	//PotentialG.SaveElectricField("EField.txt");
	//PlotDifference((Matrix)FunctionContainer, (Matrix)PotentialG);
	PotentialG.SaveElectricField("EField.txt", 4, 1, CONFIG_DX, CONFIG_DY);
	
	auto end = std::chrono::system_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	cout << endl << "Calculation time: " << static_cast<double>(elapsed.count())/1000 << " seconds." << endl;
	cout << "Performed loops: " << counter << "." << endl;
	cout << "Quantified error: " << (temp2-temp1)/(rows*columns) << endl;

	//system("pause");
	return 0;
}
