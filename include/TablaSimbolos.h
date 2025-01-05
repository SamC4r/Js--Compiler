#include <string>
#include <unordered_map>

using namespace std;

#pragma once

struct Entry {
    int pos;
    string lexema;
    string tipo;
    /*
    Mas columnas
  */
};

class TablaSimbolos {
public:
    int cnt;
    unordered_map<string, Entry> simbolos;
    unordered_map<int, Entry> posiciones;

public:
    TablaSimbolos();
    void print();
    Entry* getPos(int pos);
    Entry *getEntry(string id);
    void add(string id);
};
