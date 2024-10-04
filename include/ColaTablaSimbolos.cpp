
#include "ColaTablaSimbolos.h"

using namespace std;


ColaTablaSimbolos::ColaTablaSimbolos() {
  
}

void ColaTablaSimbolos::add(TablaSimbolos symbol) {
  q.push(symbol);
}

void ColaTablaSimbolos::pop() {
  q.pop();
}

TablaSimbolos ColaTablaSimbolos::top() {
  return q.front();
}

