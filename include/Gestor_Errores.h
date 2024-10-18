#include<string>

using namespace std;


enum errores{
  CADENA_NO_CERRADA,
  NUMERO_FUERA_RANGO,
  CADENA_LARGA,
  CARACTER_NO_DEFINIDO,

};

class GestorErrores {
public:
  void genError(errores tipoError, int line, string fuente);
};

