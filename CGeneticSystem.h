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

#include "CChromo.h"
#include "CLocalSearch.h"
#include "ProblemSolver.h"

#include <numeric>


typedef std::vector<std::vector<int> > MatrixArrayTypeInt;

class CGeneticSystem : public iProblemSolver
{
	bool m_bLocalSearch , m_LocalSearchOpt2 , m_LocalSearchOpt3 , m_LocalSearchGreed;
	int m_populationSize , m_noCitys;
    double m_mutationRatio;
    bool m_bOpt2Search;;
    bool m_bOpt3Search;
	std::vector<CChromo> m_ChromoPopulation;
    MatrixArrayTypeInt *m_distMatrix;
    std::vector<double>  m_fitness;
    std::vector<size_t>  m_bestSoFarPath;
    CLocalSearch *m_pLocalSearch;
	std::vector<std::vector<size_t> > m_nnList;
	int firsttime;
	int m_worstdistance;

	
	void updateBestSoFarPath();
	void computeFitness();
	void mutatePopulation(std::vector<CChromo> &pop);
	void SortPopulation(std::vector<CChromo> &pop , bool Direction=true);
	int tournamentSelection();
	void stepGeneration2();
	std::vector<size_t> &getBestPath();
	std::vector<size_t> & getWorstPath();
	
	double getBestSoFarPathLength();
	

	void calculateNearestNeigbhor(unsigned int NUMOFANTS=20)
	{
		if(NUMOFANTS >= m_noCitys)
			NUMOFANTS= m_noCitys -1;

		size_t *pHelpArray = new size_t[m_noCitys];
		double *pdistanceArray = new double[m_noCitys ];
		*(pdistanceArray) =(std::numeric_limits<double>::max)();

		m_nnList.resize(m_noCitys);
		for (size_t  i = 0; i < m_noCitys; ++i)
			m_nnList[i].resize(NUMOFANTS);


		for (size_t node = 0; node < m_noCitys; node++) 
		{ 
			for (size_t i = 0; i < m_noCitys; i++) 
			{
				*(pdistanceArray+i)=(*m_distMatrix)[node][i];
				*(pHelpArray+i)=  i;
			}
			double max = (std::numeric_limits<double>::max)() - 1; 
			*(pdistanceArray+node) =  (std::numeric_limits<double>::max)();  // set to a large value .. 
			this->m_pLocalSearch->sort2(pdistanceArray, pHelpArray, 0, static_cast<unsigned int>(m_noCitys - 1));
			for (size_t i = 0; i < NUMOFANTS; i++) 
			{
				m_nnList[node][i] = *(pHelpArray+i);
			}
		}
		delete [] pHelpArray;
		delete [] pdistanceArray;
	}

	void PopulateStats();


public:

	CGeneticSystem(int populationSize, int noNodes, double mutationRatio, bool doOpt2, bool doOpt3 , MatrixArrayTypeInt *d);
	CGeneticSystem(Parameters &Par , MatrixArrayTypeInt *d); 	

	virtual Tour getStats(){
		PopulateStats();
		return m_tourStats;
	}
	virtual void initSystem();
	virtual void constructSolutions();
	//	const virtual std::vector<size_t> &getBestSoFarPath()=0;
	const virtual std::vector<size_t> &getBestSoFarPath();
	virtual void localSearch();
	virtual double getBestPathLength();
	virtual double getBestPathLengthSofar(){
		return computePathLength(m_bestSoFarPath);
	}

	virtual void update(){};
	int computePathLength(const std::vector<size_t> &tour);
	CGeneticSystem(void);
	~CGeneticSystem(void);

	virtual void incrementItteration() {
		m_iterations++;
		m_noOfToursContructed = m_noOfToursContructed + 5;
		m_tourStats.currentIrreation = m_iterations;
		m_tourStats.noOfToursContructed =  m_noOfToursContructed;
		m_tourStats.timeRunning = m_startTime.getRunningTime();
	}




private:
	


};

