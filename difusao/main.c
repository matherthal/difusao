/*
 * main.c
 *
 *  Created on: Jun 28, 2009
 *      Author: matheus
 */

#include "difusao_e.h"

int main(void)
{
	fprintf(stdout, "Universidade Federal Fluminense - UFF");
	fprintf(stdout, "\nAluno: Matheus de S� Erthal");
	fprintf(stdout, "\nLista de exerc�cios 2009 - Quest�o II");
	fprintf(stdout, "\nResolver a Equa��o de Difus�o por m�todos impl�cito e expl�cito com condi��o de contorno de Neumann.\n");

	fprintf(stdout, "\nPara impl�cito aperte 1\nPara expl�cito aperte 2");

	difusao_explicito();
}
