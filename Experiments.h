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
#include "tspalgorithm.h"
#include "utils.h"

#include <string>
#include <vector>
#include <map>


class Experiments :
	public TSPalgorithm
{
	
	std::map<std::string,int> m_problemFiles;
	//std::vector<std::string> m_problemFiles;
	std::vector<Parameters> m_parameters;

public:
	virtual void Run(Parameters &par ,  std::vector<std::vector<int> > &matrix, int experiment);	
	Experiments(void);
	~Experiments(void);

	void PopulateInstances();
	void constructParameterExp1();

	std::map<std::string,int> GetInstancesArray(){
		return m_problemFiles;
	}
	const std::vector<Parameters> &GetParametersArray(){
		return m_parameters;
	}


};

