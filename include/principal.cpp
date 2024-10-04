
#include<filesystem> // 
#include<iostream>

#include "tabla-simbolos.h"
#include "analizador_lexico.h"
#include "TablaSimbolos.h"
#include "ColaTablaSimbolos.h"

 
#define all(x) (x).begin(), (x).end()
#define endl "\n"
#define pb push_back
 
using namespace std;
 
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(NULL);

  cout << filesystem::current_path() << endl;

  string filename = "ejemplo.jsm";
  string token_file="tokens.txt";

  ColaTablaSimbolos queue;
  AnalizadorLexico lexico(filename,token_file,queue);

 
  return 0;
}
