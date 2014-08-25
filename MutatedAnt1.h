#pragma once
#include "cantsystem.h"
class CMutatedAnt :
	public CAntSystem
{
	double m_q0;
	double tau0;
	int m_noMut;
	int m_weight;

	int m_riterations;
	
	void decisionRule(size_t k, size_t step);
	virtual void evaporateAllPhero()
	{
		if (m_iterations % (m_noNodes/10) == 0)
		{
			
			for(size_t i =1;i<m_noNodes;i++)
			{
				size_t idx1=m_pWorstAntItt->getAntsCityTour()[i-1];
				size_t idx2=m_pWorstAntItt->getAntsCityTour()[i];
				m_newPheromoneMatrix->evaporate(idx1,idx2);
			}
		}
		else
			for(size_t i =1;i<m_noNodes;i++)
		{
			size_t idx1=m_WorstAntToDate.getAntsCityTour()[i-1];
			size_t idx2=m_WorstAntToDate.getAntsCityTour()[i];
			m_newPheromoneMatrix->evaporate(idx1,idx2);
		}
		//	CAntSystem::evaporateAllPhero();

	}

public:
	virtual Tour getStats(){
		m_tourStats.bestTourToDate = m_BestAntToDate.getAntsCityTour();
		PopulateStats(m_Ants);
		return 	m_tourStats;
	}



	virtual void constructSolutions()
	{
		m_tourTime.startTimer();
		initAnts();
		for (size_t k = 0; k < m_Ants.size(); k++ )
		{
			m_Ants[k].setAntCity(0,m_randomPath[k]);
			m_Ants[k].setCityVisited(m_randomPath[k]);
		}
		for(size_t step = 1 ; step < m_noNodes; step++)
		{
			for(size_t k = 0; k < m_Ants.size(); k++)
				decisionRule(k,step);
		}
		uniform_int_distribution<size_t> rndSelTrsh2(0,m_noNodes-1);
		uniform_int_distribution<size_t> rndSelTrsh3(0,m_nnList[0].size()-1);
		for(size_t k = 0; k < m_Ants.size()-2; k++)
		{
			//caculate the mutation rate
			double nm = (1. / m_noNodes+ .5/(2*m_noNodes *this->m_riterations));
			m_noMut=(int) (nm) == 0 ? 1 : (int)(m_noMut/nm);
			
			for(size_t i = 0; i < m_noMut; i++)
			{
				size_t idx1 = rndSelTrsh2(g_rndTravel);
				size_t idx2 = rndSelTrsh2(g_rndTravel); 
		
				
				//int distanceBetween=distanceBetweenAnts(*m_pBestAntItt , m_BestAntToDate);
				
			
					size_t swap = m_Ants[k].getCity(idx1);
					size_t swap2 = m_Ants[k].getCity(idx2);
					m_Ants[k].setAntCity(idx2,swap);
					m_Ants[k].setAntCity(idx1,swap2);// = swap;
				
			
		}
		}
		for(size_t k = 0; k < m_Ants.size(); k++)
		{
			size_t tourstart=m_Ants[k].getCity(0);
			m_Ants[k].setAntCity(m_noNodes,tourstart);
			m_pLocalSearch->three_opt_first(m_Ants[k].getAntsCityTour() , m_nnList);
			m_Ants[k].setAntTourLength((int) this->calculatePathLength2(m_Ants[k].getAntsCityTour()));
		}
		m_tourStats.tourAverageTime = (m_tourTime.getElapsedTime())/  (m_Ants.size()-1);
		m_tourTime.stopTimer();
		updateBestSoFarPath();
	//	m_tourStats.bestTourToDate = m_BestAntToDate.getAntsCityTour();
	
	}
	virtual void update()
	{

		if(this->distanceBetweenAnts(*m_pBestAntItt , m_BestAntToDate) <= 3)
		{
			for (size_t  i=0; i<m_noNodes; i++)	
			{
				for (size_t j=0; j< m_noNodes; j++)
				{
					//this->m_newPheromoneMatrix->set(i, j , tau0);
				}
			}
			m_noMut=1;
		}
		else if(this->m_newPheromoneMatrix->averageLambdaBranchingFactor(m_lambda) < 1.000001)
		{
			for (size_t  i=0; i<m_noNodes; i++)	
			{
				for (size_t j=0; j< m_noNodes; j++)
				{
					this->m_newPheromoneMatrix->set(i, j , tau0);
				}
			}
			m_riterations=0;
		}
		else
		{
			evaporateAllPhero();
				//	int weight =m_BestAntToDate.getNoNodes();
			double d_tau = m_weight/(double)m_BestAntToDate.getAntTourLength(); 
			for(size_t city = 1; city <m_BestAntToDate.getNoNodes(); city++)
			{
				size_t from = m_BestAntToDate.getCity(city-1);
				size_t to = m_BestAntToDate.getCity(city);
				this->m_newPheromoneMatrix->add(from , to , d_tau);	
			}
			if (m_riterations % 25 == 0)
		{
		//	CAntSystem::update();
			
		}


		}
		calculateHeuristicMatrix();
		//if(this->m_iterations % 5 == 0)
			m_riterations++;

	}
	virtual void initSystem()
	{
		double best_distance = (std::numeric_limits<double>::max)();
		std::vector<size_t> randomPath = m_pLocalSearch->greedyPath(static_cast<int>(m_noNodes));
		double dist=this->calculatePathLength(randomPath);
		m_BestAntToDate.setAntsTour(randomPath);
		
		m_pLocalSearch->three_opt_first(randomPath , m_nnList);
		m_BestAntToDate.setAntsTour(randomPath);
		m_BestAntToDate.setAntTourLength(static_cast<int>(this->calculateAntPathLength(m_BestAntToDate)));
		tau0 = 1.0/ ((double)m_noNodes * dist);
		for (size_t  i=0; i<m_noNodes; i++)	
		{
			for (size_t j=0; j< m_noNodes; j++)
				this->m_newPheromoneMatrix->set(i, j , tau0);
		}
		//added
		initAnts();
		calculateHeuristicMatrix();
		m_riterations=1;
	}
	


public:
	CMutatedAnt(Parameters &Par, std::vector<std::vector<int> > *matrix);
	virtual ~CMutatedAnt(void);


};

