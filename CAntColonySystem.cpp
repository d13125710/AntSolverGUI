//************************************
// written by Gareth Hunter DIT Kevin Street artefact's for dissertation 
// MSc advanced software engineering Aug 2014
// The artefacts are available under the GNU license for download and modification
// any questions please email me at gar_hunter@yahoo.com
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//************************************
#include "StdAfx.h"
#include "StdAfx.h"
#include "CAntColonySystem.h"

#include <iostream>
#include <assert.h>  
#include <limits>       // std::numeric_limits



//************************************
// Method:    CAntColonySystem
// FullName:  CAntColonySystem::CAntColonySystem
// Access:    public 
// Returns:   
// Qualifier: : CAntSystem(Par , matrix)
// Parameter: Parameters & Par
// Parameter: MatrixArrayTypeInt * matrix
//************************************
CAntColonySystem::CAntColonySystem(Parameters &Par, MatrixArrayTypeInt *matrix)
			: CAntSystem(Par , matrix) 
{
	this->xi = Par.xi;
    this->q0 =Par.q0;
	this->m_r0 = Par.r0;
    this->tau0 = 0;
	this->m_noAnts=25;
	if(m_noNodes <= m_noAnts )
		m_noAnts = m_noNodes -1;

	//m_r0 =0.1;// Par.r0;
	//xi = 0.3;//Par.xi;
	//this->m_rho=0.1;
	//uniform_real<double> m_ChoiceExplore(0.0, 1);

}

CAntColonySystem::~CAntColonySystem(void)
{
	//clog.logMatrix(m_newPheromoneMatrix->getMatrix());   
}
//************************************
// Method:    evaporateAllPhero
// FullName:  CAntColonySystem::evaporateAllPhero
// Access:    virtual private 
// Returns:   void
// Qualifier:
//************************************
void CAntColonySystem::evaporateAllPhero()
{ 
	for (size_t i=1; i < m_BestAntToDate.getNoNodes(); i++){
		size_t idx1 = m_BestAntToDate.getCity(i); 
		size_t idx2 = m_BestAntToDate.getCity(i-1); 
		m_newPheromoneMatrix->multipy(idx1 , idx2 ,(1-m_r0));
	}
}
/*
In ACS only the globally best ant (i.e., the ant which constructed the shortest tour from the
beginning of the trial) is allowed to deposit pheromone. This choice, together with the use of
the pseudo-random-proportional rule, is intended to make the search more directed: Ants
search in a neighborhood of the best tour found up to the current iteration of the algorithm.
Global updating is performed after all ants have completed their tours. The pheromone level
is updated by applying the global updating rule of Eq. (4)
*/


 //************************************
 // Method:    globalPheromoneDeposit
 // FullName:  CAntColonySystem::globalPheromoneDeposit
 // Access:    public 
 // Returns:   void
 // Qualifier:
 //************************************
 void CAntColonySystem::globalPheromoneDeposit()
{
	double delta = 1.0 / ((double)(m_BestAntToDate.getAntTourLength()));
	for(size_t i=0;i<m_BestAntToDate.getAntsCityTour().size();i++) 
	{
		if(i==0)
			m_newPheromoneMatrix->add(m_noNodes-1, m_BestAntToDate.getCity(i) ,(m_r0*delta));
		else
			m_newPheromoneMatrix->add(m_BestAntToDate.getCity(i-1), m_BestAntToDate.getCity(i) , (m_r0*delta));
	}

/* weight * pheromones.get_evaporation_rate() * pheromone)
	for(size_t i = 1; i < m_BestAntToDate.getNoNodes(); i++)
	{ 
		size_t idx1 = m_BestAntToDate.getCity(i);
		size_t idx2 = m_BestAntToDate.getCity(i-1); 
	    this->m_newPheromoneMatrix->add(idx1,idx2 ,(m_r0*delta));
	}
*/	
}
 //************************************
 // Method:    CalculateHeuristicForNode
 // FullName:  CAntColonySystem::CalculateHeuristicForNode
 // Access:    public 
 // Returns:   void
 // Qualifier:
 // Parameter: size_t i
 // Parameter: size_t j
 //************************************
 void CAntColonySystem::CalculateHeuristicForNode(size_t i, size_t j)
 {
	 double xx = m_newPheromoneMatrix->get(i,j);
	 double yy = (*m_distanceMatrix)[i][j];
	 (*m_heuristicMatrix)[i][j] = abs(pow( xx , m_alpha) * pow(HEURISTIC(i,j),m_beta));
	 (*m_heuristicMatrix)[j][i] =  (*m_heuristicMatrix)[i][j] ;
 }
 //************************************
 // Method:    localPheromoneEndIndexUpdate
 // FullName:  CAntColonySystem::localPheromoneEndIndexUpdate
 // Access:    public 
 // Returns:   void
 // Qualifier:
 // Parameter: size_t idx1
 // Parameter: size_t idx2
 //************************************
 void CAntColonySystem::localPheromoneEndIndexUpdate(size_t idx1, size_t idx2)
 {
	// double delta = 1.0 / ((double)(m_BestAntToDate.getAntTourLength()));
	// double currentValue = this->m_newPheromoneMatrix->get(idx1, idx2);
	// double distance = (*m_distanceMatrix)[idx1][idx2];
	// double update=  (*m_heuristicMatrix)[idx1][idx2];

	// double updateValue =  (1.0-xi)*(currentValue+xi)* tau0;
	// this->m_newPheromoneMatrix->set(idx1, idx2 , updateValue);
	 this->m_newPheromoneMatrix->local_pheromone_update(idx1 , idx2 , xi , tau0);

	 CalculateHeuristicForNode(idx1, idx2);
 }
 //************************************
 // Method:    localPheromoneUpdate
 // FullName:  CAntColonySystem::localPheromoneUpdate
 // Access:    public 
 // Returns:   void
 // Qualifier:
 // Parameter: size_t ant
 // Parameter: size_t step
 //************************************
 void CAntColonySystem::localPheromoneUpdate(size_t ant, size_t step)
 {
	 size_t idx1 = this->m_Ants[ant].getCity(step);
	 size_t idx2 = m_Ants[ant].getCity(step-1);
	 localPheromoneEndIndexUpdate(idx1, idx2);
 } 

//************************************
// Method:    calculateHeuristicMatrix
// FullName:  CAntColonySystem::calculateHeuristicMatrix
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntColonySystem::calculateHeuristicMatrix()
{
	for(size_t i = 0; i < m_noNodes; i++)
		for(size_t j = 0; j <m_noNodes; j++)
		{
			CalculateHeuristicForNode(i, j);
		}	
}
//************************************
// Method:    updatePheromones
// FullName:  CAntColonySystem::updatePheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntColonySystem::update()
{ 
	
	//evaporateAllPhero(); //this
	//this->m_newPheromoneMatrix->evaporate_all();
	globalPheromoneDeposit();
	calculateHeuristicMatrix();
}
//************************************
// Method:    initPheromones
// FullName:  CAntColonySystem::initPheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntColonySystem::initSystem()
{
	double best_distance = (std::numeric_limits<double>::max)();
	std::vector<size_t> randomPath = m_pLocalSearch->greedyPath(m_noNodes);
	double dist=this->calculatePathLength(randomPath);
	m_BestAntToDate.setAntsTour(randomPath);
	if(m_LocalSearchOpt2)
		m_pLocalSearch->opt2(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchOpt3)
		m_pLocalSearch->opt3(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchGreed)
		m_pLocalSearch->three_opt_first(randomPath , m_nnList);
	m_BestAntToDate.setAntsTour(randomPath);
	m_BestAntToDate.setAntTourLength(this->calculateAntPathLength(m_BestAntToDate));
	tau0 = 1.0/ ((double)m_noNodes * dist);
	for (size_t  i=0; i<m_noNodes; i++)	
	{
		for (size_t j=0; j< m_noNodes; j++)
			this->m_newPheromoneMatrix->set(i, j , tau0);
	}
	//added
	initAnts();
	calculateHeuristicMatrix();
//	 clog.logMatrix(m_newPheromoneMatrix->getMatrix());  
}


//************************************
// Method:    decisionRule
// FullName:  CAntColonySystem::decisionRule
// Access:    virtual private 
// Returns:   void
// Qualifier:
// Parameter: size_t k
// Parameter: size_t step
// Parameter: uniform_real<double> & rndSelTrsh
// Parameter: uniform_real<double> & choiceE
//************************************
void CAntColonySystem::decisionRule(size_t k, size_t step, uniform_real<double> &rndSelTrsh, uniform_real<double> &choiceE)
{
     //roulette wheel selection  
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
		
		  std::tr1::mt19937 eng;
		//rndSelTrsh
	//	double x = rndSelTrsh(eng) * m_strength[m_noNodes];
		double x = fRand(0,1.0)  * m_strength[m_noNodes];
		
		int j = 0;
		while (!((m_strength[j] <= x) && (x <= m_strength[j+1])))
		  j++;
		
		size_t randomDecision =j;

		//find best neg
		double maxHeuristic = -1;
        int maxHeuristicIdx = -1;
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

        if(x < q0)  
		{
            m_Ants[k].setAntCity(step, maxHeuristicIdx);
            m_Ants[k].setCityVisited(maxHeuristicIdx);
        }
        else //if exploitation
		{
			//	std::cout << maxHeuristicIdx << ",";
            m_Ants[k].setAntCity(step, randomDecision);
            m_Ants[k].setCityVisited(randomDecision);
        }

}

//************************************
// Method:    constructSolutions
// FullName:  CAntColonySystem::constructSolutions
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntColonySystem::constructSolutions()
{
	m_tourTime.startTimer();
	//clear ants memory and shuffle start pos
	initAnts();
	//place ants in ramdom citys for starting
	for (size_t k = 0; k <  m_Ants.size(); k++ )
	{
		m_Ants[k].setAntCity(0,m_randomPath[k]);
		m_Ants[k].setCityVisited(m_randomPath[k]);
	}
	uniform_real<double> rndSelTrsh(0.0, 1.0);
	uniform_real<double> m_ChoiceExplore(0.0 , 1.0);

	for(size_t step = 1 ; step < m_noNodes; step++)
	{
		for(size_t k = 0; k < m_Ants.size(); k++)
		{
			decisionRule(k,step, rndSelTrsh, m_ChoiceExplore);
		//	localPheromoneUpdate(k,m_Ants[k].getCity(step));
		}
	}

	for(size_t k = 0; k <  m_Ants.size(); k++)
	{
		int tourstart=m_Ants[k].getCity(0);
		m_Ants[k].setAntCity(m_noNodes,tourstart);
		m_Ants[k].setAntTourLength((int)this->calculateAntPathLength(m_Ants[k]));
	//	size_t idx1 = this->m_Ants[k].getCity(m_noNodes);
	//	size_t idx2 = m_Ants[k].getCity(m_noNodes-1);

	//	m_newPheromoneMatrix->local_pheromone_update(m_noNodes-1, m_Ants[k].getAntsCityTour()[0] ,xi, tau0);
	//	localPheromoneEndIndexUpdate(idx1, idx2);
	}
	
	for(size_t k = 0; k < m_Ants.size(); k++)
		for(size_t i=0;i<m_Ants[k].getAntsCityTour().size();i++) 
		{
			if(i==0)
				m_newPheromoneMatrix->local_pheromone_update(m_noNodes-1, m_Ants[k].getAntsCityTour()[i] ,xi, tau0);
			else 
				m_newPheromoneMatrix->local_pheromone_update(m_Ants[k].getAntsCityTour()[i-1], m_Ants[k].getAntsCityTour()[i],xi , tau0 );
		}
	
	m_tourStats.tourAverageTime = (m_tourTime.getElapsedTime())/  (m_Ants.size()-1);
	m_tourTime.stopTimer();
	updateBestSoFarPath();
}