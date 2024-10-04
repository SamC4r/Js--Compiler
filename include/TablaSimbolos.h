#include<iostream>

using namespace std;


#pragma once

class TablaSimbolos{
private:
  int value;
public:
  TablaSimbolos();
  void print();
  bool existe(string id);
};
