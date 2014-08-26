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
#include "problemsolver.h"
#include <vector>

#include "Utils.h"


//not this alg is n! can only use on small tsps
class BruteForceSystem :
	public iProblemSolver
{
	MatrixArrayTypeInt  *m_distanceMatrix;
	std::vector<size_t> m_defaultTour;
	std::vector<size_t> m_bestTour;
	size_t numActive;

public:

	BruteForceSystem(MatrixArrayTypeInt  *distanceMatrix) :
	  m_defaultTour(distanceMatrix->size()){
		m_distanceMatrix = distanceMatrix;
		numActive = m_distanceMatrix->size();
		//CREATE A TOUR
		for(size_t i = 0; i < m_defaultTour.size(); i++){
			m_defaultTour[i] = i;
		}
	}
    //************************************
    // Method:    score
    // FullName:  BruteForceSystem::score
    // Access:    public 
    // Returns:   double
    // Qualifier: const
    //************************************
    double score() const {
		  double result = (*m_distanceMatrix)[m_defaultTour[0]][m_defaultTour[m_defaultTour.size() - 1]];
		  for(int i = 1; i < m_defaultTour.size(); ++i) { // i starts at 1, not 0!
			  result += (*m_distanceMatrix)[m_defaultTour[i - 1]][m_defaultTour[i]];
		  }
		  return result;
	  }
	//************************************
	// Method:    brute_force
	// FullName:  BruteForceSystem::brute_force
	// Access:    public 
	// Returns:   void
	// Qualifier:
	//************************************
	void brute_force(){
		double best_score = score();
		m_tourTime.startTimer();

		m_bestTour=m_defaultTour;
		while (next_permutation(m_defaultTour.begin() + 1, m_defaultTour.end())) 
		{
			double s = score();
			if (s < best_score)	{
				best_score = s;
				m_bestTour = m_defaultTour;
			}
		}
		m_tourStats.foundTime = m_tourTime.getElapsedTime();
		m_tourStats.tourAverageTime = m_tourTime.getElapsedTime();
		m_tourTime.stopTimer();
	}

	~BruteForceSystem(void){
	}
	virtual Tour getStats(){
		m_tourStats.foundTime = m_tourTime.getElapsedTime();
		m_tourStats.tourAverageTime = m_tourTime.getElapsedTime();
		this->m_tourStats.bestTourToDate = this->m_bestTour;
		m_tourStats.bestTourToDateLenght = getBestPathLengthSofar();
		return m_tourStats;
	}
	virtual void constructSolutions(){
		brute_force();
	}
	const virtual std::vector<size_t> &getBestSoFarPath(){
		return m_bestTour;
	}
	virtual double getBestPathLengthSofar()	{
		double result = (*m_distanceMatrix)[m_bestTour[0]][m_bestTour[m_bestTour.size() - 1]];
		for(int i = 1; i < m_bestTour.size(); ++i) {                // i starts at 1, not 0!
			result += (*m_distanceMatrix)[m_bestTour[i - 1]][m_bestTour[i]];
		}
		return result;
	}

	//************************************
	// Method:    not implemented
	//************************************
	virtual void localSearch(){}
	virtual void update(){}
	virtual void initSystem(){}
};

