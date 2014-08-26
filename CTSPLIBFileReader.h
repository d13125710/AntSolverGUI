
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
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <random>
#include <array> //for std::array


#include "Tokenizer.h"
#include "TokenFinder.h"
#include "DistanceHelper.h"

using namespace std;
typedef Tokenizer<TokenFinder> Tk;
typedef std::vector<std::vector<int> > MatrixArrayType;

class Coords{
public:
	double x;
	double y;

	double GetX(){
		return x;
	}
	double GetY()
	{
		return y;
	}
};

class CTSPLIBFileReader
{
	
private:
	//open
	//bool openFileStream(const string &str);
	void calculateDistanceMatrix();

private: 
	std::ifstream m_inFileStream; 
	std::string m_fileName;
	string m_sInput;
	vector<Coords> m_coordsVector;
 	MatrixArrayType m_theMatrix;
	IDistance *p_mDistanceCalculator;

public:
	bool Read();
	double ShowSolution();
	double ReadMatrix(int x, int y) const{
		return  m_theMatrix[x][y];
	}
	MatrixArrayType CopyMatrix(){
		return m_theMatrix; 
	}
	vector<Coords> CopyCoordsMatrix(){
		return m_coordsVector;
	}

	int getMatrixSize() const {
	     return  (int)m_theMatrix.size();
	}
	MatrixArrayType & getMatrixbyref() {
		return  m_theMatrix;
	}

	CTSPLIBFileReader(const string &str);
	CTSPLIBFileReader(){
		p_mDistanceCalculator=0;
	}
	void Init(const std::string &fileName)
	{
		m_fileName = fileName;
		m_inFileStream.open(fileName);
		p_mDistanceCalculator =0;
		m_theMatrix.empty();
		m_coordsVector.empty();
		m_theMatrix.resize(0);
		m_coordsVector.resize(0);
		
	}
	~CTSPLIBFileReader(void); 
	bool ReadPoint();

	int minx;
	int miny;
	int maxx;
	int maxy;


};

