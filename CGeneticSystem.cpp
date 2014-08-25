#include "StdAfx.h"
#include "CGeneticSystem.h"


CGeneticSystem::CGeneticSystem(void){
}
CGeneticSystem::~CGeneticSystem(void){
	delete m_pLocalSearch;
}
CGeneticSystem::CGeneticSystem(Parameters &Par , MatrixArrayTypeInt *d)
{
	//nedd to refactor
	this->m_populationSize = Par.noAnts;
	this->m_noAnts = Par.noAnts;
	this->m_noCitys = d->size();
	this->m_mutationRatio =Par.mutationRatio;
	m_LocalSearchOpt2 = Par.LocalSearchOpt2;
	m_LocalSearchOpt3 = Par.LocalSearchOpt3;
	m_LocalSearchGreed = Par.LocalSearchGreed;
	m_ChromoPopulation.resize(m_populationSize);
	firsttime =1;
	m_distMatrix = d;
	for(int i = 0; i < m_populationSize; i++)
	{
		CChromo p(m_noCitys , m_distMatrix);
		m_ChromoPopulation[i] = p;
	}
	m_fitness.resize(m_populationSize); 
	m_bestSoFarPath.resize(m_noCitys);
	for(int i = 0; i < m_noCitys; i++)
		m_bestSoFarPath[i] = i;
	m_iterations = 0;
	m_pLocalSearch = new CLocalSearch(m_distMatrix);
	calculateNearestNeigbhor();
	m_worstdistance = 0;
	m_startTime.startTimer();
}
CGeneticSystem::CGeneticSystem(int populationSize, int noNodes, double mutationRatio, bool doOpt2, bool doOpt3 , MatrixArrayTypeInt *d)
{
	this->m_populationSize = populationSize;
	this->m_noCitys = noNodes;
	this->m_mutationRatio = mutationRatio;
	this->m_bOpt2Search = doOpt2;
	this->m_bOpt3Search = doOpt3;
	m_ChromoPopulation.resize(populationSize);
	firsttime =1;
	m_distMatrix = d;
	for(int i = 0; i < populationSize; i++)
	{
		CChromo p(noNodes , m_distMatrix);
		m_ChromoPopulation[i] = p;
	}
	m_fitness.resize(populationSize); 
	m_bestSoFarPath.resize(noNodes);
	for(int i = 0; i < noNodes; i++)
		m_bestSoFarPath[i] = i;
	m_iterations = 0;
	m_pLocalSearch = new CLocalSearch(m_distMatrix);
	}
    

void CGeneticSystem::initSystem()
{
	for(int i = 0; i < m_populationSize; i++)
	{
		m_ChromoPopulation[i].shuffle();
	}

}

void CGeneticSystem::updateBestSoFarPath()
{
	double dist = m_ChromoPopulation[0].getDistance();
	double irrdist = getBestPathLength();
	if(dist > irrdist || firsttime == 1)
	{
		  m_bestSoFarPath =  m_ChromoPopulation[0].getPath();
		  m_tourStats.bestTourFounditter = m_iterations;
		  m_tourStats.bestTourToDate = m_bestSoFarPath;
		  m_tourStats.bestTourToDateLenght = dist;
		  m_tourStats.foundTime = m_startTime.getRunningTime();
		  m_tourStats.bestTourFounditter = m_iterations;


	}
	int worstdistance = computePathLength(getWorstPath());

	if(worstdistance > m_worstdistance)
	{
		m_worstdistance = worstdistance;
		m_tourStats.worstTourFounditter = m_iterations;
		m_tourStats.worstTourToDate = getWorstPath();
		m_tourStats.worseTourToDateLenght = m_worstdistance;

	}

	m_tourStats.worstTourLenghtItter = worstdistance;
	m_tourStats.bestTourLenghtItter = dist;

	
}

void CGeneticSystem::PopulateStats()
{

	std::vector<size_t> lengths(m_ChromoPopulation.size());
	for (size_t k = 0; k < m_ChromoPopulation.size(); k++) {
		lengths[k] = m_ChromoPopulation[k].getDistance();
	}
	double sum = std::accumulate(lengths.begin(), lengths.end(), 0.0);
	double mean = sum / lengths.size();
	double sq_sum = std::inner_product(lengths.begin(), lengths.end(), lengths.begin(), 0.0);
	double stdev = std::sqrt(sq_sum / lengths.size() - mean * mean);
	m_tourStats.branching_factor = 0;
	m_tourStats.Distance_betweenAnts = 0;
	m_tourStats.pop_stddev = stdev;
	m_tourStats.mean = mean; 
}
std::vector<size_t> & CGeneticSystem::getBestPath()
{
	SortPopulation(m_ChromoPopulation , true);
	return m_ChromoPopulation[0].getPath();
}
std::vector<size_t> & CGeneticSystem::getWorstPath()
{
	return m_ChromoPopulation[m_ChromoPopulation.size()-1].getPath();
}
double CGeneticSystem::getBestPathLength()
{
	SortPopulation(m_ChromoPopulation , true);
 	return static_cast<double>(m_ChromoPopulation[0].getDistance());
}
int CGeneticSystem::computePathLength(const std::vector<size_t> &Path)
{
	int len = 0;
	for(int i = 0; i < m_noCitys-1; i++){
		len+=(*m_distMatrix)[Path[i]][Path[i+1]];
	}
	len+=(*m_distMatrix)[Path[m_noCitys-1]][Path[0]];
	return len;
}
// Selects candidate tour for crossover
int CGeneticSystem::tournamentSelection()
{
	double sumFitness = 0.0;
	int idx1 = 0;
    for(int i = 0; i < m_populationSize; i++)
        sumFitness += m_fitness[i];
	double _rnd = rand();
	if(_rnd==0)
		_rnd++;
	double prob = ((double)_rnd / RAND_MAX) * sumFitness;
		
    double sum = m_fitness[0];
    while(sum < prob){
        idx1++;
        sum+=m_fitness[idx1];
    }
        
	return idx1;	
        
}
void CGeneticSystem::stepGeneration2()
{
	std::vector<CChromo> newPopulation(m_populationSize*2);
	for(int i = 0; i < m_populationSize*2; i++)
	{
		CChromo p(m_noCitys , m_distMatrix);
		newPopulation[i] = p;
	}

	int newPopulationSize = 0;
	SortPopulation(m_ChromoPopulation , false);
	computeFitness();

	for(int i = 0; i < m_populationSize; i++)
	{
		for(int j = 0; j < m_noCitys; j++)
		{
			int test = m_ChromoPopulation[i].getGene(j);
			newPopulation[i].setGene(j,test );
		}
		newPopulationSize++;
	}
     

	while (newPopulationSize < 2*m_populationSize) 
	{
		int idx1 = tournamentSelection();
		int idx2 = tournamentSelection();
		CChromo offspring = m_ChromoPopulation[idx1].CrossOver2(&m_ChromoPopulation[idx2]);
		newPopulation[newPopulationSize] = offspring;
		newPopulationSize++;
	}
  	mutatePopulation(newPopulation);
	SortPopulation(newPopulation , true);
	for(int i = 0; i < m_populationSize; i++)
	{
		m_ChromoPopulation[i] = newPopulation[i];

	//	m_ChromoPopulation[i].getPath().resize(m_noCitys+1);
	//	m_ChromoPopulation[i].getPath()[m_noCitys] = m_ChromoPopulation[i].getPath()[0] ;
	}
	SortPopulation(m_ChromoPopulation,true);
	updateBestSoFarPath();
}



void CGeneticSystem::constructSolutions()
{
	m_tourTime.startTimer();
	//stepGeneration2();
	//return;
	std::vector<CChromo> newPopulation(m_populationSize*2);
	for(int i = 0; i < m_populationSize*2; i++)
	{
		CChromo p(m_noCitys , m_distMatrix);
		newPopulation[i] = p;
	}
    int newPopulationSize = 0;
    SortPopulation(m_ChromoPopulation,false);
    computeFitness();
    for(int i = 0; i < m_populationSize; i++)
	{
	  for(int j = 0; j < m_noCitys; j++)
		{
			int test = m_ChromoPopulation[i].getGene(j);
            newPopulation[i].setGene(j,test );
        }
        newPopulationSize++;
    }
    while(newPopulationSize < 2*m_populationSize)
	{
        int idx1 =0; //tournamentSelection();
		int idx2 =0; //tournamentSelection();
		while(idx1 == idx2)
		{
			idx2= tournamentSelection();
			idx1= tournamentSelection();
		}
		CChromo &pfather=m_ChromoPopulation[idx2];
		CChromo &pMother=m_ChromoPopulation[idx1];
        CChromo p_offspring1(m_noCitys,m_distMatrix) , p_offspring2(m_noCitys,m_distMatrix);
		pMother.crossover(&pfather, &p_offspring1, &p_offspring2);
		newPopulation[newPopulationSize] = p_offspring1;
        newPopulationSize++;
		if(newPopulationSize >= newPopulation.size())
            break;
        newPopulation[newPopulationSize] = p_offspring2;
        newPopulationSize++;

    }
    mutatePopulation(newPopulation);
    SortPopulation(newPopulation , true);  //ass
    for(int i = 0; i < m_populationSize-2; i++) //keep last best
	{
		m_ChromoPopulation[i] = newPopulation[i];

	//	m_ChromoPopulation[i].getPath().resize(m_noCitys+1);
	//	m_ChromoPopulation[i].getPath()[m_noCitys] = m_ChromoPopulation[i].getPath()[0] ;
	}
    SortPopulation(m_ChromoPopulation , true);
	m_tourTime.stopTimer();
	m_tourStats.tourAverageTime = (m_tourTime.getElapsedTime())/  (m_populationSize);
    updateBestSoFarPath();
}
void CGeneticSystem::computeFitness()
{
    double selectionPressure = 2;
    for(int i = 0; i < m_populationSize; i++)
	{
        double fit = (double) 1*(i+1)/ (double) (m_populationSize-1);
        m_fitness[i] = fit;
    }
}
	

void CGeneticSystem::mutatePopulation(std::vector<CChromo> &popN)
{
    for(int i = 0; i < popN.size(); i++)
	{
		double rnd =((double) rand() / (RAND_MAX)) ;
		if(rnd < m_mutationRatio)
			popN[i].mutate();
    }
}

void CGeneticSystem::SortPopulation(std::vector<CChromo> &pop , bool Direction)
{
	bool done = false;
	while(!done){
		done = true;
		for(int i = 0; i < pop.size()-1; i++)
		{
			double from = (Direction) ? pop[i].getDistance() : pop[i+1].getDistance();
			double to = (Direction) ? pop[i+1].getDistance() : pop[i].getDistance();
			
			if(from > to)  //true
			{
				CChromo swap =pop[i];
				//swap.
				pop[i] = pop[i+1];
				pop[i+1] = swap;
				done = false;
			}
		}
	}
	
}
const std::vector<size_t> & CGeneticSystem::getBestSoFarPath()
{
	return m_bestSoFarPath;
}
double CGeneticSystem::getBestSoFarPathLength()
{
    return computePathLength(m_bestSoFarPath);
}
void CGeneticSystem::localSearch(){

	if(m_LocalSearchOpt2)
		for(int i = 0; i < m_populationSize; i++)
		{
			m_pLocalSearch->opt2(m_ChromoPopulation[i].getPath());
				//dawarin update see thesis
			int newdistance = this->computePathLength(m_ChromoPopulation[i].getPath());
			m_ChromoPopulation[i].setDistance(newdistance);
		
		}
	if(m_LocalSearchOpt3)
		for(int i = 0; i < m_populationSize; i++)
		{
			m_pLocalSearch->opt3(m_ChromoPopulation[i].getPath());
				//dawarin update see thesis
			int newdistance = this->computePathLength(m_ChromoPopulation[i].getPath());
			m_ChromoPopulation[i].setDistance(newdistance);

		}
	if(m_LocalSearchGreed)
		for(int i = 0; i <m_populationSize; i++)
		{
			//hack so can fit with thomas fuction
			m_ChromoPopulation[i].getPath().resize(m_noCitys+1);
			m_ChromoPopulation[i].getPath()[m_noCitys]= m_ChromoPopulation[i].getPath()[0];

			m_pLocalSearch->three_opt_first(m_ChromoPopulation[i].getPath() , m_nnList);
			//put it back
			m_ChromoPopulation[i].getPath().resize(m_noCitys);
			//dawarin update see dissertation
			int newdistance = this->computePathLength(m_ChromoPopulation[i].getPath());
			m_ChromoPopulation[i].setDistance(newdistance);
			
		}
	SortPopulation(m_ChromoPopulation , true);
    updateBestSoFarPath();
}

