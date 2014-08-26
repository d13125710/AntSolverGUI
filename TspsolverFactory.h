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

#include "BestWorstAntSystem.h"
#include "CAntColonySystem.h"
#include "CAntSystem.h"
#include "CElitistAntSystem.h"
#include "RandomTest.h"
#include "CMinMaxAntSystem.h"
#include "CGeneticSystem.h"

#include "BruteForceSystem.h"
#include "MutatedAnt1.h"

class TSPSolverFactory
{
public:
	static iProblemSolver *create(Parameters &par , MatrixArrayTypeInt *matrix_)
	{
		iProblemSolver *pSolver =0;
		switch (par.ALG)
		{
				case (MMAS) :
					pSolver = new CMinMaxAntSystem(par , matrix_);
					break;
				case (ACS) :
					pSolver = new CAntColonySystem(par , matrix_);
					break;
				case (AS) :
					pSolver= new CAntSystem(par , matrix_);
					break;
				case (GS) :
					pSolver = new CGeneticSystem(par , matrix_);
					break;
				case (BWAS) :
					pSolver= new CBestWorstAntSystem(par , matrix_);
					break;
				case (MA) :
					pSolver = new CMutatedAnt(par , matrix_);
					break;
				case (EAS) :
					pSolver = new CElitistAntSystem(par , matrix_);
					break;
				case (BF) :
					pSolver = new BruteForceSystem(matrix_);
					break;
				case (RA) :
					pSolver = new CRandomAnt(par , matrix_);
					break;
				default:
					break;
		}
		return pSolver;
	}

};


