/*
 * difusao_e.c
 *
 *  Created on: Jun 23, 2009
 *      Author: matheus
 *
 *  Resolu��o do segundo desafio da disciplina de M�todos Num�ricos 2:
 *  	Resolver a Equa��o de Difus�o por m�todos impl�cito e expl�cito
 *  	com condi��o de contorno de Neumann
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
		return(0.0);
}

void difusao_explicito()
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
	fprintf(stdout, "\n Solu��o inicial:");
	for (j = 0; j < nx; j++)
	{
		solucao[0][j] = f(x);
		fprintf(stdout, "\n phi[0][%d] = %f", j, solucao[0][j]);

		x += k;
	}

	/* Condicao de contorno */

	for (i = 0; i < nt; i++)
	{
		/*Eh usado: fprintf(stdout, "\n i = %d",i);*/
		t += h;
		/*Eh usado: fprintf(stdout, "\n phi(%d,%d) = %f",i+1,0,solucao[i+1][0]);*/

		//for (j = 1; j < nx - 1; j++)
		for (j = 2; j < nx - 2; j++)
		{
			solucao[i + 1][j] = (1.0 - 2.0 * lambda) * solucao[i][j] +
				lambda * (solucao[i][j + 1] + solucao[i][j - 1]);
			/*fprintf(stdout, "\n phi(%d,%d) = %f*(phi(%d,%d) + phi(%d,%d)) + %f*phi(%d,%d) = %f",
					(i+1),j,lambda,i,(j+1),i,(j-1),(1.0 - 2.0 * lambda),i,j,solucao[i + 1][j]);*/
			/*Esse que � usado na impress�o:*/
			/*fprintf(stdout, "\n phi(%d,%d) = %f*%f + %f*(%f + %f) = %f",(i+1),j,(1.0 - 2.0 * lambda),solucao[i][j],
					lambda,solucao[i][j + 1],solucao[i][j - 1],solucao[i + 1][j]);*/
		}

		solucao[i + 1][1] = (1.0 - 2.0 * lambda) * solucao[i][1] +
			lambda * (solucao[i][2] + solucao[i][1]);
		solucao[i + 1][nx - 2] = (1.0 - 2.0 * lambda) * solucao[i][nx - 1] +
			lambda * ((solucao[i][nx - 2] + k) + solucao[i][nx - 3]);

		solucao[i + 1][0] = solucao[i + 1][1];
		solucao[i + 1][nx - 1] = solucao[i + 1][nx - 2] + k;
		/*Eh usado: fprintf(stdout, "\n phi(%d,%d) = %f",i+1,(nx-1),solucao[i+1][nx-1]);*/
	}

	//Impress�o no terminal
	for (i = 0; i < nt; i++)
	{
		fprintf(stdout, "\n i = %d",i);
		fprintf(stdout, "\n phi(%d,%d) = %f",i+1,0,solucao[i+1][0]);

		for (j = 1; j < nx - 1; j++)
		{
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

void difusao_implicito()
{
	FILE *outf;

	float matriz_phi[50][50], // matriz_phi guarda a matriz tridiagonal dos coeficientes     (matriz A)
		igualdade[50], //a matriz igualdade guarda os phi(i,j)    (matriz b)
		solucao[50], //solucao � a matriz que guarda os phi(i+1,j)     (matriz X da equa��o: A*X = b)
		t, t0, tfim, x, x0, xfim, h, k, alfa, lambda;

	int i, j, nx, nt;

	if ((outf = fopen("difusao_i.dat", "w")) == NULL)
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

	nt = (int) ((tfim - t0)/h);
	nx = (int) ((xfim - x0)/k);

	fprintf(stdout, "\n Numero de intervalos temporais %d e espaciais %d", nt, nx);

	/* Parametros fisicos e variavel auxiliar */

	alfa = 1.0;

	lambda = alfa * h/(k * k);
	fprintf(stdout, "\n lambda(k*k) = %f",lambda);

	/* Condicao inicial*/
	for (j = 0; j < nx - 1; j++)
	{
		igualdade[j] = f(x);
		/*fprintf(stdout, "\n matriz_phi[0][%d] = %f", j, matriz_phi[0][j]);*/

		x += k;
	}
	igualdade[nx - 1] += lambda*k;

	/* Condicao de contorno */
	for (i = 0; i < nx - 1; i++)
	{
		fprintf(stdout, "\n i = %d",i);
		t += h;

		for (j = 0; j < nx - 1; j++)
		{
			if (j==i)
				matriz_phi[i][j] = (1.0 + 2.0 * lambda);
			else if ((j==(i+1)) || (j==(i-1)))
				matriz_phi[i][j] = lambda;
			else
				matriz_phi[i][j] = 0.0;
		}

		matriz_phi[0][0] -= lambda;
		matriz_phi[nx - 1][nx - 1] -= lambda;
	}

	x = x0;

	for (j = 0; j < nx; j++)
	{
		fprintf(outf, "%f ", x);
		for (i = 0; i < nt; i++)
		{
			fprintf(outf, "%f ", matriz_phi[i][j]);
		}
		fprintf(outf, "\n");
		x += k;
	}
}
