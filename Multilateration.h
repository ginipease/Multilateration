#include <stdio.h>
#include <math.h>
 
#define   MAXZERO  0.0
#define min(a,b) (a<b?a:b)
#define max(a,b) (a>b?a:b)

typedef struct coord3D	coord3D;
struct coord3D {
	int	d;
	double	x;
	double	y;
	double	z;
};

/* Initialise x,y,z coordinates of sensors */
coord3D SensorInit(const coord3D, const int);

/* Subtraction: vector1 - vector2 */
coord3D VectorDiff(const coord3D, const coord3D);
 
/* Addition: vector1 + vector2 */
coord3D VectorSum(const coord3D, const coord3D);

/* Division: vector1 / n */
coord3D VectorDiv(const coord3D, const double);
 
/* Multiplication: vector1 * n */
coord3D VectorMult(const coord3D, const double);
 
/* Euclidean Norm of Single Vector */
double VectorNorm(const coord3D vector);
 
/* DotProduct product of vector1 and vector2 */
double DotProduct(const coord3D, const coord3D);
 
/* Cross product of vector1 and vector2 */
coord3D CrossProduct(const coord3D, const coord3D);

/* Return zero if successful, negative error otherwise.
 * The last parameter is the largest nonnegative number considered zero;
 * it is somewhat analoguous to machine epsilon (but inclusive).
*/
int trilateration(coord3D *const, coord3D *const,
                  const coord3D, const double,
                  const coord3D, const double,
                  const coord3D, const double,
                  const double);

/* Print Insolvability Reason */
void PrintNoSolution(const int, const int, const int, const int, FILE *OutputFile);

 
