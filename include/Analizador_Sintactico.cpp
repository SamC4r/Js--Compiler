#include "Analizador_Sintactico.h"
#include <iostream>

AnalizadorSintactico::AnalizadorSintactico(AnalizadorLexico &lexico,
                                           GestorErrores &errores)
    : lexico(lexico), errores(errores) {
  while (siguienteToken() != "EOF")
    ;
};

string AnalizadorSintactico::siguienteToken() {

  string s = lexico.getToken();
  cout << "token: " << s << endl;
  return s;
}
