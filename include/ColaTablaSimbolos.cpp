#include "ColaTablaSimbolos.h"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

ColaTablaSimbolos::ColaTablaSimbolos(string ts_file_name) {
  ts_file.open(ts_file_name, fstream::out);
}

void ColaTablaSimbolos::add(TablaSimbolos symbol) { q.push(symbol); }

void ColaTablaSimbolos::pop() { q.pop(); }

TablaSimbolos *ColaTablaSimbolos::top() { return &q.top(); }

void ColaTablaSimbolos::print() {
  TablaSimbolos *ambito_actual = top();

  ts_file << "Contenidos de la Tabla # " << q.size() << " :" << endl;

  vector<pair<int, string>> ids;

  for (auto row : ambito_actual->simbolos) {
    Entry entry = row.second;
    ids.push_back({entry.pos, entry.lexema});
  }

  // sort by pos number and output by pos number
  sort(ids.begin(), ids.end());

  for (auto p : ids) {
    ts_file << "  * LEXEMA : \'" << ambito_actual->getEntry(p.second)->lexema
            << '\'' << endl;
  }
}
