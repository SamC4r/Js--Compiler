#include <ios>
#include<iostream>
#include <fstream>
#include<string>
#include <unordered_map>
#include "Generator.h"


using namespace std;


class AnalizadorLexico{
private:
  fstream programa;
  Generator generator;

  bool is_delimiter(char c);
  bool is_eof(char c);
  bool d(char c);
  bool l(char c);
  bool caracter_especial(char c);
  bool cadena(char c);
  bool menos(char c);
  char leer_digito();
  char leer_letra();
  char predecremento();
  char cadena();

public:
  AnalizadorLexico(string nombre,string token_file);

};

vector<pair<char,int>> v;
