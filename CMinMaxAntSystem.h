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

#include <vector>
#include <random>
#include <time.h>
#include <math.h>
#include <iostream>
#include <limits>       


#include "CAnt.h"
#include "CAntSystem.h"
#include "CLocalSearch.h"


class CMinMaxAntSystem: public CAntSystem
{
private:

	double m_tau0;
	double m_lambda;
	double  m_trail_0;
	double  m_trail_max;
	double	m_trail_min;
	bool m_resetAnt;
	double m_restartBestAntTourLength;
	std::vector<size_t> m_restartAntBestPath;

private:

	void initPheromoneTrails(double initialValue) const;
	void globalUpdatePheromone(const std::vector<size_t> &AntTour);
 	double nodeBranching(double l) const;
	virtual bool updateBestSoFarPath();


public:
	
	virtual void initSystem();
	void checkPheromoneLimits();
	virtual void update();
	CMinMaxAntSystem(Parameters& Par , std::vector<std::vector<int> >  *Vdistance);
	virtual ~CMinMaxAntSystem(void);
};

