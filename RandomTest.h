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
#pragma once

#include <random>
#include "CLocalSearch.h"
#include "Utils.h"



class CRandomAnt : public iProblemSolver
{
	MatrixArrayTypeInt  *m_distanceMatrix;
	std::vector<std::vector<size_t> > m_nnList;
	std::vector<size_t> m_randomPath;

	std::vector<std::vector<size_t>> m_Tours;
	std::vector<size_t> m_besttour;
	std::vector<size_t> m_Restarttour;
	size_t m_noNodes;
	double m_mutationRatio;
	long m_stag;

	int restarts;
	size_t m_nomut;

	int worst_distance;


	std::mt19937 g_rndTravel;

	void calculateNearestNeigbhor(unsigned int NUMOFANTS=20)
	{
		if(NUMOFANTS >= m_noNodes)
			NUMOFANTS= m_noNodes -1;

		size_t *pHelpArray = new size_t[m_noNodes];
		double *pdistanceArray = new double[m_noNodes ];
		*(pdistanceArray) =(std::numeric_limits<double>::max)();

		m_nnList.resize(m_noNodes);
		for (size_t  i = 0; i < m_noNodes; ++i)
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
			this->m_pLocalSearch->sort2(pdistanceArray, pHelpArray, 0, static_cast<unsigned int>(m_noNodes - 1));
			for (size_t i = 0; i < NUMOFANTS; i++) 
			{
				m_nnList[node][i] = *(pHelpArray+i);
			}
		}
		delete [] pHelpArray;
		delete [] pdistanceArray;
	}

	CLocalSearch *m_pLocalSearch;

	
public:
	int m_mut;
	double current_distance;

	double calculatePathLength2(const std::vector<size_t> &currentPath) const
	{
		size_t  start, end;
		double d = 0;
		for ( size_t i=1; i<currentPath.size(); i++)
		{
			start = currentPath[i-1];
			end = currentPath[i];
			d += (*m_distanceMatrix)[start][end];	
		}
		return d;
	}
	
	const virtual std::vector<size_t> &getBestSoFarPath()	{
		return m_besttour;
	}
	void mutatePopulation(int mut , std::vector<size_t> &tour){
		tour.resize(m_noNodes); //strip of last
	
		uniform_int_distribution<int> rndSelTrsh2(0,m_noNodes-1);

		for(size_t i = 0; i < mut; i++)
		{
			//double rnd = rndSelTrsh(g_rndTravel); 

			int idx1 = rndSelTrsh2(g_rndTravel);//getRandomNumber(0, m_noNodes-1);
			int idx2 = rndSelTrsh2(g_rndTravel); //getRandomNumber(0,m_noNodes-1, idx1);
			size_t swap = tour[idx1];
			tour[idx1] = tour[idx2];
			tour[idx2] = swap;
		}
		tour.resize(m_noNodes+1); //strip of last
		tour[m_noNodes] = tour[0];///strip of last
	}

	void shuffle()
	{
		m_randomPath.resize(m_noNodes);
		for (size_t i=0; i<m_noNodes; i++)
			m_randomPath[i]=i;
		std::random_shuffle( m_randomPath.begin() , m_randomPath.end() );

		m_randomPath.resize(m_noNodes+1);
		m_randomPath[m_noNodes] = m_randomPath[0];
	}

	void restart()
	{
		m_randomPath = m_Restarttour;
		for(size_t i=0;i< m_Tours.size()/2 ;i++)
		{
			m_Tours[i] = m_Restarttour;
			m_Tours[i+m_Tours.size()/2] = m_besttour;
		}
	}

	CRandomAnt(Parameters &par , MatrixArrayTypeInt  *distanceMatrix)
	{
		m_distanceMatrix = distanceMatrix;
		m_pLocalSearch = new CLocalSearch(m_distanceMatrix);
		m_mutationRatio=  0.9;
		m_noNodes = m_distanceMatrix->size();
		m_stag=0;
		//m_mutationRatio =1.0/ par.q0;
		m_stag=0;
		restarts=0;
		m_mut= par.Beta;
		m_nomut=m_noNodes/10; 
	}

	virtual Tour getStats()
	{
		return m_tourStats;
	}

	virtual void constructSolutions(){
		
		m_tourTime.startTimer();
	//	initAnts();
		uniform_int_distribution<int> rndSelTrsh2(0,m_nomut);
			

		for(size_t i=0;i<m_Tours.size()/2;i++)
		{
			int idx1 = rndSelTrsh2(g_rndTravel);
			mutatePopulation(idx1 ,  m_Tours[i]);

			m_pLocalSearch->three_opt_first( m_Tours[i] , m_nnList);
			//mutatePopulation(m_mut);

			double n_distance  = calculatePathLength2(m_Tours[i]);
			if(n_distance < current_distance)
			{
				current_distance = n_distance;
				m_besttour = m_Tours[i];
				m_tourStats.bestTourToDateLenght = n_distance;
				m_tourStats.bestTourToDate = m_besttour;
				m_tourStats.bestTourFounditter = m_iterations;



			}
			if(n_distance > worst_distance)
			{
				worst_distance = n_distance;
				m_tourStats.worseTourToDateLenght = n_distance;
				m_tourStats.worstTourToDate = m_Tours[i];
	
			}
			else
			{
				m_stag++;
			}

		}
	
		m_tourStats.tourAverageTime = (m_tourTime.getElapsedTime())/  (20);
		m_tourTime.stopTimer();
		//mutatePopulation(this->m_mutationRatio);
	}
	virtual void localSearch(){
		
	};
	virtual void update()
	{
		if(m_stag > 200)
		{
			restart();
			restarts++;
			m_nomut = m_nomut -1;
			if(m_nomut == 0)
				m_nomut = 5;

			if(restarts > 10)
			{
				restarts =0;
			//	m_mut=1;
			}
			m_stag=0;
		}


	}

	virtual void initSystem()
	{
		calculateNearestNeigbhor((int)m_noNodes);
		worst_distance =0;
		double current_mindistance = (std::numeric_limits<int>::max)();

		m_Tours.resize(2);
		for(size_t i=0;i< m_Tours.size();i++ )
		{
			m_Tours[i].resize(m_noNodes);
			for (size_t t=0; t<m_noNodes; t++)
				m_Tours[i][t]=t;
			std::random_shuffle( m_Tours[i].begin() , m_Tours[i].end() );
			m_Tours[i].resize(m_noNodes+1);
			m_Tours[i][m_noNodes] = m_Tours[i][0];
			m_pLocalSearch->three_opt_first(m_Tours[i] , m_nnList);
			double distance =calculatePathLength2(m_Tours[i]);
			if(distance < current_mindistance)
			{
				m_besttour = m_Tours[i];
				m_Restarttour =  m_Tours[i];
				current_distance =distance;
				current_mindistance = distance;
				m_randomPath= m_Tours[i];

				m_tourStats.bestTourToDateLenght = current_mindistance;
				m_tourStats.bestTourToDate = m_besttour;
				m_tourStats.bestTourFounditter = m_iterations;
			}

		}


	}
	virtual double getBestPathLengthSofar(){
		return calculatePathLength2(m_besttour);
	}
	~CRandomAnt(void){
		delete m_pLocalSearch;
	}
};





