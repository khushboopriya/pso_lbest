+8#include <stdio.h>
#include <stdlib.h>

/* Random number generator in [0, 1] */
#define Rand() ((double)rand()/RAND_MAX)
// #define rand_01 ((float)rand() / (float)RAND_MAX)
/* Structure of a particle */
typedef struct {
double *x;
double *v;
double f;
double pbest;
double *x_star;
int lbestof_p_i;
} ParticleRec, *Particle;

/*
Parameters for PSO
*/

/* Number of particles.this is the swarm size. Good if this lies between 10 and 30 */
#define Nparticles 10
/* Maximum number of iterations */
#define T_MAX 100
/* The value of inertia weight at t=0 (W_0) and t=T_MAX (W_T) . THis is increasing inertia.*/
#define W_0 0.9
#define W_T 0.4
#define MAX_V 2.0
/* The cognitive parameter (c1) and the social parameter (c2) */
#define c1 2.0
#define c2 2.0

/*
Definitions for a problem
*/

/* Number of variables: problem dependent . */
#define Nvariables 3

/* Objective function for minimization: problem dependent */
#define better(y1, y2) (y1<y2)






/* The following is the function of (x_i-2)^2 */
void Evaluate(Particle P)
{
int i;

P->f=0.0;
for(i=0; i<Nvariables; i++)
P->f+=(P->x[i]-2)*(P->x[i]-2);
}

/* update pbest */
void UpdateBest(Particle P)
{
int j;

for(j=0; j<Nvariables; j++)
P->x_star[j]=P->x[j];
P->pbest=P->f;
}



/* Initialization of particles: problem dependent */
/* The function returns the index of the best particle */
void Initialize(Particle P, int n)
{
int i, j,bi,ai,ni;
//
for(i=0; i<n; i++) {
for(j=0; j<Nvariables; j++) {
P[i].x[j]=Rand(); /* problem dependent */
P[i].v[j]=0.0; /* problem dependent */
}
Evaluate(&P[i]);
UpdateBest(&P[i]);
//if(better(P[i].f, P[G].f)) G=i;//need to change the code to store lbest of each ith swarm
P[i].lbestof_p_i=i;//making everyone pbest of itself

//initializing lbest for each particle
/*
 bi=i-1;
 ni=i;
 ai=i+1;

if(i==0)
    bi=9;
if(i==9)
    ai=0;


if( better(P[bi].f,P[ni].f)&& better(P[bi].f,P[ai].f) )
    P[i].lbestof_p_i=bi;


if( better(P[ai].f,P[ni].f)&& better(P[ai].f,P[bi].f) )
    P[i].lbestof_p_i=ai;


if( better(P[ni].f,P[bi].f)&& better(P[ni].f,P[ai].f) )
    P[i].lbestof_p_i=ni;
    */

}
//return lb;
}

/*
Defination of pso
*/

/* allocate new data structures . purane velocities ko rakhna zaroori hai cauz if the next value is not good then it retains the previous value*/
#define New(type, n, msg) (type *)NewCell(sizeof(type), n, msg)

void *NewCell(int size, int n, char *msg)
{
void *new;

if((new=malloc(size*n))==NULL) {
fprintf(stderr, "Cannot allocate memory for %d %s\n", n, msg);
exit(1);
}
return new;
}





/* allocate "n" new particles */
Particle NewParticles(int n)
{
int i;
Particle P;

P=New(ParticleRec, n, "particles");
for(i=0; i<n; i++)
{
P[i].x=New(double, Nvariables, "x");
P[i].v=New(double, Nvariables, "v");
P[i].x_star=New(double, Nvariables, "x*");
}
return P;
}





/* Print a particle */
void Print(Particle P)
{
int j;

for(j=0; j<Nvariables; j++)
printf("%f ", P->x_star[j]);
printf(" = %g\t", P->pbest);
printf("\tlbestindex  %d \n  ",P->lbestof_p_i);
}






void main()
{
int t, i, j,a,b,n;
Particle P;
//int lb; index of lbest
double w;

P=NewParticles(Nparticles);
Initialize(P, Nparticles);
w=W_0;
for(t=1; t<=T_MAX; t++) {
for(i=0; i<Nparticles; i++) {
for(j=0; j<Nvariables; j++) {
P[i].v[j]=w*P[i].v[j]+c1*Rand()*(P[i].x_star[j]-P[i].x[j])+c2*Rand()*(P[P[i].lbestof_p_i].x_star[j]-P[i].x[j]);
if(P[i].v[j]<-MAX_V)
P[i].v[j]=-MAX_V;
else if(P[i].v[j]>MAX_V)
P[i].v[j]=MAX_V;
P[i].x[j]+=P[i].v[j];
}
Evaluate(&P[i]);
if(better(P[i].f, P[i].pbest))
UpdateBest(&P[i]);
//if(better(P[i].f, P[G].pbest)) G=i;

 b=i-1;
 n=i;
 a=i+1;

if(i==0)
    b=9;
if(i==9)
    a=0;
if( better(P[b].f,P[n].f)&& better(P[b].f,P[a].f) )
    P[i].lbestof_p_i=b;
if( better(P[a].f,P[n].f)&& better(P[a].f,P[b].f) )
    P[i].lbestof_p_i=a;
if( better(P[n].f,P[b].f)&& better(P[n].f,P[a].f) )
    P[i].lbestof_p_i=n;
printf(" iteration no is %4d:\n ", t);
printf("values for particleno ==%d is %d %d %d \t",i,b,a,n);
Print(&P[i]);
}

w-=(W_0-W_T)/T_MAX;
}
}

