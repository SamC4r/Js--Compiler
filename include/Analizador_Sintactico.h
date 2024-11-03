
#include "analizador_lexico.h"
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class AnalizadorSintactico {

  AnalizadorLexico &lexico;
  GestorErrores &errores;

  string siguienteToken();

  set<string> terminales = {
      "var",   "function", "id",      ";",      ",",
      "if",    "else",     "(",       "{",      "}",
      ")",     "int",      "boolean", "string", "void",
      ">",     "-",        "%",       "!",      "return",
      "input", "output",   "=",       "--",     "constanteEntera",
      "cadena"

  };

  set<string> noTerminales = {"Z", "B", "T", "E", "O", "M", "P", "L", "Q", "I",
                              "J", "C", "S", "D", "X", "F", "H", "A", "K"};

  map<string, vector<string>> producciones;

public:
  AnalizadorSintactico(AnalizadorLexico &lexico, GestorErrores &errores);
};
