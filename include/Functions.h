#ifndef __FUNCTIONSP3_
#define __FUNCTIONSP3_

//#include<stdexcept>
#include<iterator>
#include <algorithm>

void PlotDifference(Matrix m1, Matrix m2)
{
	ofstream outf("diff.txt");
	
	for(int i = 0; i < m1.getRows(); i++)
	{
		for (int j = 0; j < m1.getColumns(); j++)
		{
			outf << m1(i, j) - m2(i,j) << "\t";
		}
		outf << "\n";
	}
}

template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

unsigned int countLines(std::string file)
{
    std::ifstream myfile(file);

    // new lines will be skipped unless we stop it from happening:    
    myfile.unsetf(std::ios_base::skipws);

    // count the newlines with an algorithm specialized for counting:
    unsigned line_count = std::count(
        std::istream_iterator<char>(myfile),
        std::istream_iterator<char>(), 
        '\n');

    return line_count;
}

unsigned int countColumns(std::string file)
{
  ifstream infile(file);
  unsigned int count = 0;
  
  if (infile.good())
  {
    string s;
    getline(infile, s);
	count = std::count(s.begin(), s.end(), '	');
  }
  infile.close();
  
  return count;
}	
#endif // __FUNCTIONSP3_