
#include "genetic_algo_lib.h"
#include <time.h>					// for random seed


int main()
{
	//random seed
	srand(unsigned(time(NULL)));

	//standard test
	/*Gen_algo_lib genetic;
	genetic.evolve();*/

	//test with boundaries
	double fitness;
	int delta;

	//target
	int myints[] = { 1, 3, 8, 64, 21, 4, 3, 256 };
	std::vector<int> tar(myints, myints + sizeof(myints) / sizeof(int));
	//genome from lib
	std::vector<int> genome;

	//boundaries for genome
	std::vector<gal_boundaries> lim;
	gal_boundaries l;
	l.low = 1; l.high = 10;	lim.push_back(l);//layer
	l.low = 1; l.high = 3;  lim.push_back(l);//type
	l.low = 1; l.high = 8;  lim.push_back(l);//activation
	l.low = 1; l.high = 64; lim.push_back(l);//w, h
	l.low = 1; l.high = 21; lim.push_back(l);//kersize
	l.low = 1; l.high = 4;  lim.push_back(l);//channels nmaps
	l.low = 1; l.high = 3;  lim.push_back(l);//pool
	l.low = 1; l.high = 256; lim.push_back(l);//hidden

	Gen_algo_lib gen(tar, lim);
	gen.init_population();

	//iterration: genetic
	for (int i = 0; i<gen.ret_max_iter(); i++) 
	{
		//iteration for each citizen
		for (int j = 0; j < gen.ret_pop_size(); j++)
		{
			//gen.calc_fitness();		// calculate fitness
			//calculate fitness from target or else
			genome = gen.ret_genome(j);
			fitness = 0;
			for (int k = 0; k < genome.size(); k++)
			{
				delta = tar[k] - genome[k];
				delta = delta > 0 ? delta : -delta;
				fitness += delta;
			}
			gen.set_fitness(j, fitness);
			if (fitness < 1.0F) break;
		}
		
		gen.sort_by_fitness();	// sort them
		gen.print_best();		// print the best one
		
		if (fitness < 1.0F) break;
		

		gen.mate();				// mate the population together
		gen.swap();				// swap buffers
	}
	
	return 0;
}
