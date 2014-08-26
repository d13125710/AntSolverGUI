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
#include "BestWorstAntSystem.h"
#include "resource.h"
#include <limits>       // std::numeric_limits
#include "Utils.h"



//************************************
// Method:    CBestWorstAntSystem
// FullName:  CBestWorstAntSystem::CBestWorstAntSystem
// Access:    public 
// Returns:   
// Qualifier: : CAntSystem(Par , distanceMatrix)
// Parameter: Parameters & Par
// Parameter: MatrixArrayTypeInt * distanceMatrix
//************************************
CBestWorstAntSystem::CBestWorstAntSystem(Parameters& Par,MatrixArrayTypeInt *distanceMatrix) 
	: CAntSystem(Par , distanceMatrix)
{
	restart_iteration =1;
	starttime=GetTickCount();
	std::vector<size_t>  randomPath = m_pLocalSearch->greedyPath(static_cast<int>(m_noNodes));
	m_BestAntToDate.setAntsTour(randomPath);
	//m_pLocalSearch->opt3(m_BestAntToDate.getAntsCityTour());

	if(m_LocalSearchOpt2)
		m_pLocalSearch->opt2(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchOpt3)
		m_pLocalSearch->opt3(m_BestAntToDate.getAntsCityTour());
	else if(m_LocalSearchGreed)
		m_pLocalSearch->three_opt_first(m_BestAntToDate.getAntsCityTour() , m_nnList);

	(m_BestAntToDate.getAntsCityTour()).resize(m_noNodes+1);

	m_BestAntToDate.getAntsCityTour()[m_noNodes] = m_BestAntToDate.getAntsCityTour()[0];

	m_BestAntToDate.setAntTourLength(static_cast<int>(this->calculateAntPathLength(m_BestAntToDate)));
	trail_0 = 1.0 / (m_BestAntToDate.getAntTourLength());
}
/*    
      FUNCTION:      uses additional evaporation on the arcs of iteration worst
                     ant that are not shared with the global best ant
      INPUT:         pointer to the worst (a1) and the best (a2) ant
      OUTPUT:        none
      (SIDE)EFFECTS: pheromones on some arcs undergo additional evaporation
*/
void CBestWorstAntSystem::BWUpdate( const CAnt &worseA, const CAnt &best)
{  
	size_t   i, pos, pred;
	unsigned int   distance=0;

	std::vector<size_t> pos2(m_noNodes+1);
	for ( i = 0 ; i < best.getNoNodes() ; i++ ) 
	{
		pos2[best.getCity(i)] = i;
	}
	for ( i = 1 ; i < best.getNoNodes()   ; i++ ) 
	{
		size_t to = worseA.getCity(i);
		size_t from = worseA.getCity(i-1);
		pos = pos2[to];

		size_t bestnextpos=-1;
		size_t bestprevpos=-1;

		if(pos+1 > m_noNodes)
			pred = m_noNodes - 1;
		else if(pos-1 < 0)
		{
			pred = 0;
			bestnextpos = best.getCity(0);
			bestprevpos = best.getCity(m_noNodes - 1);
		}
		else if(pos == 0)
		{
			pred = 0;
			bestnextpos = best.getCity(0);
			bestprevpos = best.getCity(m_noNodes - 1);
		}

		else
		{
			pred = pos;
			bestnextpos = best.getCity(pred+1);
			bestprevpos = best.getCity(pred-1);

		}

		if (bestnextpos == from)
			; /* do nothing, edge is common with a2 (best solution found so far) */
		else if (bestprevpos == from)
			; /* do nothing, edge is common with a2 (best solution found so far) */
		else 
		{   /* edge (j,h) does not occur in ant a2 */       
			m_newPheromoneMatrix->evaporate(to,from);
		//	m_newPheromoneMatrix->evaporate1(from,to);

			//(*m_pheromoneMatrix)[j][h] = (1 - m_rho) * (*m_pheromoneMatrix)[j][h];
			//(*m_pheromoneMatrix)[h][j] = (1 - m_rho) * (*m_pheromoneMatrix)[h][j];
		}
	}

}

//************************************
// Method:    initPheromones
// FullName:  CBestWorstAntSystem::initPheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CBestWorstAntSystem::initPheromones() 
{
	initPheromoneTrails(trail_0);
	//CAntSystem::initPheromones();
	calculateHeuristicMatrix();
}

//************************************
// Method:    initPheromoneTrails
// FullName:  CBestWorstAntSystem::initPheromoneTrails
// Access:    public 
// Returns:   void
// Qualifier: const
// Parameter: double initialValue
//************************************
void CBestWorstAntSystem::initPheromoneTrails(double initialValue) const
{
	size_t i, j;
	for (i = 0; i < m_noNodes; i++)
	{
		for (j = 0; j < i; j++) 
		{
			m_newPheromoneMatrix->set(i , j , initialValue);
		}
	}

	for(i = 0; i < m_noNodes; i++)
		m_newPheromoneMatrix->set(i , i , 0);
	
}


//The pheromone update is a little bit different: on each iteration, the best to date ant deposits an additional quantity of pheromone on paths it traveled:
//************************************
// Method:    updatePheromones
// FullName:  CBestWorstAntSystem::updatePheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CBestWorstAntSystem::updatePheromes()
{

	ASSERT(m_BestAntToDate.getNoNodes() == m_noNodes +1 );

	double delta = 1.0 / ((double)(m_BestAntToDate.getAntTourLength()));
	for(size_t i=0;i<m_BestAntToDate.getAntsCityTour().size();i++) 
	{
		if(i==0)
			m_newPheromoneMatrix->add(m_noNodes-1, m_BestAntToDate.getCity(i) ,(delta));
		else
			m_newPheromoneMatrix->add(m_BestAntToDate.getCity(i-1), m_BestAntToDate.getCity(i) , (delta));
	}
	BWUpdate(*m_pWorstAntItt , m_BestAntToDate );

	long distance_best_worst = distanceBetweenAnts(m_BestAntToDate , *m_pWorstAntItt);
	long reset =  (long) (0.05 * m_noNodes);
	if ( distance_best_worst < reset ) 
	{
		//m_RestartBestAnt.setAntTourLength((std::numeric_limits<int>::max)());
		initPheromoneTrails(trail_0);
		restart_iteration = m_iterations;
		restart_time = GetTickCount();
	}
	else 
	{
		Mutation();
	}

	calculateHeuristicMatrix();
 }
void CBestWorstAntSystem::Mutation( void )
{
	double mutation_rate = 0.2;
	/* compute average pheromone trail on edges of global best solution */
	double avg_trail = this->m_newPheromoneMatrix->averagePheromoneTrail(m_BestAntToDate.getAntsCityTour());
	//solution to complex forumla thos strudle
	double mutation_strength = 4.0 * (double)avg_trail * (double) (GetTickCount() - restart_time) / (3000000 - (restart_time + 0.0001) );
	

	mutation_rate = mutation_rate / this->m_noNodes *m_noAnts;
	size_t num_mutations = (int) (this->m_noNodes * mutation_rate) / 2;
    if ( restart_iteration < 2 )
	     num_mutations = 0; 

    for (size_t i = 0 ; i < num_mutations ; i++ ) 
	{
		int j =   (int) (fRand(0.0, 1.0) * (double)m_noNodes-1) ;
		int k =   (int) (fRand(0.0, 1.0) * (double)m_noNodes-1) ;
		if(j!=k)
		{
			double chance =  fRand(0.0,1.0);
			if ( chance < 0.5 ) 
			{
				this->m_newPheromoneMatrix->add(j,k , mutation_strength);
			}
			else 
			{	
				this->m_newPheromoneMatrix->subtract(j,k , mutation_strength);

			}
		}
    }
}




CBestWorstAntSystem::~CBestWorstAntSystem(void)
{
}
