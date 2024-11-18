
#include "analizador_lexico.h"
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;

class AnalizadorSintactico {

  AnalizadorLexico &lexico;
  GestorErrores &errores;
  fstream parse;

  stack<string> pila;

  string siguienteToken();

  set<string> terminales = {
      "var",   "function", "id",      ";",      ",",
      "if",    "else",     "(",       "{",      "}",
      ")",     "int",      "boolean", "string", "void",
      ">",     "-",        "%",       "!",      "return",
      "input", "output",   "=",       "--",     "constanteEntera",
      "cadena", "lambda"

  };

  set<string> noTerminales = {"Z", "B", "T", "E", "O", "M", "P", "L", "Q", "I",
                              "J", "C", "S", "D", "X", "F", "H", "A", "K"};

std::map<std::string, std::string> token_char = {
    {"operadorAsignacion", "="},
    {"coma", ","},
    {"puntoComa", ";"},
    {"parentesisIzda", "("},
    {"parentesisDcha", ")"},
    {"llavesIzda", "{"},
    {"llavesDcha", "}"},
    {"operadorMenos", "-"},
    {"operadorModulo", "%"},
    {"operadorMayor", ">"},
    {"operadorNegacion", "!"},
};
  map<string, int> producciones = {
    {"Z -> B Z", 1},
    {"Z -> F Z", 2},
    {"Z -> lambda", 3},

    {"B -> var T id ;", 4},
    {"B -> if ( E ) I", 5},
    {"B -> S", 6},

    {"T -> int", 7},
    {"T -> boolean", 8},
    {"T -> string", 9},

    {"E -> R N", 10},

    {"N -> > R N", 11},
    {"N -> lambda", 12},

    {"R -> O P", 13},

    {"P -> - O P", 14},
    {"P -> lambda", 15},

    {"O -> M Y", 16},

    {"Y -> % M Y", 17},
    {"Y -> lambda", 18},

    {"M -> - M", 19},
    {"M -> ! M", 20},
    {"M -> id V", 21},
    {"M -> ( E )", 22},
    {"M -> constanteEntera", 23},
    {"M -> cadena", 24},
    {"M -> -- id", 25},

    {"V -> ( L )", 26},
    {"V -> lambda", 27},

    {"L -> E Q", 28},
    {"L -> lambda", 29},

    {"Q -> , E Q", 30},
    {"Q -> lambda", 31},

    {"I -> S", 32},
    {"I -> { C } J", 33},

    {"J -> else { C }", 34},
    {"J -> lambda", 35},

    {"C -> B C", 36},
    {"C -> lambda", 37},

    {"S -> id U", 38},
    {"S -> output E ;", 39},
    {"S -> input D ;", 40},
    {"S -> return X ;", 41},

    {"U -> = E ;", 42},
    {"U -> ( L ) ;", 43},

    {"D -> id", 44},
    {"D -> ( id )", 45},

    {"X -> E", 46},
    {"X -> lambda", 47},

    {"F -> function H id ( A ) { C }", 48},

    {"H -> T", 49},
    {"H -> void", 50},

    {"A -> T id K", 51},
    {"A -> void", 52},

    {"K -> , T id K", 53},
    {"K -> lambda", 54}
};

  map<pair<string, string>, string> M = {
      {{"Z", "id"}, "B Z"},
      {{"Z", "var"}, "B Z"},
      {{"Z", "function"}, "F Z"},
      {{"Z", "input"}, "B Z"},
      {{"Z", "output"}, "B Z"},
      {{"Z", "return"}, "B Z"},
      {{"Z", "if"}, "B Z"},
      {{"Z", "$"}, "lambda"},
      {{"B", "id"}, "S"},
      {{"B", "var"}, "var T id ;"},
      {{"B", "input"}, "S"},
      {{"B", "output"}, "S"},
      {{"B", "return"}, "S"},
      {{"B", "if"}, "if ( E ) I"},
      {{"T", "boolean"}, "boolean"},
      {{"T", "int"}, "int"},
      {{"T", "string"}, "string"},
      {{"E", "id"}, "R N"},
      {{"E", "cadena"}, "R N"},
      {{"E", "constanteEntera"}, "R N"},
      {{"E", "("}, "R N"},
      {{"E", "!"}, "R N"},
      {{"E", "--"}, "R N"},
      {{"N", ")"}, "lambda"},
      {{"N", ";"}, "lambda"},
      {{"N", ">"}, "> R N"},
      {{"R", "id"}, "O P"},
      {{"R", "cadena"}, "O P"},
      {{"R", "constanteEntera"}, "O P"},
      {{"R", "("}, "O P"},
      {{"R", "!"}, "O P"},
      {{"R", "--"}, "O P"},
      {{"P", ")"}, "lambda"},
      {{"P", ";"}, "lambda"},
      {{"P", "-"}, "- O P"},
      {{"P", ">"}, "lambda"},
      {{"O", "id"}, "M Y"},
      {{"O", "cadena"}, "M Y"},
      {{"O", "constanteEntera"}, "M Y"},
      {{"O", "("}, "M Y"},
      {{"O", "!"}, "M Y"},
      {{"O", "--"}, "M Y"},
      {{"Y", ")"}, "lambda"},
      {{"Y", ";"}, "lambda"},
      {{"Y", "-"}, "lambda"},
      {{"Y", ">"}, "lambda"},
      {{"Y", "%"}, "% M Y"},
      {{"M", "id"}, "id V"},
      {{"M", "cadena"}, "cadena"},
      {{"M", "constanteEntera"}, "constanteEntera"},
      {{"M", "("}, "( E )"},
      {{"M", "!"}, "! M"},
      {{"M", "--"}, "-- id"},
      {{"V", "("}, "( L )"},
      {{"V", ")"}, "lambda"},
      {{"V", ";"}, "lambda"},
      {{"V", "-"}, "lambda"},
      {{"V", "%"}, "lambda"},
      {{"V", ">"}, "lambda"},
      {{"L", "id"}, "E Q"},
      {{"L", "cadena"}, "E Q"},
      {{"L", "constanteEntera"}, "E Q"},
      {{"L", "("}, "E Q"},
      {{"L", ")"}, "lambda"},
      {{"L", "!"}, "E Q"},
      {{"L", "--"}, "E Q"},
      {{"Q", ")"}, "lambda"},
      {{"Q", ","}, ", E Q"},
      {{"I", "id"}, "S"},
      {{"I", "input"}, "S"},
      {{"I", "output"}, "S"},
      {{"I", "return"}, "S"},
      {{"I", "{"}, "{ C } J"},
      {{"J", "else"}, "else { C }"},
      {{"J", "var"}, "lambda"},
      {{"J", "$"}, "lambda"},
      {{"C", "id"}, "B C"},
      {{"C", "var"}, "B C"},
      {{"C", "input"}, "B C"},
      {{"C", "output"}, "B C"},
      {{"C", "return"}, "B C"},
      {{"C", "if"}, "B C"},
      {{"C", "}"}, "lambda"},
      {{"S", "id"}, "id U"},
      {{"S", "input"}, "input D ;"},
      {{"S", "output"}, "output E ;"},
      {{"S", "return"}, "return X ;"},
      {{"U", "("}, "( L ) ;"},
      {{"U", "="}, "= E ;"},
      {{"D", "id"}, "id"},
      {{"D", "("}, "( id )"},
      {{"X", "id"}, "E"},
      {{"X", "cadena"}, "E"},
      {{"X", "constanteEntera"}, "E"},
      {{"X", "("}, "E"},
      {{"X", ";"}, "lambda"},
      {{"X", "!"}, "E"},
      {{"X", "--"}, "E"},
      {{"F", "function"}, "function H id ( A ) { C }"},
      {{"H", "boolean"}, "T"},
      {{"H", "int"}, "T"},
      {{"H", "string"}, "T"},
      {{"H", "void"}, "void"},
      {{"A", "boolean"}, "T id K"},
      {{"A", "int"}, "T id K"},
      {{"A", "string"}, "T id K"},
      {{"A", "void"}, "void"},
      {{"K", ")"}, "lambda"},
      {{"K", ","}, ", T id K"},
      {{"E", "-"}, "R N"},
      {{"N", ","}, "lambda"},
      {{"R", "-"}, "O P"},
      {{"P", ","}, "lambda"},
      {{"O", "-"}, "M Y"},
      {{"Y", ","}, "lambda"},
      {{"M", "-"}, "- M"},
      {{"V", ","}, "lambda"},
      {{"L", "-"}, "E Q"},
      {{"J", "id"}, "lambda"},
      {{"J", "function"}, "lambda"},
      {{"J", "input"}, "lambda"},
      {{"J", "output"}, "lambda"},
      {{"J", "return"}, "lambda"},
      {{"J", "if"}, "lambda"},
      {{"X", "-"}, "E"}
  };

  void error(string unexpected);

public:
  AnalizadorSintactico(AnalizadorLexico &lexico, GestorErrores &errores);
};
