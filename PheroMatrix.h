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
#include <float.h>
#include "Utils.h"




class PheroMatrix
{
	double evaporation_rate_ , initial_pheromone_;
	MatrixArrayTypeDouble *matrix_;

public:


	~PheroMatrix(void){
		delete matrix_;
	}
	MatrixArrayTypeDouble *getMatrix(){
			return matrix_;
	}
	PheroMatrix(size_t size, double evaporation_rate, double initial_pheromone){
		matrix_ = new MatrixArrayTypeDouble(size , size);
		evaporation_rate_ = evaporation_rate;
		initial_pheromone_ = initial_pheromone;
	}
	PheroMatrix(size_t size, double evaporation_rate){
		matrix_ = new MatrixArrayTypeDouble(size , size);
		evaporation_rate_ = evaporation_rate;
		initial_pheromone_ = (double) 1/size;
		for (size_t i=0;i<matrix_->size();i++)	{
			for (size_t j=0;j<matrix_->size();j++)
				(*matrix_)[i][j] = initial_pheromone_;// 0.1 * rand() / (double)RAND_MAX;
		}
	}
	double get(size_t  v, size_t w) {
		return (*matrix_)[v][w];
	}
	void add(size_t v, size_t w, double amount) {
		(*matrix_)[v][w] += amount;
		(*matrix_)[w][v] += amount;
	}
	void multipy(size_t  v, size_t  w, double amount) {
		(*matrix_)[v][w] *= amount;
		(*matrix_)[w][v] = (*matrix_)[v][w];
	}
	void subtract(size_t v, size_t  w, double amount){
		if((*matrix_)[v][w] - amount <= 0.0)
		{
			(*matrix_)[v][w]=0.00000000000000000000000000000001;
			(*matrix_)[w][v]=0.00000000000000000000000000000001;
		}
		else
		{
			(*matrix_)[v][w] -= amount;
			(*matrix_)[w][v] = (*matrix_)[v][w];
		}
	
	}
	void evaporate(size_t v, size_t w) {
		(*matrix_)[v][w] *= 1 - evaporation_rate_;
		(*matrix_)[w][v] = (*matrix_)[v][w];
	}
	
	//************************************
	// Method:    evaporate1
	// FullName:  PheroMatrix::evaporate1
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: size_t v
	// Parameter: size_t w
	// Definition: for use with asemertical matrix
	//************************************
	void evaporate1(size_t v, size_t w) {
		(*matrix_)[v][w] *= 1 - evaporation_rate_;

	}
	void set1(size_t v, size_t w , double value) {
		(*matrix_)[v][w] = value;
	}
	void set(size_t v,  size_t w , double value) {
		(*matrix_)[v][w] = value;
		(*matrix_)[w][v] = value;
	}
	void evaporate_all() {
		for(size_t i=0;i<matrix_->size();i++) {
			for(size_t j=0;j<matrix_->size();j++) {
				evaporate(i,j);
			}
		}
	}
	double get_evaporation_rate() {
		return evaporation_rate_;
	}
	size_t size() {
		return (matrix_->size());
	}
	double lambda_branching_factor(unsigned int v, double lambda) {
		double min_pheromone = (std::numeric_limits<double>::max)();
		double max_pheromone = 0.0;
		for(unsigned int i=0;i<this->size();i++) {
			double pheromone = (*matrix_)[v][i];
			if(min_pheromone > pheromone) {
				min_pheromone = pheromone;
			}
			if(max_pheromone < pheromone) {
				max_pheromone = pheromone;
			}
		}
		double limit = min_pheromone + lambda * (max_pheromone - min_pheromone);
		unsigned int branching_factor = 0;
		for(unsigned int j=0;j<this->size();j++) {
			if((*matrix_)[v][j] >= limit) {
				branching_factor++;
			}
		}
		return branching_factor;
	}

	double averageLambdaBranchingFactor(double lambda) {
		double sum = 0.0;
		for(unsigned int i=0;i<this->size();i++){
			sum += lambda_branching_factor(i, lambda);
		}
		return sum / (this->size() *2);
	}
	/* compute average pheromone trail on edges of an ants path */
	double averagePheromoneTrail(std::vector<size_t> &AntPath){
		double avg_trail=0.0;
		size_t noNodes =  AntPath.size();
		for ( size_t i = 0 ; i < noNodes-1 ; i++ ){
			avg_trail +=  (*matrix_)[AntPath[i]][AntPath[i+1]];
		}
		avg_trail /= (double) noNodes;
		return avg_trail;
	}
	double evaporateAntPath(std::vector<size_t> &AntPath){
		double avg_trail=0.0;
		size_t noNodes =  AntPath.size();
		for ( size_t i = 1 ; i < noNodes ; i++ ){
			size_t to = AntPath[i]; 
			size_t from = AntPath[i-1];
			evaporate(to,from);
		}

	}
	void local_pheromone_update(size_t v, size_t  w , double xi_ , double initial_pheromone_ ) {
		(*matrix_)[v][w] = (1 - xi_) * (*matrix_)[v][w] + xi_ * initial_pheromone_;
	}
	void evaporateACS(unsigned int v, unsigned int w , double evaporation_rate_) {
		(*matrix_)[v][w] *= 1 - evaporation_rate_;
	}



};

