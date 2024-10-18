/*

Implementacion de los metodos de la clase TablaSimbolos

*/

#include "TablaSimbolos.h"
#include <iostream>

using namespace std;

TablaSimbolos::TablaSimbolos() { this->cnt = 0; }

void TablaSimbolos::print() { cout << "hola " << endl; }

void TablaSimbolos::add(string id) {
  simbolos[id] = {cnt++, id, "var"};
  // cout << simbolos[id].pos << endl;
}

int TablaSimbolos::get(string id) {
  if (!simbolos.count(id)) {
    return -1;
  } else {
    return simbolos[id].pos;
  }
}

Entry TablaSimbolos::getEntry(string id) { return simbolos[id]; }
