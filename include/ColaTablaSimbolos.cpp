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

int ColaTablaSimbolos::size() { return q.size(); }

TablaSimbolos* ColaTablaSimbolos::top() { return q.top(); }

struct ts_content{
    int id;
    Entry entry;
    friend bool operator<(const ts_content& lhs, const ts_content& rhs){
        return lhs.id < rhs.id;
    }
};



void ColaTablaSimbolos::print(string nombre) {
    static int it = 2;
    TablaSimbolos* ambito_actual = top();

    ts_file << "Contenidos de la Tabla " << nombre << " # " << (nombre == "principal"  ? 1:it++) << " :" << endl;

    vector<ts_content> ids;
    cerr << ambito_actual->posiciones.size() << endl;
    for (auto row : ambito_actual->posiciones) {
        Entry entry = row.second;
        ids.push_back({row.first, entry});
        cerr << entry.lexema << endl;
    }

    // sort by pos number and output by pos number
    sort(ids.begin(), ids.end());
    for (auto p : ids) {
        Entry e = p.entry;
        ts_file << "  * LEXEMA : \'" << e.lexema << '\'' << endl;
        if(e.tipo != ""){
            ts_file << "    ATRIBUTOS :" << endl;
            ts_file << "    + Tipo : \'" << e.tipo << '\'' << endl;
        }

        if(e.tipo != "function"){
            ts_file << "    + Despl : " << e.desp << endl;
        }else{
            ts_file << "    + numParam : " << e.f.n_params << endl;
            int n=1;
            for(string t : e.f.tipo_params){
                ts_file << "    + TipoParametro" << n++ << " : \'" << t << "\'"<< endl;
            }
            ts_file << "    + TipoRetorno : \'" << e.f.ret << "\'"<< endl;
            ts_file << "    + Etiqueta : \'" << e.lexema << "\'"<< endl;
        }
        ts_file << "   --------- ----------" << endl;
    }
}
