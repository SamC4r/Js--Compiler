#include <string>
#include <unordered_map>

using namespace std;

#pragma once

struct Entry {
  int pos;
  string lexema;
  string atributo;
  /*
    Mas columnas
  */
};

class TablaSimbolos {
public:
  int cnt;
  unordered_map<string, Entry> simbolos;

public:
  TablaSimbolos();
  void print();
  int getPos(string id);
  Entry *getEntry(string id);
  void add(string id);
};
