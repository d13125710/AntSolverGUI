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
#include "CAntSystem.h"
#include "resource.h"

#include <numeric>






//************************************
// Method:    CAntSystem
// FullName:  CAntSystem::CAntSystem
// Access:    public 
// Returns:   
// Qualifier:
// Parameter: Parameters & Par
// Parameter: MatrixArrayTypeInt * distanceMatrix
//************************************
CAntSystem::CAntSystem(Parameters& Par, MatrixArrayTypeInt *distanceMatrix) : 
	m_distanceMatrix(distanceMatrix) 
{
	//m_distanceMatrix = distanceMatrix;
	Create(Par);
   
}
//************************************
// Method:    Create
// FullName:  CAntSystem::Create
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: Parameters & Par
//************************************
void CAntSystem::Create(Parameters& Par)
{
	this->m_noNodes = m_distanceMatrix->size();
	if(Par.noAnts > m_noNodes)
		Par.noAnts = (int) m_noNodes-1;
	this->m_noAnts = Par.noAnts;
    this->m_alpha = Par.Alpha;
    this->m_beta = Par.Beta;
    this->m_rho = Par.rho;
	m_LocalSearchOpt2 = Par.LocalSearchOpt2;
	m_LocalSearchOpt3 = Par.LocalSearchOpt3;
	m_LocalSearchGreed = Par.LocalSearchGreed;
	this->m_iterations = 0;
	irreationSinceLastBest=0;

	m_RestartBestAnt.setAntTourLength(0);

	m_pLocalSearch = new CLocalSearch(m_distanceMatrix);
 
	m_heuristicMatrix= new std::vector<std::vector<double> >(m_noNodes , m_noNodes);
	m_newPheromoneMatrix = new PheroMatrix(m_noNodes,m_rho);


	m_Ants.empty();
	m_Ants.resize(m_noAnts-1);
	for(int i = 0; i < m_Ants.size(); i++)
	{
		CAnt newAnt(m_noNodes);
		m_Ants[i] = newAnt;
	}
	

	m_randomPath.resize(m_noNodes);
	for (size_t i=0; i<m_noNodes; i++)
		m_randomPath[i]=i;
	std::random_shuffle( m_randomPath.begin() , m_randomPath.end() );
	//rndSelTrsh(g_rndTravel);
	t_prob.resize(m_noNodes);
	m_strength.resize(m_noNodes+1);
	
	//new
	m_RestartBestAnt.setAntTourLength( (std::numeric_limits<int>::max)());
	m_WorstAntToDate.setAntTourLength(0);
	m_BestAntToDate.setAntTourLength((std::numeric_limits<int>::max)());

	calculateNearestNeigbhor();

	m_lambda = Par.lambda; 

	g_rndTravel.seed(time(0));

	m_startTime.startTimer();

	m_tourStats.foundTime = 0.00001; // set to a small value as ma finds solutions almost quicker than the timer

}

//************************************
// Method:    updatePheromones
// FullName:  CAntSystem::updatePheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void  CAntSystem::update()
{
    evaporateAllPhero();
    for(size_t k = 0; k < m_Ants.size(); k++)
        adjustPheremone(k);
	//dont think i need this on irreration
    calculateHeuristicMatrix();
}

//************************************
// Method:    initMatrix
// FullName:  CAntSystem::initMatrix
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: std::vector<std::vector<int> > & distanceMatrix
//************************************
void CAntSystem::initMatrix(std::vector<std::vector<int> > &distanceMatrix)
{
    size_t i,j;
	this->m_distanceMatrix = &distanceMatrix;
     
	for(i = 0; i < m_noNodes; i++)
		for(j = 0; j < m_noNodes; j++)
		{
			m_newPheromoneMatrix->set(i,j,0.0);
			(*m_heuristicMatrix)[i][j] = 0.0;
		}
}
 void CAntSystem::initPheromones()
{
	for (size_t i=0; i<m_noNodes; i++)	{
		for (size_t j=0; j<m_noNodes; j++)
			m_newPheromoneMatrix->set(i,j, (0.1 * rand() / (double)RAND_MAX));
	}
    
}
 void CAntSystem::initAnts()
 {
     for(size_t i = 0; i < m_Ants.size(); i++)
		m_Ants[i].ReinitiseAnt();
//	shuffle(m_Randomnseq.begin(), m_Randomnseq.end(), m_generator);

 }

void CAntSystem::adjustPheremone(size_t antPos)
{
	const CAnt &pant=m_Ants[antPos];
	double tour_length = (double) (1.0 / pant.getAntTourLength());
	for(size_t city = 0; city < pant.getNoNodes()-1; city++)
	{
	      
		size_t from = m_Ants[antPos].getCity(city);
		size_t to   = m_Ants[antPos].getCity(city+1);
		// eq 14.2 / 14.3
		m_newPheromoneMatrix->add(from , to , tour_length );	
	}

}
//************************************
// Method:    distance_between_ants
// FullName:  CAntSystem::distance_between_ants
// Access:    protected 
// Returns:   long
// Qualifier:
// Parameter: const CAnt & worseA
// Parameter: const CAnt & best
//************************************
long CAntSystem::distanceBetweenAnts( const CAnt &worseA, const CAnt &best)
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
			bestnextpos = best.getCity(pred+1);
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
			distance++;

			//(*m_pheromoneMatrix)[j][h] = (1 - m_rho) * (*m_pheromoneMatrix)[j][h];
			//(*m_pheromoneMatrix)[h][j] = (1 - m_rho) * (*m_pheromoneMatrix)[h][j];
		}
	}
	return distance;
}

//************************************
// Method:    decisionRule
// FullName:  CAntSystem::decisionRule
// Access:    public 
// Returns:   void
// Qualifier:
// Parameter: size_t k
// Parameter: size_t step
//************************************
void CAntSystem::decisionRule(size_t k, size_t step, normal_distribution<double> &rndSelTrsh)
{
	size_t c = m_Ants[k].getCity(step-1); 
	for (size_t i=0; i < m_noNodes; i++) 
	{
		t_prob[i] = 0.0;
		m_strength[i] = 0;	
		if (m_Ants[k].isCityVisited(i) == false && c!=i)	
		{
			try
			{
				t_prob[i]= (double)(*m_heuristicMatrix)[c][i];
			}
			catch (...)
			{
				TRACE("Exception k=%d , step=%d , i=%d , c=%d" , k , step , i ,c);
			}
			
		}
	}
	for (size_t z =0; z < m_noNodes; z++)
		m_strength[z+1] = t_prob[z] + m_strength[z];
	
//	double x = rndSelTrsh(g_rndTravel) * m_strength[m_noNodes];
	double x = fRand(0, 1.0) * m_strength[m_noNodes];
	size_t y = 0;
	//while (  y !=m_strength.size() && !((m_strength[y] <= x) && (x <= m_strength[y+1])) )
	while (!((m_strength[y] <= x) && (x <= m_strength[y+1])) )
		y++;

	m_Ants[k].setAntCity(step, y);
	m_Ants[k].setCityVisited(y);

}


//************************************
// Method:    constructSolutions
// FullName:  CAntSystem::constructSolutions
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntSystem::constructSolutions()
{
	m_tourTime.startTimer();
	initAnts();
	for (size_t k = 0; k < m_Ants.size(); k++ )
	{
		m_Ants[k].setAntCity(0,m_randomPath[k]);
		m_Ants[k].setCityVisited(m_randomPath[k]);
	}
	normal_distribution<double> rndSelTrsh(0.0,1.0);
	rndSelTrsh(g_rndTravel);
	for(size_t step = 1 ; step < m_noNodes; step++)
	{
		for(size_t k = 0; k < m_Ants.size(); k++)
			decisionRule(k,step, rndSelTrsh);
	}
    //add in the last city to the first city tour ...  
	for(size_t k = 0; k < m_Ants.size(); k++)
	{
		size_t tourstart=m_Ants[k].getCity(0);
		m_Ants[k].setAntCity(m_noNodes,tourstart);
		m_Ants[k].setAntTourLength((int) this->calculatePathLength2(m_Ants[k].getAntsCityTour()));
	}
	m_tourStats.tourAverageTime = (m_tourTime.getElapsedTime())/  (m_Ants.size()-1);
	m_tourTime.stopTimer();
	updateBestSoFarPath();
}
//************************************
// Method:    evaporateAllPhero
// FullName:  CAntSystem::evaporateAllPhero
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntSystem::evaporateAllPhero()
{
	m_newPheromoneMatrix->evaporate_all();
}

//************************************
// Method:    calculatePathLength
// FullName:  CAntSystem::calculatePathLength
// Access:    public 
// Returns:   double
// Qualifier: const
// Parameter: const std::vector<size_t> & currentPath
//************************************
double CAntSystem::calculatePathLength(const std::vector<size_t> &currentPath) const
{
    size_t  start, end;
	double d = 0;
	for ( size_t i=1; i<currentPath.size(); i++)
	{
		start = currentPath[i-1];
		end = currentPath[i];
		d += (*m_distanceMatrix)[start][end];	
	}
	d += (*m_distanceMatrix)[currentPath[m_noNodes-1]][currentPath[0]];
	return d;
}
double CAntSystem::calculatePathLength2(const std::vector<size_t> &currentPath) const
{
	size_t  start, end;
	double d = 0;
	for ( size_t i=1; i<currentPath.size(); i++)
	{
		start = currentPath[i-1];
		end = currentPath[i];
		d += (*m_distanceMatrix)[start][end];	
	}
//	d += (*m_distanceMatrix)[currentPath[m_noNodes-1]][currentPath[0]];
	return d;
}
//************************************
// Method:    calculateHeuristicMatrix
// FullName:  CAntSystem::calculateHeuristicMatrix
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntSystem::calculateHeuristicMatrix()
{
	for(size_t i = 0; i < m_noNodes; i++)
		for(size_t j = 0; j <m_noNodes; j++)
		{
			double xx = m_newPheromoneMatrix->get(i,j);
			double yy = (*m_distanceMatrix)[i][j];
			(*m_heuristicMatrix)[i][j] =(double) (yy) ?  abs(pow( xx , m_alpha) / pow(yy,m_beta)) :  abs(pow( xx , m_alpha) / pow(1,m_beta));
		}	
}
//************************************
// Method:    updateBestSoFarPath
// FullName:  CAntSystem::updateBestSoFarPath
// Access:    virtual public 
// Returns:   void
// Qualifier:
//************************************
void CAntSystem::calculateNearestNeigbhor(size_t NUMOFANTS/*=20*/)
{
	if(NUMOFANTS >= m_noNodes)
		NUMOFANTS= m_noNodes -1;
	
	size_t *pHelpArray = new size_t[m_noNodes];
	double *pdistanceArray = new double[m_noNodes ];
	*(pdistanceArray) =(std::numeric_limits<double>::max)();

	m_nnList.resize(m_noNodes);
	for (size_t i = 0; i < m_noNodes; ++i)
		m_nnList[i].resize(NUMOFANTS);

	for (size_t node = 0; node < m_noNodes; node++) 
	{ 

		for (size_t i = 0; i < m_noNodes; i++) 
		{
			*(pdistanceArray+i)=(*m_distanceMatrix)[node][i];
			*(pHelpArray+i)=  i;
		}
		double max = (std::numeric_limits<double>::max)() - 1; 
		*(pdistanceArray+node) =  (std::numeric_limits<double>::max)();  // set to a large value .. 
		this->m_pLocalSearch->sort2(pdistanceArray, pHelpArray, 0, (m_noNodes - 1));
		for (size_t i = 0; i < NUMOFANTS; i++) 
		{
			m_nnList[node][i] = *(pHelpArray+i);
		}
	}
	delete [] pHelpArray;
	delete [] pdistanceArray;
}
bool CAntSystem::updateBestSoFarPath()
{

	//find the worst ant , best ant , and

	bool bFoundUpdate =false;
	int pathdistance =(std::numeric_limits<int>::max)();
	int worstdistance = 0;

	m_tourStats.currentIrreation = m_iterations;
	
	for(size_t a =0;a<m_Ants.size(); a++)
	{
		int antPathLength = m_Ants[a].getAntTourLength();
		if(antPathLength < pathdistance)
		{
			pathdistance = antPathLength;
			m_pBestAntItt = &m_Ants[a];
			m_tourStats.bestTourLenghtItter =pathdistance;
		}
		if(antPathLength > worstdistance)
		{
			worstdistance = antPathLength;
			m_pWorstAntItt = &m_Ants[a];
			m_tourStats.worstTourLenghtItter =worstdistance;
		}
	}
	if(m_pBestAntItt->getAntTourLength() < m_RestartBestAnt.getAntTourLength())
	{
		m_RestartBestAnt = *m_pBestAntItt;
		std::string s =std::string(format("Restart best Ants %d @ %d" , m_RestartBestAnt.getAntTourLength() ,m_iterations));
		this->m_tourStats.extrainfo =s;

		//m_tourStats.foundTime = m_startTime.getElapsedTime();
		//static string c;c=s;
		//AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&c);
		bFoundUpdate = true;

	}

	if(m_pWorstAntItt->getAntTourLength() > m_WorstAntToDate.getAntTourLength())
	{
		m_WorstAntToDate = *m_pWorstAntItt;
		std::string s =format("Worst Ant to date found  %f  @ %d",m_WorstAntToDate.getAntTourLength() ,m_iterations);
		static string c; c=s;
		this->m_tourStats.extrainfo =s;
		m_tourStats.worstTourToDate = m_WorstAntToDate.getAntsCityTour();
		//worseTourToDateLenght
		m_tourStats.worseTourToDateLenght = m_WorstAntToDate.getAntTourLength();
		m_tourStats.worstTourFounditter = m_iterations;
		//AfxGetMainWnd()->PostMessage(IDC_VERBOSE ,0, (LPARAM)&c);
		bFoundUpdate=true;
	}
	if(m_pBestAntItt->getAntTourLength() < m_BestAntToDate.getAntTourLength())
	{
		m_tourStats.foundTime = m_startTime.getRunningTime();
		m_BestAntToDate = *m_pBestAntItt;
		irreationSinceLastBest=0;
		m_tourStats.bestTourToDate = this->m_BestAntToDate.getAntsCityTour();
		m_tourStats.bestTourToDateLenght = this->m_BestAntToDate.getAntTourLength();
		m_tourStats.bestTourFounditter = m_iterations;
		//	m_tourStats.bestTourToDate = m_BestAntToDate.getAntsCityTour();
		bFoundUpdate =true;
	}
	else
	{
		irreationSinceLastBest++;
	}

	return bFoundUpdate;
}
//************************************
// Method:    PopulateStats
// FullName:  CAntSystem::PopulateStats
// Access:    protected 
// Returns:   void
// Qualifier:
// Parameter: std::vector<CAnt> & AntPaths
//************************************
void CAntSystem::PopulateStats(std::vector<CAnt> &AntPaths)
{
	std::vector<size_t> lengths(AntPaths.size());
	for (size_t k = 0; k < AntPaths.size(); k++) {
		lengths[k] = AntPaths[k].getAntTourLength();
	}
	double sum = std::accumulate(lengths.begin(), lengths.end(), 0.0);
	double mean = sum / lengths.size();
	double sq_sum = std::inner_product(lengths.begin(), lengths.end(), lengths.begin(), 0.0);
	double stdev = std::sqrt(sq_sum / lengths.size() - mean * mean);
	double branching_factor = m_newPheromoneMatrix->averageLambdaBranchingFactor(m_lambda);
	double avg_distance =0.0;
	for (size_t k = 0; k < AntPaths.size(); k++)
		for (size_t j = k + 1; j <AntPaths.size(); j++) 
		{
			avg_distance += (double) distanceBetweenAnts(AntPaths[k], AntPaths[j]);
		}
		avg_distance /= ((double) AntPaths.size() * (double) (AntPaths.size()) / 2.0);
	
	m_tourStats.branching_factor = branching_factor;
	m_tourStats.Distance_betweenAnts = avg_distance;
	m_tourStats.pop_stddev = stdev;
	m_tourStats.mean = mean; 
	m_tourStats.bestTourToDateLenght =m_BestAntToDate.getAntTourLength();




}


//************************************
// Method:    ~CAntSystem
// FullName:  CAntSystem::~CAntSystem
// Access:    virtual public 
// Returns:   
// Qualifier:
// Parameter: void
//************************************
CAntSystem::~CAntSystem(void)
{
	delete m_heuristicMatrix;
	delete m_pLocalSearch;
	delete m_newPheromoneMatrix;
}

