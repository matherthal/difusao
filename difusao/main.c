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
	fprintf(stdout, "\nAluno: Matheus de Sá Erthal");
	fprintf(stdout, "\nLista de exercícios 2009 - Questão II");
	fprintf(stdout, "\nResolver a Equação de Difusão por métodos implícito e explícito com condição de contorno de Neumann.\n");

	fprintf(stdout, "\nPara implícito aperte 1\nPara explícito aperte 2");

	difusao_explicito();
}
