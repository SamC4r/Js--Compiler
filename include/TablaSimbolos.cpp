/*

Implementacion de los metodos de la clase TablaSimbolos

*/

#include "TablaSimbolos.h"
#include <iostream>

using namespace std;

TablaSimbolos::TablaSimbolos() { 
    this->cnt = 0; 
    this->global_desp = 0;
}

void TablaSimbolos::print() { 
    for (auto x : simbolos) { 
        cout << x.first << " " << x.second.lexema << " " << x.second.pos << endl; 
    } 
}

void TablaSimbolos::add(string id) {
    int desp = this->global_desp;
    posiciones[cnt] = {cnt,id,"entero",desp};
    simbolos[id] = {cnt++, id,"entero",desp}; 
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
