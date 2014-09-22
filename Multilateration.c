/* 
* 4 reference Multilateration for:
*
*		d
*	P1-----------P2
*		|
*		|i
*		|
*	 -------P3
*	    j
*
* Author: Sarogini Grace Pease May 2014
*
*/

#include "Multilateration.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	coord3D	p1, p2, p3, p4, soln1, soln2, soln3, soln4, soln5, soln6, soln7, soln8;
	double	r1, r2, r3, r4, d1, d2, h;
	int	reasonA, reasonB, reasonC, reasonD, maxsolution, reasoncount;
	while (scanf("%lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg %lg",
	                     &p1.x, &p1.y,
	                     &p2.x, &p2.y,
	                     &p3.x, &p3.y,
	                     &p4.x, &p4.y,
	                     &h, &r1, &r2,
	                     &r3, &r4 ) == 13) {

    		FILE * Output = fopen("coordinates.txt", "a");
    		if (!Output)
    		{
        		perror( "Unable to open file" );
        		exit( EXIT_FAILURE );
    		} else {
 			printf("--------------------------------------------\n");
 			printf("Print output to file\n");
		}

	/* Create cartesian coordinates for 4 sensor nodes*/
		p1.z = h;
		p2.z = h;
		p3.z = h;
		p4.z = h;

		reasoncount=0;
		reasonA = trilateration(&soln1, &soln2, p1, r1, p2, r2, p3, r3, MAXZERO);
		reasonB = trilateration(&soln3, &soln4, p1, r1, p2, r2, p4, r4, MAXZERO);
		reasonC = trilateration(&soln5, &soln6, p1, r1, p3, r3, p4, r4, MAXZERO);
		reasonD = trilateration(&soln7, &soln8, p2, r2, p3, r3, p4, r4, MAXZERO);
		maxsolution=max(max(max(reasonA,reasonB),reasonC), reasonD);

		if (reasonA == maxsolution){
			/* SOLVE FOR SPHERES 1 2 3 */
			reasonA = trilateration(&soln1, &soln2, p1, r1, p2, r2, p3, r3, MAXZERO);
			if (reasonA<0) {
				PrintNoSolution(reasonA, 1, 2, 3, Output);
				reasoncount++;
		  	} else {
				fprintf(Output, "1\t2\t3\tA\t%.1f\t%.1f\t%.1f\t%d\tB\t%.1f\t%.1f\t%.1f\t%d\n",soln1.x, soln1.y, soln1.z,reasonA,soln2.x, soln2.y, soln2.z,reasonA);
			}
		} else if (reasonB == maxsolution){ 
			/* SOLVE FOR SPHERES 1 2 4 */
			reasonB = trilateration(&soln3, &soln4, p1, r1, p2, r2, p4, r4, MAXZERO);
			if (reasonB<0) {
				PrintNoSolution(reasonB, 1, 2, 4, Output);
				reasoncount++;
			} else {
				fprintf(Output, "1\t2\t4\tA\t%.1f\t%.1f\t%.1f\t%d\tB\t%.1f\t%.1f\t%.1f\t%d\n", soln1.x, soln1.y, soln1.z,reasonB, soln2.x, soln2.y, soln2.z,reasonB);
			}

		} else if (reasonC == maxsolution){ 
			/* SOLVE FOR SPHERES 1 3 4 */
			reasonC = trilateration(&soln5, &soln6, p1, r1, p3, r3, p4, r4, MAXZERO);
			if (reasonC<0) {
				PrintNoSolution(reasonC, 1, 3, 4, Output);
				reasoncount++;
			} else {
				fprintf(Output, "1\t3\t4\tA\t%.1f\t%.1f\t%.1f\t%d\tB\t%.1f\t%.1f\t%.1f\t%d\n", soln1.x, soln1.y, soln1.z,reasonC, soln2.x, soln2.y, soln2.z,reasonC);
			}

		} else if (reasonD == maxsolution){ 
			/* SOLVE FOR SPHERES 2 3 4 */
			reasonD = trilateration(&soln7, &soln8, p2, r2, p3, r3, p4, r4, MAXZERO);
			if (reasonD<0) {
				PrintNoSolution(reasonD, 2, 3, 4, Output);
				reasoncount++;
			} else {
				fprintf(Output, "2\t3\t4\tA\t%.1f\t%.1f\t%.1f\t%d\tB\t%.1f\t%.1f\t%.1f\t%d\n", soln1.x, soln1.y, soln1.z,reasonD, soln2.x, soln2.y, soln2.z,reasonD);
			}
		}
		fclose(Output);
		printf("\n%d were reason\nTry again or \nQ=quit\n--------------------------------------------\n",reasoncount);
	}
	return 0;
}


/* Return zero if successful, negative error otherwise.
 * The last parameter is the largest nonnegative number considered zero;
*/
int trilateration(coord3D *const result1, coord3D *const result2,
                  const coord3D p1, const double r1,
                  const coord3D p2, const double r2,
                  const coord3D p3, const double r3,
                  const double maxzero)
{
	coord3D	ex, ey, ez, t1, t2;
	double	d, i, j, x, y, z, t;

 	/* Treat P1 P2 P3 as vectors from the origin */
	ex = VectorDiff(p2, p1);			/* ex = (p2 - p1) */
	d = VectorNorm(ex);				/* d  = |p2 - p1| */
	if (d <= maxzero) {
		/* PROBLEM: p1 and p2 are concentric. */
		return -1;
	}
	ex = VectorDiv(ex, d);				/* ex = (p2 - p1) / |p2 - p1| */
 
	t1 = VectorDiff(p3, p1);			/* t1 = p3 - p1 */
	i = DotProduct(ex, t1);
	t2 = VectorMult(ex, i);				/* t2 = ex (ex . (p3 - p1)) */
 
	ey = VectorDiff(t1, t2);			/* ey = (t1 - t2) */
	t = VectorNorm(ey);				/* t  = |t1 - t2| */
	if (t > maxzero) {
		ey = VectorDiv(ey, t);			/* ey = (t1 - t2) / |t1 - t2| */
 
		j = DotProduct(ey, t1);			/* j  = ey . (p3 - p1) */
	} else
		j = 0.0;				/* t <= maxzero implies j = 0.0. */
	

	if (fabs(j) <= maxzero) {
		/* PROBLEM: p1, p2 and p3 are colinear. */
		/* Is point p1 + (r1 along the axis) the intersection? */
		t2 = VectorSum(p1, VectorMult(ex, r1));
		if (fabs(VectorNorm(VectorDiff(p2, t2)) - r2) <= maxzero && fabs(VectorNorm(VectorDiff(p3, t2)) - r3) <= maxzero) {
			/* Yes, t2 is the only intersection point. */
			if (result1)
				*result1 = t2;
			if (result2)
				*result2 = t2;
			return min(d,fabs(j));
		}
 
		/* Is point p1 - (r1 along the axis) the intersection? */
		t2 = VectorSum(p1, VectorMult(ex, -r1));
		if (fabs(VectorNorm(VectorDiff(p2, t2)) - r2) <= maxzero && fabs(VectorNorm(VectorDiff(p3, t2)) - r3) <= maxzero) {
			/* Yes, t2 is the only intersection point. */
			if (result1)
				*result1 = t2;
			if (result2)
				*result2 = t2;
			return min(d,fabs(j));
		}
 
		return -2;
	}
	ez = CrossProduct(ex, ey);			/* ez = ex x ey */

 	/* Solve for x, y, z. */
	x = (r1*r1 - r2*r2) / (2*d) + d / 2;
	y = (r1*r1 - r3*r3 + i*i) / (2*j) + j / 2 - x * i / j;
	z = r1*r1 - x*x - y*y;
	if (z < -maxzero) {
		/* The solution is invalid. */
		return -3;
	} else
	if (z > 0.0)
		z = sqrt(z);
	else
		z = 0.0;
 
	t2 = VectorSum(p1, VectorMult(ex, x));		/* t2 = p1 + x ex + y ey */
	t2 = VectorSum(t2, VectorMult(ey, y));
 
	if (result1)
		*result1 = VectorSum(t2, VectorMult(ez, z));/* result1 = p1 + x ex + y ey + z ez */
 
	if (result2)
		*result2 = VectorSum(t2, VectorMult(ez, -z));/* result1 = p1 + x ex + y ey - z ez */
 
	return min(d,fabs(j));
}

/* Subtraction: vector1 - vector2 */
coord3D VectorDiff(const coord3D vector1, const coord3D vector2)
{
	coord3D v;
	v.x = vector1.x - vector2.x;
	v.y = vector1.y - vector2.y;
	v.z = vector1.z - vector2.z;
	return v;
}
 
/* Addition: vector1 + vector2 */
coord3D VectorSum(const coord3D vector1, const coord3D vector2)
{
	coord3D v;
	v.x = vector1.x + vector2.x;
	v.y = vector1.y + vector2.y;
	v.z = vector1.z + vector2.z;
	return v;
}

/* Division: vector1 / n */
coord3D VectorDiv(const coord3D vector, const double n)
{
	coord3D v;
	v.x = vector.x / n;
	v.y = vector.y / n;
	v.z = vector.z / n;
	return v;
}
 
/* Multiplication: vector1 * n */
coord3D VectorMult(const coord3D vector, const double n)
{
	coord3D v;
	v.x = vector.x * n;
	v.y = vector.y * n;
	v.z = vector.z * n;
	return v;
}
 
/* Euclidean Norm of Single Vector */
double VectorNorm(const coord3D vector)
{
	return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
}
 
/* DotProduct product of vector1 and vector2 */
double DotProduct(const coord3D vector1, const coord3D vector2)
{
	return vector1.x * vector2.x + vector1.y * vector2.y + vector1.z * vector2.z;
}
 
/* Cross product of vector1 and vector2 */
coord3D CrossProduct(const coord3D vector1, const coord3D vector2)
{
	coord3D v;
	v.x = vector1.y * vector2.z - vector1.z * vector2.y;
	v.y = vector1.z * vector2.x - vector1.x * vector2.z;
	v.z = vector1.x * vector2.y - vector1.y * vector2.x;
	return v;
}

/* Print Insolvability Reason */
void PrintNoSolution(const int reason, const int sph1, const int sph2, const int sph3, FILE *OutputFile)
{
	switch (reason)
	{
	case -1:
		fprintf(OutputFile, "p%d-p%d_Concentric\n",sph1, sph2);
	break;
	case -2:
		fprintf(OutputFile, "p%d-p%d-p%d_Colinear\n", sph1, sph2, sph3);
	break;
	case -3:
		fprintf(OutputFile, "Insolvable_%d_%d_%d_Z_coordinate\n", sph1, sph2, sph3);
	break;
	default:
		fprintf(OutputFile, "Insolvable_%d_%d_%d_No_solution_(%d)\n", reason, sph1, sph2, sph3);
	}
	return;
}

 
