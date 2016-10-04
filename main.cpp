
#include "genetic_algo_lib.h"
#include <time.h>					// for random seed

int main()
{
	srand(unsigned(time(NULL)));

	Gen_algo_lib genetic;
	genetic.evolve();

	return 0;
}
