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
#include "cantsystem.h"
class CBestWorstAntSystem :
	public CAntSystem
{

	double  trail_0;
	int restart_iteration;
	DWORD restart_time;
	DWORD starttime;

	void BWUpdate(const CAnt &bestAnt, const CAnt &worstAnt);
	void Mutation();

public:

	CBestWorstAntSystem(Parameters &Par, MatrixArrayTypeInt *distanceMatrix);
	virtual ~CBestWorstAntSystem(void);
	virtual void updatePheromes();
	void initPheromoneTrails(double initialValue) const;
	virtual void initPheromones();

	virtual void initSystem(){
		initPheromones();
		calculateHeuristicMatrix();
	}
	virtual void update(){
		updatePheromes();
	}

};

