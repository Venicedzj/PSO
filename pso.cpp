#include<iostream>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<iomanip>
using namespace std;

#define PARTICLE_NUM 20		//particle's number
#define DIMENSION 10		//spatial dimension
#define POS_MAX 100.0		//maximum position coordinate
#define POS_MIN -POS_MAX	//minimum position coordinate
#define V_MAX 0.2 * POS_MAX	//maximum velocity
#define V_MIN 0.2 * POS_MIN	//minimum velocity
#define c1 2.0				//individual cognitive coefficient
#define c2 2.0				//social learning coefficient
#define wMax 0.9			//maximum weights
#define wMin 0.4			//minimum weights
#define Tmax 2000			//maximum iterations number
int T = 1;					//current iterations number
#define PI acos(-1)
#define e exp(1)

/* particle structrue */
struct Particle
{
	vector<double> V;
	vector<double> POS;
	vector<double> pBest_pos;
	double fitness;
	double pBest_ftns;
};

/* gBest structrue */
struct GBest {
	vector<double> POS;
	double fitness;
};

/* function statement */
void initPtc();
double getFitnessVal(vector<double>);
void update_V_POS();
void print(GBest);
inline double random(double a, double b) { return ((double)rand() / RAND_MAX) * (b - a) + a; }

/* global particles and gBest */
vector<Particle> particles(PARTICLE_NUM);
GBest gBest;

int main() {
	srand((int)time(0));
	initPtc();
	while (T <= Tmax) {
		update_V_POS();
		print(gBest);
		T++;
	}
	return 0;
}

/*initialize each particle's velocity, position, pBest and global best position gBest*/
void initPtc() {
	gBest.fitness = DBL_MAX;
	for (auto &ptc : particles) {
		for (int i = 0; i < DIMENSION; i++) {
			ptc.V.push_back(random(V_MIN, V_MAX));
			ptc.POS.push_back(random(POS_MIN, POS_MAX));
		}
		ptc.fitness = getFitnessVal(ptc.POS);

		//initialize pBest
		ptc.pBest_pos = ptc.POS;
		ptc.pBest_ftns = ptc.fitness;

		//initialize gBest
		if (ptc.pBest_ftns < gBest.fitness) {
			gBest.POS = ptc.pBest_pos;
			gBest.fitness = ptc.pBest_ftns;
		}
	}
}

/*update particle's velocity, position and pBest*/
void update_V_POS() {
	double w = wMax - (wMax - wMin) * ((double)T / Tmax);
	for (auto &ptc : particles) {
		for (int i = 0; i < DIMENSION;i++) {
			double r1 = random(0, 1), r2 = random(0, 1);
			ptc.V[i] = w * ptc.V[i]
				+ c1 * r1 * (ptc.pBest_pos[i] - ptc.POS[i])
				+ c2 * r2 * (gBest.POS[i] - ptc.POS[i]);
			if (ptc.V[i] > V_MAX) ptc.V[i] = V_MAX;
			if (ptc.V[i] < V_MIN) ptc.V[i] = V_MIN;
			ptc.POS[i] = ptc.POS[i] + ptc.V[i];
			if (ptc.POS[i] > POS_MAX) ptc.POS[i] = POS_MAX;
			if (ptc.POS[i] < POS_MIN) ptc.POS[i] = POS_MIN;
		}
		ptc.fitness = getFitnessVal(ptc.POS);

		//keep the best
		if (ptc.fitness < ptc.pBest_ftns) {
			ptc.pBest_pos = ptc.POS;
			ptc.pBest_ftns = ptc.fitness;
			//keep gBest
			if (ptc.pBest_ftns < gBest.fitness) {
				gBest.POS = ptc.pBest_pos;
				gBest.fitness = ptc.pBest_ftns;
			}
		}
	}
}

/*using position coordinates as parameter calculate the fitness value by fitness function*/
double getFitnessVal(vector<double> POS) {
	double fitness = 0.0;

	///Sphere function
	for (auto i : POS) {
		fitness += i * i;
	}

	///Ackley function
	//double sum1 = 0.0, sum2 = 0.0;
	//for (auto i : POS) {
	//	sum1 += pow(i, 2);
	//	sum2 += cos(2 * PI * i);
	//}
	//fitness = 20 + e - 20 * exp(-0.2 * sqrt(sum1 / DIMENSION)) - exp(sum2 / DIMENSION);

	return fitness;
}

void print(GBest gBest) {
	cout << setiosflags(ios::left) << T << " gBest fitness value: " << gBest.fitness;
	//cout << " gBest value: ";
	//for (auto i : gBest.POS) cout << setiosflags(ios::left) << i << " ";
	cout << endl;
}
