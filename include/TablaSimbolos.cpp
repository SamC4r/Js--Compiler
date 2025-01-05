/*

Implementacion de los metodos de la clase TablaSimbolos

*/

#include "TablaSimbolos.h"
#include <iostream>

using namespace std;

TablaSimbolos::TablaSimbolos() { this->cnt = 0; }

void TablaSimbolos::print() { 
    for (auto x : simbolos) { 
        cout << x.first << " " << x.second.lexema << endl; 
    } 
}

void TablaSimbolos::add(string id) {
    posiciones[cnt] = {cnt,id,"entero"};
    simbolos[id] = {cnt++, id,"entero"}; 
}

Entry* TablaSimbolos::getPos(int pos) { 
    return &posiciones[pos]; 
}

Entry* TablaSimbolos::getEntry(string id) {
  if (!simbolos.count(id)) {
    return NULL;
  }
  return &simbolos[id];
}
