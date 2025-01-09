#include <string>
#include <unordered_map>
#include<vector>

using namespace std;

#pragma once

struct Function{
    string etiqueta;
    int n_params;
    vector<string> tipo_params;
    string ret;
};

struct Entry {
    int pos;
    string lexema;
    string tipo;
    int desp;
    
    Function f;

    /*
    Mas columnas
  */
};

class TablaSimbolos {
public:
    int cnt;
    int global_desp;
    string name;
    unordered_map<string, Entry> simbolos;
    unordered_map<int, Entry> posiciones;

public:
    TablaSimbolos();
    void print();
    Entry* getPos(int pos);
    Entry *getEntry(string id);
    void add(string id);

};
