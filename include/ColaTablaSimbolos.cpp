#include "ColaTablaSimbolos.h"
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

ColaTablaSimbolos::ColaTablaSimbolos(string ts_file_name) {
    ts_file.open(ts_file_name, fstream::out);
}

void ColaTablaSimbolos::add(TablaSimbolos* symbol) { q.push(symbol); }

void ColaTablaSimbolos::pop() { q.pop(); }

TablaSimbolos* ColaTablaSimbolos::top() { return q.top(); }

struct ts_content{
    int id;
    Entry entry;
    friend bool operator<(const ts_content& lhs, const ts_content& rhs){
        return lhs.id < rhs.id;
    }
};

void ColaTablaSimbolos::print() {
    TablaSimbolos* ambito_actual = top();

    ts_file << "Contenidos de la Tabla # " << q.size() << " :" << endl;

    vector<ts_content> ids;

    for (auto row : ambito_actual->posiciones) {
        Entry entry = row.second;
        ids.push_back({row.first, entry});
    }

    // sort by pos number and output by pos number
    sort(ids.begin(), ids.end());

    for (auto p : ids) {
        Entry e = p.entry;
        ts_file << "  * LEXEMA : \'" << e.lexema << '\'' << endl;
        if(e.tipo != ""){
            ts_file << "    ATRIBUTOS :" << endl;
            ts_file << "    + tipo : " << e.tipo << endl;
        }
        ts_file << "   --------- ----------" << endl;
  }
}
