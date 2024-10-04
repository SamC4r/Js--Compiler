#include<queue>


#include "TablaSimbolos.h"

using namespace std;

#pragma once

class ColaTablaSimbolos{

private:
  queue<TablaSimbolos> q;

public:
  ColaTablaSimbolos();
  void add(TablaSimbolos symbol);
  void pop();
  TablaSimbolos top();
  void search(string id);
};
