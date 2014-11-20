#include "Util.h"

bool contains(vector<int> vec, int res)
{
	bool ret=false;
	vector<int>::iterator vec_iter;
	for(vec_iter=vec.begin();vec_iter!=vec.end();vec_iter++){
		if(*vec_iter==res){
			ret=true;
			break;
		}
	}
	return ret;
}

double rnd(double dbLow, double dbUpper)
{
	double dbTemp=rand()/((double)RAND_MAX+1.0);
	return dbLow+dbTemp*(dbUpper-dbLow);
}

double ROUND(double dbA)
{
	return (double)((int)(dbA+0.5));
}
