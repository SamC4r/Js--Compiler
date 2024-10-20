/*

Implementacion de los metodos de la clase TablaSimbolos

*/

#include "TablaSimbolos.h"
#include <iostream>

using namespace std;

TablaSimbolos::TablaSimbolos() { this->cnt = 0; }

void TablaSimbolos::print() { cout << "hola " << endl; }

void TablaSimbolos::add(string id) { simbolos[id] = {cnt++, id, "var"}; }

Entry *TablaSimbolos::getEntry(string id) {
  if (!simbolos.count(id)) {
    return NULL;
  }
  return &simbolos[id];
}
