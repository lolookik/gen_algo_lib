#ifndef __GEN_ALGO_LIB__
#define __GEN_ALGO_LIB__

/*genetic algo lib
 *small object-based header-only genetic algorithm library*/

#include <iostream>		
#include <vector>		

#include <algorithm>	//sort
#include <time.h>		//time as seed
#include <math.h>		//???

using namespace std;

struct gal_structure
{
	vector<int> genome;		// the genome
	double fitness;			// its fitness
};

struct gal_boundaries
{
	int low;
	int high;
};

class Gen_algo_lib {
public:
	Gen_algo_lib();
	Gen_algo_lib(vector<int>, vector<gal_boundaries>);
	int init_population();
	int calc_fitness();
	int set_fitness(int, double);
	
	//bool fitness_sort_binary(gal_structure, gal_structure);
	int sort_by_fitness();
	int elitism(vector<gal_structure>*, vector<gal_structure>*);
	int mutate(gal_structure*);
	int mate();
	int print_best();
	int swap();

	int evolve();

	long int ret_pop_size();
	long int ret_max_iter();
	vector<int> ret_genome(int);

private:
	vector <gal_structure> population;
	vector <gal_structure> buffer;
	vector <gal_boundaries> limits;
	long int pop_size =  2048;
	long int max_iter = 65536;
	double elit_rate = 0.10;
	double mutation_rate = 0.25;
	double mutation = RAND_MAX * mutation_rate;
	vector<int> target;// = "Hello world!";

};//class Gen_algo_lib


/*****************************************************************************/
/* by default evolve a small genome of numbers between 1 and 10
 * TODO someday: replace pushback based init by size based init */
Gen_algo_lib::Gen_algo_lib()
{
	gal_boundaries lim;
	lim.low = 0;
	lim.high = 10;
	
	target.push_back(1); limits.push_back(lim);
	target.push_back(4); limits.push_back(lim);
	target.push_back(2); limits.push_back(lim);
	target.push_back(8); limits.push_back(lim);
	target.push_back(5); limits.push_back(lim);
	target.push_back(7); limits.push_back(lim);
	target.push_back(1); limits.push_back(lim);
}

/*****************************************************************************/
/* create a dummy genome and boundaries condition outside the library for
 * complex evolution
 * dummy genome is used only for genome size evaluation */
Gen_algo_lib::Gen_algo_lib(vector<int> tar, vector<gal_boundaries> lim)
{
	target = tar;
	limits = lim;
}

/*****************************************************************************/
/* uses the target from the constructor to create random genomes within 
 * boundaries */
int Gen_algo_lib::init_population()
{
	int tsize = target.size();

	for (int i = 0; i<pop_size; i++) {
		gal_structure citizen;

		citizen.fitness = 0;
		citizen.genome.resize(target.size());
		//citizen.genome = 0; //how to init to 0?

		for (int j = 0; j<tsize; j++)
			citizen.genome[j] = (rand() % (limits[j].high-limits[j].low+1) + limits[j].low);

		population.push_back(citizen);
		buffer.push_back(citizen);
	}

	//buffer.resize(pop_size);
	return 0;
}

/*****************************************************************************/
/* computes the fitness with respect to the constructor's target */
int Gen_algo_lib::calc_fitness()
{
	double fitness;
		
	for (int i = 0; i<pop_size; i++) {
		fitness = 0;
		for (int j = 0; j<target.size(); j++) {
			fitness += abs(double(population[i].genome[j] - target[j]));
		}
		
		population[i].fitness = fitness;
		
	}
	return 0;
}

/*****************************************************************************/
/* in case the fitness is computed outside the library */
int Gen_algo_lib::set_fitness(int pop, double fit)
{
	population[pop].fitness = fit;
	

	return 0;
}

/*****************************************************************************/
/* used by std sort algorithm */
bool fitness_sort_binary(gal_structure x, gal_structure y)
{
	return (x.fitness < y.fitness);
}

/*****************************************************************************/
/* search for best citizens */
int Gen_algo_lib::sort_by_fitness()
{
	sort(population.begin(), population.end(), fitness_sort_binary);
	return 0;
}

/*****************************************************************************/
/* copy best citizens */
int Gen_algo_lib::elitism(vector<gal_structure> *population,
	vector<gal_structure> *buffer)
{
	for (int i = 0; i < pop_size*elit_rate; i++) {
		buffer->at(i).genome = population->at(i).genome;
		buffer->at(i).fitness = population->at(i).fitness;
	}
	return 0;
}

/*****************************************************************************/
/* random mutations on selected citizen */
int Gen_algo_lib::mutate(gal_structure* member)
{
	int tsize = target.size();
	int ipos = rand() % tsize;
	int delta = (rand() % (limits[ipos].high - limits[ipos].low +1) + limits[ipos].low);

	member->genome[ipos] = ((member->genome[ipos] + delta) % (limits[ipos].high - limits[ipos].low + 1) + limits[ipos].low);
	return 0;
}


/*****************************************************************************/
/* *hum* */
int Gen_algo_lib::mate()
{
	long int esize = pop_size * elit_rate;
	long int tsize = target.size(), spos, i1, i2;
	
	//buffer stores old genomes as they are needed for the creation of the new ones
	elitism(&population, &buffer);

	for (long int i = esize; i<pop_size; i++) {
		i1 = rand() % (pop_size/ 2);
		i2 = rand() % (pop_size/ 2);
		spos = rand() % tsize; 
		

		for (int j = 0; j < spos; j++)
			buffer[i].genome[j] = population[i1].genome[j];
		
		for (int j = spos; j < tsize; j++)
			buffer[i].genome[j] = population[i2].genome[j];
		

		if (rand() < mutation) 
			mutate(&(buffer[i]));
	}
	return 0;
}

/*****************************************************************************/
/* prints the best genome */
int Gen_algo_lib::print_best()
{
	cout << "Best: ";
	for (int i = 0; i < population[0].genome.size(); i++)
		cout << population[0].genome[i] << " ";

	cout << "Fitness: "<< population[0].fitness << endl;
	return 0;
}

/*****************************************************************************/
/* swaps population and buffer vector */
int Gen_algo_lib::swap()
{
	vector<gal_structure> temp = population; 
	population = buffer; 
	buffer = temp;
	return 0;
}

/*****************************************************************************/
/* skynet is comming */
int Gen_algo_lib::evolve()
{
	init_population();
	
	for (int i = 0; i<max_iter; i++) 
	{
		calc_fitness();		// calculate fitness
		sort_by_fitness();	// sort them
		print_best();		// print the best one

		if (population[0].fitness == 0) break;

		mate();				// mate the population together
		swap();				// swap buffers
	}
	return 0;
}

/*****************************************************************************/
/* returns population size */
long int Gen_algo_lib::ret_pop_size()
{
	return pop_size;
}

/*****************************************************************************/
/* returns maximum iteration number size */
long int Gen_algo_lib::ret_max_iter()
{
	return max_iter;
}

vector<int> Gen_algo_lib::ret_genome(int index)
{
	return population[index].genome;
}


#endif //__GEN_ALGO_LIB__ #pragma once