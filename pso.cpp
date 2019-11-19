#include<iostream>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<iomanip>
using namespace std;

#define PARTICLE_NUM 40		//particle's number
#define DIMENSION 10		//spatial dimension
#define POS_MAX 100.0		//maximum position coordinate
#define POS_MIN -POS_MAX	//minimum position coordinate
#define V_MAX 0.2 * POS_MAX	//maximum velocity
#define V_MIN 0.2 * POS_MIN	//minimum velocity
#define c1 2.0				//individual cognitive coefficient
#define c2 2.0				//social learning coefficient
#define wMax 0.9			//maximum weights
#define wMin 0.4			//minimum weights
#define Tmax 100			//maximum iterations number
int T = 1;					//current iterations number

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
void get_gBest();
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
		get_gBest();
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
		ptc.pBest_pos = ptc.POS;
		ptc.pBest_ftns = ptc.fitness;
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
		if (ptc.fitness < ptc.pBest_ftns) {
			ptc.pBest_pos = ptc.POS;
			ptc.pBest_ftns = ptc.fitness;
		}
	}
}

/*calculate gBest after updating velocity and position*/
void get_gBest() {
	double minimum = particles[0].fitness;
	int flag = 0;
	for (int i = 0; i < PARTICLE_NUM; i++) {
		if (particles[i].fitness < minimum) {
			flag = i;
			minimum = particles[i].fitness;
		}
	}
	if (particles[flag].fitness < gBest.fitness) {
		gBest.POS = particles[flag].POS;
		gBest.fitness = particles[flag].fitness;
	}
}

/*using position coordinates as parameter calculate the fitness value by fitness function*/
double getFitnessVal(vector<double> POS) {
	double fitness = 0;
	for (auto pos : POS) {
		fitness += pos * pos;
	}
	return fitness;
}

void print(GBest gBest) {
	cout << setiosflags(ios::left) << setw(3) 
		<< T << " gBest fitness value: " << gBest.fitness;
	cout << " gBest value: ";
	for (auto i : gBest.POS) {
		cout << setiosflags(ios::fixed) << setprecision(5) << setiosflags(ios::left) << setw(8) 
			<< i << " ";
	}
	cout << endl;
}
