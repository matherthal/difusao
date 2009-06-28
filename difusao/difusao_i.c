/*
 * difusao_i.c
 *
 *  Created on: Jun 28, 2009
 *      Author: matheus
 */

/*
 * parabolicai.c
 *
 *  Created on: Jun 14, 2009
 *      Author: matheus
 */

#include <stdio.h>
#include <math.h>

/* Programa de demonstracao que implementa o metodo de resolucao de equacao */
/* parabolica  por um metodo explicito */

float f(float x)
/* Funcao que da' a condicao inicial */
{
	if ((x > 0.0) && (x <= 0.3))
		return(100.0 * x);
	else
		return(0);
}

float gx0(float t)
{
	return(10.0);
}

float gxfim(float t)
{
	return(20.0);
}

float f(float x);
float gx0(float t);
float gxfim(float t);

int main(void)
{
	FILE *outf;

	float solucao[600][50],
		t, t0, tfim, x, x0, xfim, h, k, alfa, lambda;

	int i, j, nx, nt;

	if ((outf = fopen("difusao.dat", "w")) == NULL)
	{
		printf("\nProblemas na abertura do arquivo\n");
	}

	h = 0.001; 	/* Discretizacao do tempo */

	k = 1.0/6.0;  	/* Discretizacao do espaco */
	fprintf(stdout, " k = %f", k);

	t0   = 0.0;
	tfim = 0.004;

	x0   = 0.0;
	xfim = 1.0;

	nt = (int) ((tfim - t0)/h) + 2;
	nx = (int) ((xfim - x0)/k) + 2;

	fprintf(stdout, "\n Numero de intervalos temporais %d e espaciais %d", nt, nx);

	/* Parametros fisicos e variavel auxiliar */

	alfa = 1.0;

	lambda = alfa * h/(k * k);
	fprintf(stdout, "\n lambda(k*k) = %f",lambda);

	/* Condicao inicial */

	for (j = 0; j < nx; j++)
	{
		solucao[0][j] = f(x);
		/*fprintf(stdout, "\n solucao[0][%d] = %f", j, solucao[0][j]);*/

		x += k;
	}

	/* Condicao de contorno */

	for (i = 0; i < nt; i++)
	{
		fprintf(stdout, "\n i = %d",i);
		t += h;

		for (j = 1; j < nx - 1; j++)
		{
			if (j==i)
				solucao[i][j] = (1.0 + 2.0 * lambda);
			else if ((j==(i+1)) || (j==(i-1)))
				solucao[i][j] = lambda;
			else
				solucao[i][j] = 0.0;
		}
	}

	/*for (i = 0; i < nt; i++)
	{
		fprintf(stdout, "\n i = %d",i);
		t += h;

		if((i==0)&&(j==1))		/*Primeiro valo da matriz*/
		/*	solucao[0][1] = (1.0 + 2.0 * lambda) * solucao[i+1][j] -
				lambda * (solucao[i+1][j+1] + gx0(t));
		else if((i==nt-1)&&(j==nx-1))		/*Último valo da matriz*/
		/*	solucao[i][nx - 1] = (1.0 + 2.0 * lambda) * solucao[i+1][j] -
				lambda * (gxfim(t) + solucao[i+1][j-1]);
		else
		{
			fprintf(stdout, "\n phi(%d,%d) = %f",i+1,0,solucao[i+1][0]);

			for (j = 1; j < nx - 1; j++)
			{
				if (j==i)
					solucao[i][j] = (1.0 + 2.0 * lambda);
				else if ((j==(i+1)) or (j==(i-1)))
					solucao[i][j] = lambda;
			}
		}*/

		fprintf(stdout, "\n !!",i+1,0,solucao[i+1][0]);
	}

	for (i = 0; i < nt; i++)
	{
		fprintf(stdout, "\n i = %d",i);
		t += h;
		solucao[i+1][0]      = gx0(t);
		solucao[i+1][nx - 1] = gxfim(t);
		fprintf(stdout, "\n phi(%d,%d) = %f",i+1,0,solucao[i+1][0]);

		for (j = 1; j < nx - 1; j++)
		{
			solucao[i + 1][j] = lambda * (solucao[i][j + 1] + solucao[i][j - 1]) +
				(1.0 - 2.0 * lambda) * solucao[i][j];
			/*fprintf(stdout, "\n phi(%d,%d) = %f*(phi(%d,%d) + phi(%d,%d)) + %f*phi(%d,%d) = %f",
					(i+1),j,lambda,i,(j+1),i,(j-1),(1.0 - 2.0 * lambda),i,j,solucao[i + 1][j]);*/
			fprintf(stdout, "\n phi(%d,%d) = %f*%f + %f*(%f + %f) = %f",(i+1),j,(1.0 - 2.0 * lambda),solucao[i][j],
					lambda,solucao[i][j + 1],solucao[i][j - 1],solucao[i + 1][j]);
		}

		fprintf(stdout, "\n phi(%d,%d) = %f",i+1,(nx-1),solucao[i+1][nx-1]);
	}

	x = x0;

	for (j = 0; j < nx; j++)
	{
		fprintf(outf, "%f ", x);
		for (i = 0; i < nt; i++)
		{
			fprintf(outf, "%f ", solucao[i][j]);
		}
		fprintf(outf, "\n");
		x += k;
	}

}
