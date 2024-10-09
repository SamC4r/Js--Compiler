#include <string>
#include <unordered_map>

using namespace std;

#pragma once

struct Entry {
  int pos;
  string lexema;
  string atributo;
};

class TablaSimbolos {
  int cnt;
  unordered_map<string, Entry> simbolos;

public:
  TablaSimbolos();
  void print();
  int get(string id);
  Entry getEntry(string id);
  void add(string id);
};
