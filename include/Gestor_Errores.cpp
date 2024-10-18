#include<string>
#include <iostream>

using namespace std;


enum errores{
  CADENA_NO_CERRADA,
  NUMERO_FUERA_RANGO,
  CADENA_LARGA,

};

class GestorErrores {
public:
  void Error(string error) {
    cout << "Error: " << error << endl;
  }

  void genError(errores tipoError, int line, string fuente) {
    switch(tipoError){
      case(errores::CADENA_NO_CERRADA): {
        string error="\n[-] Error Lexico [-] en linea: " + to_string(line) + "\n Cadena no cerrada";
        error+="\n - "+fuente+":"+to_string(line);
        error+="\n\twhat()\n\n ************************* \n";

        cerr << error << endl;
        break;
      }

      case(errores::CADENA_LARGA):{
        string error="\n[-] Error Lexico [-] en linea: " + to_string(line) + "\n Cadena Demasiado larga (len=" + to_string(fuente.size()) + " > 64) --> \'";
        error+="\n - "+fuente+":"+to_string(line);
        error+="\n\twhat()\n\n ************************* \n";
        cerr << error << endl;

      }
      case(errores::NUMERO_FUERA_RANGO):{

      string error="\n[-] Error Lexico [-] en linea: " + to_string(line) + "\n Numero fuera de rango [-2^15,2^15-1] --> " + fuente;
      error+="\n - "+fuente+":"+to_string(line);
      error+="\n\twhat()\n\n ************************* \n";
      cerr << error << endl;

      }
    }
  }

};
