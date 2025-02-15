#include <fstream>
#include <stack>

#include "TablaSimbolos.h"

using namespace std;

#pragma once

class ColaTablaSimbolos {

private:
  stack<TablaSimbolos*> q;
  fstream ts_file;

public:
  ColaTablaSimbolos(string ts_file_name);
  void add(TablaSimbolos* symbol);
  void pop();
  int size();
  TablaSimbolos *top();
  void print(string nombre);
};
