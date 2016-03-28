#ifndef __CONFIGP_
#define __CONFIGP_
		
#include <iostream>
		
bool LOAD_FILE = true;
unsigned int ReverseFileOrdering = 0;

std::string FILE_NAME = "setup.txt";

/* 
	Default configuration if none is specified 
*/
double CONFIG_LENGTH_X = 0;
double CONFIG_LENGTH_Y = 0;
double CONFIG_DX = 0.1;
double CONFIG_DY = 0.1;
double CONFIG_DT = 0.001;
double CONFIG_EPS = 10E-10;

int CONFIG_SIZE_X = 61;
int CONFIG_SIZE_Y = 61;
int CONFIG_ORIGIN_X = CONFIG_SIZE_X/2;
int CONFIG_ORIGIN_Y = CONFIG_SIZE_Y/2;
int CONFIG_LOOPS = 1000;

void ImportRawData(CartesianGeometry &Boundary,
				   CartesianGeometry &Potential,
				   CartesianGeometry &Potential2,
				   std::string FILE_NAME)
{
	std::string delimiter = "	";
	size_t pos = 0;
	std::string token;

	std::ifstream infile(FILE_NAME);
	if (!infile)
	{
		std::cout << "Specified data file was not found. Exiting." << std::endl;
		exit(1);
	}
	
	int fileRows = countLines(FILE_NAME);
	int fileColumns = countColumns(FILE_NAME);

	/* Reconstructing Matrices based on new size data */
	CartesianGeometry BoundaryConfigurationG(fileRows, 
											 fileColumns, 
											 0, 
											 0, 
											 true);
	CartesianGeometry p1(fileRows, 
						 fileColumns, 
						 0, 
						 0);
						 
	CartesianGeometry p2(fileRows, 
						 fileColumns, 
						 0, 
						 0);
												 
												 
	Boundary = BoundaryConfigurationG;
	Potential = p1;
	Potential2 = p2;
											 
	std::string line;

	int row = 0;
	int col = 0;
	while (std::getline(infile, line))
	{
		while ((pos = line.find(delimiter)) != std::string::npos) 
		{
			if (line.empty() || line[0] == '#')
				break;
				
			token = line.substr(0, pos);
			((Matrix&)Boundary)(row,col) = token.compare(std::string("i")) ? atof(token.c_str()) : sqrt(-1);
			line.erase(0, pos + delimiter.length());
			col++;
		}
			//std::cout << "[" << row << ", " << col << "]" << std::endl;
			//((Matrix&)Boundary)(row, col) = atof(line.c_str());
			col = 0;
	
		row++;
		if (row >= fileRows)
			break;
	}	
	
}

void ImportObjects(CartesianGeometry &Boundary, std::string FILE_NAME)
{
	std::string delimiter = "	";
	size_t pos = 0;
	std::string token;

	std::ifstream infile(FILE_NAME);
	if (!infile)
	{
		std::cout << "Specified configuration file was not found. Exiting." << std::endl;
		exit(1);
	}

	std::string line;
	std::vector<std::string> ConfigContainer;


	while (std::getline(infile, line))
	{

		while ((pos = line.find(delimiter)) != std::string::npos) 
		{
			if (line.empty() || line[0] == '#')
				break;
				
			token = line.substr(0, pos);
			ConfigContainer.push_back(token);
			line.erase(0, pos + delimiter.length());
		}
			ConfigContainer.push_back(line);
			

			if (ConfigContainer[0] == std::string("CYLINDER")) 
			{
				double Radius = atof(ConfigContainer[3].c_str());
				
				if (Radius > 0)
				Boundary.InsertPotentialCircular(atof(ConfigContainer[1].c_str()), 
										   atof(ConfigContainer[2].c_str()), 
										   Radius, 
										   atof(ConfigContainer[4].c_str()));
				else 
					std::cout << "Cylinder must have non-zero radius." << std::endl;
			} 		
			if (ConfigContainer[0] == std::string("HWIRE")) 
			{
				Boundary.InsertPotentialWireHorizontal(atof(ConfigContainer[1].c_str()),
													   atof(ConfigContainer[2].c_str()), 
													   atof(ConfigContainer[3].c_str()), 
													   atof(ConfigContainer[4].c_str()));
			
			} 

			ConfigContainer.clear();

	}
}

void LoadConfigurationFile(std::string FILE_NAME) 
{

	std::string delimiter = "	";
	size_t pos = 0;
	std::string token;

	std::ifstream infile(FILE_NAME);
	
	if (!infile)
	{
		std::cout << "Specified configuration file was not found. Exiting." << std::endl;
		exit(1);
	}

	std::string line;
	std::vector<std::string> ConfigContainer;


	while (std::getline(infile, line))
	{

		while ((pos = line.find(delimiter)) != std::string::npos) {
			if (line.empty() || line[0] == '#')
				break;
				
			token = line.substr(0, pos);
			ConfigContainer.push_back(token);
			line.erase(0, pos + delimiter.length());
		}
			ConfigContainer.push_back(line);
			

			if (ConfigContainer[0] == std::string("epsilon")) 
			{
				CONFIG_EPS = atof(ConfigContainer[1].c_str());
			} 		
			if (ConfigContainer[0] == std::string("dx")) 
			{
				CONFIG_DX = atof(ConfigContainer[1].c_str());
			} 
			else if (ConfigContainer[0] == std::string("dy")) 
			{
				CONFIG_DY = atof(ConfigContainer[1].c_str());
			}
			else if (ConfigContainer[0] == std::string("dt")) 
			{
				CONFIG_DT = atof(ConfigContainer[1].c_str());
			}
			else if (ConfigContainer[0] == std::string("loops")) 
			{
				CONFIG_LOOPS = atof(ConfigContainer[1].c_str());
			}
			else if (ConfigContainer[0] == std::string("SIZE_X")) 
			{
				CONFIG_SIZE_X = atoi(ConfigContainer[1].c_str());
				CONFIG_ORIGIN_X = CONFIG_SIZE_X/2;
			}
			else if (ConfigContainer[0] == std::string("SIZE_Y")) 
			{
				CONFIG_SIZE_Y = atoi(ConfigContainer[1].c_str());
				CONFIG_ORIGIN_Y = CONFIG_SIZE_Y/2;
			}
			else if (ConfigContainer[0] == std::string("ORIGIN_X")) 
			{
				CONFIG_ORIGIN_X = atoi(ConfigContainer[1].c_str());
			}
			else if (ConfigContainer[0] == std::string("ORIGIN_Y")) 
			{
				CONFIG_ORIGIN_Y = atoi(ConfigContainer[1].c_str());
			}
			if (ConfigContainer[0] == std::string("LENGTH_X")) 
			{
				CONFIG_LENGTH_X = atof(ConfigContainer[1].c_str());
				CONFIG_SIZE_X = CONFIG_LENGTH_X/CONFIG_DX;
				CONFIG_ORIGIN_X = CONFIG_SIZE_X/2;
			} 
			if (ConfigContainer[0] == std::string("LENGTH_Y")) 
			{
				CONFIG_LENGTH_Y = atof(ConfigContainer[1].c_str());
				CONFIG_SIZE_Y = CONFIG_LENGTH_Y/CONFIG_DY;
				CONFIG_ORIGIN_Y = CONFIG_SIZE_Y/2;
			} 
			ConfigContainer.clear();

	}

}

void printConfig() 
{
	std::cout << "Algorithm constants" << std::endl;
	std::cout << "dx:\t" << CONFIG_DX << std::endl;
	std::cout << "dy:\t" << CONFIG_DY << std::endl;
	//std::cout << "DT:\t" << CONFIG_DT << std::endl;
	std::cout << "Eps:\t" << CONFIG_EPS << std::endl;
	std::cout << "Max loops:\t" << CONFIG_LOOPS << std::endl;
	
	std::cout << std::endl;
	std::cout << "Grid parameters" << std::endl;
	std::cout << "Size X:\t" << CONFIG_SIZE_X << std::endl;
	std::cout << "Size Y:\t" << CONFIG_SIZE_Y << std::endl;
	std::cout << "Origin X:\t" << CONFIG_ORIGIN_X << std::endl;
	std::cout << "Origin Y:\t" << CONFIG_ORIGIN_Y << std::endl;	
}

#endif // __CONFIGP_
