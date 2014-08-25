#include "StdAfx.h"
#include "MutatedAnt1.h"


CMutatedAnt::CMutatedAnt(Parameters &Par, std::vector<std::vector<int> > *matrix)
		: CAntSystem(Par , matrix) 
{
	m_Ants.resize(5);
	m_noAnts=m_Ants.size();
	m_q0 = Par.q0;
	m_weight = Par.EliteWeight;
	if(!Par.EliteWeight)
		m_weight = matrix->size();
	m_noMut=Par.mutationRatio;

	//added
	m_weight = matrix->size();
	m_noMut=1.5;

}


void CMutatedAnt::decisionRule(size_t k, size_t step)
{
	size_t c = m_Ants[k].getCity(step-1); 
	double sum_prob =0;

	for (size_t i=0; i<m_noNodes; i++) 
	{
		t_prob[i] = 0.0;
		m_strength[i] = 0; 	
		if (m_Ants[k].isCityVisited(i) == false )	
		{
			t_prob[i]= (*m_heuristicMatrix)[c][i];
			sum_prob +=t_prob[i];
		}
	}
	for (size_t z =0; z < m_noNodes; z++)
		m_strength[z+1] = t_prob[z] + m_strength[z];
	double x = fRand(0,1.0)  * m_strength[m_noNodes];

	size_t j = 0;
	while (!((m_strength[j] <= x) && (x <= m_strength[j+1])))
		j++;

	size_t randomDecision =j;

	//find best neg
	double maxHeuristic = -1;
	size_t maxHeuristicIdx = 0;
	for(j = 0; j < m_noNodes; j++)
	{
		double choice = (*m_heuristicMatrix)[c][j];
		if(maxHeuristic < choice && !(m_Ants[k].isCityVisited(j))) //&& c!=j)
		{
			maxHeuristic = choice;
			maxHeuristicIdx = j;
		}
	}
	x=fRand(0,1.0);//choiceE(eng);

	if(x < m_q0)  
	{
		m_Ants[k].setAntCity(step, maxHeuristicIdx);
		m_Ants[k].setCityVisited(maxHeuristicIdx);
	}
	else //if exploitation
	{
		m_Ants[k].setAntCity(step, randomDecision);
		m_Ants[k].setCityVisited(randomDecision);
	}
}



CMutatedAnt::~CMutatedAnt(void)
{
}


