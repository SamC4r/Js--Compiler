
#include <filesystem> //
#include <iostream>

#include "ColaTablaSimbolos.h"
#include "analizador_lexico.h"

#define all(x) (x).begin(), (x).end()
#define endl "\n"
#define pb push_back

using namespace std;

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(NULL);

  cout << filesystem::current_path() << endl;

  string filename = "ejemplo.jsm";
  string token_file = "tokens.txt";
  string ts_file = "tabla_simbolos.txt";

  ColaTablaSimbolos queue;
  GestorErrores errores;
  AnalizadorLexico lexico(filename, token_file, ts_file, queue,errores);

  return 0;
}
