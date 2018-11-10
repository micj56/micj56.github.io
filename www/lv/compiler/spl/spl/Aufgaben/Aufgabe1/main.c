/*
 * main.c -- an interpreter for SLPL
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "slpl.h"
#include "table.h"
#include "prog.h"


void zeigeBaum(Knoten *baum) {
  /******************************/
  /* Das muessen Sie schreiben! */
  /******************************/
  printf("Hier sollte die von Ihnen programmierte ");
  printf("Ausgabe des Programmbaumes stehen!");
}


void interpretiere(Knoten *baum) {
  /******************************/
  /* Das muessen Sie schreiben! */
  /******************************/
  printf("Hier sollte die Ausgabe des von Ihnen ");
  printf("programmierten Interpreters stehen!");
}


int main(void) {
  Knoten *programm;

  programm = konstruiereProgramm();
  zeigeBaum(programm);
  printf("\n");
  interpretiere(programm);
  printf("\n");
  return 0;
}
