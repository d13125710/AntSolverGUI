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
#include "CElitistAntSystem.h"


//************************************
// Method:    CElitistAntSystem
// FullName:  CElitistAntSystem::CElitistAntSystem
// Access:    public 
// Returns:   
// Qualifier: : CAntSystem(Par , distanceMatrix)
// Parameter: Parameters & Par
// Parameter: MatrixArrayTypeInt * distanceMatrix
//get the best distance and adjust phero with that distance
//reported, that the best value for mWeight is between four and six.
//************************************
CElitistAntSystem::CElitistAntSystem(Parameters& Par, MatrixArrayTypeInt *distanceMatrix) 
	: CAntSystem(Par , distanceMatrix) , mWeight( Par.EliteWeight)
{
	if(mWeight ==0)
		mWeight = m_noNodes;
}
//
//************************************
// Method:    updatePheromones
// FullName:  CElitistAntSystem::updatePheromones
// Access:    virtual public 
// Returns:   void
// Qualifier:
// The pheromone update is a little bit different: on each iteration,
//the best to date ant deposits an additional quantity of pheromone on paths it traveled:
//************************************
void CElitistAntSystem::update()
{
	evaporateAllPhero();
	double d_tau = (double)mWeight/ m_BestAntToDate.getAntTourLength(); 
	for(size_t city = 1; city <m_BestAntToDate.getNoNodes(); city++)
	{
		size_t from = m_BestAntToDate.getCity(city-1);
		size_t to = m_BestAntToDate.getCity(city);
 		// eq 14.2 / 14.3
		this->m_newPheromoneMatrix->add(from , to , d_tau);	
	}
	for(size_t  k = 0; k <m_Ants.size(); k++)
		adjustPheremone(k);
	calculateHeuristicMatrix();
}

//************************************
// Method:    ~CElitistAntSystem
// FullName:  CElitistAntSystem::~CElitistAntSystem
// Access:    virtual public 
// Returns:   
// Qualifier:
// Parameter: void
//************************************
CElitistAntSystem::~CElitistAntSystem(void)
{
}
