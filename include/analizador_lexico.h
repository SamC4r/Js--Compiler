#include "ColaTablaSimbolos.h"
#include "Generator.h"
#include "Gestor_Errores.h"
#include <fstream>
#include <string>

using namespace std;

class AnalizadorLexico {
private:
  fstream programa;
  GestorErrores errores;

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
    Generator generator;
  int lineas;
  pair<string,string> getToken();
  AnalizadorLexico(string nombre, string token_file, ColaTablaSimbolos &queue,
                   GestorErrores &errores);
};
