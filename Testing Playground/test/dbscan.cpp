#import <mlpack/core.hpp>
//#import <mlpack/methods/dbscan/dbscan.hpp>
#include <mlpack/mehtods/piecewise_linear_regresion>

using namespace mlpack;

int main()
{
	arma::mat data;
	data::Load("map_data", data);

	dbscan::DBSCAN<> model(0.5,5);

	return 0;
}
