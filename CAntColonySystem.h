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
#include "CAntSystem.h"
#include "CLocalSearch.h"
#include "Utils.h"

class CAntColonySystem :
	public CAntSystem
{
	std::mt19937 m_MExplore;
	double xi;
	double tau0;
	double q0;
	double m_r0;


private:
	void CalculateHeuristicForNode(size_t i, size_t j);
	virtual void decisionRule(size_t k, size_t step, uniform_real<double> &rndSelTrsh, uniform_real<double> &choiceE);
	virtual void evaporateAllPhero();
	double HEURISTIC(size_t m, size_t n) {
		return (1.0 / ((double) (*m_distanceMatrix)[m][n] + 0.000000001));
	}

	void globalPheromoneDeposit();
	void localPheromoneEndIndexUpdate(size_t idx1, size_t idx2);
	void localPheromoneUpdate(size_t ant, size_t step);
	void decisionRule2(size_t k, size_t step);
	
public:
	virtual void calculateHeuristicMatrix();
	virtual void constructSolutions();
	virtual void update();
	virtual void initSystem();
	CAntColonySystem(Parameters &Par, std::vector<std::vector<int> > *matrix);
	virtual ~CAntColonySystem(void);
};

