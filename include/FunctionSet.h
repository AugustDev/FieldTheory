#include <cmath>

double Gaussian(double x, double y)
{
	return 100*exp(-(x*x/10+y*y/10));
}

double TopHat(double x, double y)
{
	if (fabs(x) < 1 && fabs(y) < 1)
		return 1;
	return 0;
}

/* 
	Glogal configuration for this problem 
*/
	double CS_d = 4.5;
	double CS_R = 0.5;
	double CS_Vo = 1;
	
double CylinderSystem(double x, double y)
{	
	//if (y < 0) return 0;
	
	double l_num = pow((sqrt(CS_d*CS_d - CS_R*CS_R) + y), 2) + x*x;
	double l_den = pow((-sqrt(CS_d*CS_d - CS_R*CS_R) + y), 2) + x*x;
	
	double result = 0.5*CS_Vo*log( l_num / l_den ) / acosh(CS_d/CS_R);
	
	/* Since this solution is inadequate inside the cylinder */
	if (result > CS_Vo)
		return CS_Vo;
		
	/* Since the physical problem assumes plane of zero potential */
	if (result < 0)
		return 0;

	return result;
}