#include <iostream>

#include "Gestor_Errores.h"

using namespace std;

void GestorErrores::genError(errores tipoError, int line, string fuente) {
  switch (tipoError) {
  case (errores::CADENA_NO_CERRADA): {
    string error = "\n \033[1;31m [-] Error Lexico [!] \033[0m en linea " +
                   to_string(line) + "\n Cadena no cerrada";
    error += ": \'" + fuente;
    error += "\n\n\twhat()\n\n ************************* \n";

    cerr << error << endl;
    break;
  }

  case (errores::CADENA_LARGA): {
    string error = "\n \033[1;31m [-] Error Lexico [!] \033[0m en linea " +
                   to_string(line) + "\n Cadena Demasiado larga (len=" +
                   to_string(fuente.size()) + " > 64)";
    error += "\n - \'" + fuente + "\'";
    error += "\n\n\twhat()\n\n ************************* \n";
    cerr << error << endl;
    break;
  }
  case (errores::NUMERO_FUERA_RANGO): {

    string error = "\n \033[1;31m [-] Error Lexico [!] \033[0m en linea " +
                   to_string(line) +
                   "\n Numero fuera de rango [-2^15,2^15-1]: ";
    error += fuente;
    error += "\n\n\twhat()\n\n ************************* \n";
    cerr << error << endl;
    break;
  }
  case (errores::CARACTER_NO_DEFINIDO): {
    string error = "\n \033[1;31m [-] Error Lexico [!] \033[0m en linea " +
                   to_string(line) + "\n Caracter no definido";
    error += ": " + fuente;
    error += "\n\n\twhat()\n\n ************************* \n";
    cerr << error << endl;
    break;
  }
  default:
    break;
  }
}
