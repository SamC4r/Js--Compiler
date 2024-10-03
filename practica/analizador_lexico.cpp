#include <ios>
#include<iostream>
#include <fstream>
#include<string>
#include <unordered_map>

#define endl "\n"

using namespace std;


// For debugging
void __print(int x) { cerr << x; }
void __print(unsigned x) { cerr << x; }
void __print(unsigned long x) { cerr << x; }
void __print(unsigned long long x) { cerr << x; }
void __print(float x) { cerr << x; }
void __print(double x) { cerr << x; }
void __print(long double x) { cerr << x; }
void __print(char x) { cerr << '\'' << x << '\''; }
void __print(const char *x) { cerr << '\"' << x << '\"'; }
void __print(const string &x) { cerr << '\"' << x << '\"'; }
void __print(bool x) { cerr << (x ? "true" : "false"); }

template <typename T, typename V> void __print(const pair<T, V> &x) {
  cerr << '{';
  __print(x.first);
  cerr << ',';
  __print(x.second);
  cerr << '}';
}
template <typename T> void __print(const T &x) {
  int f = 0;
  cerr << '{';
  for (auto &i : x)
    cerr << (f++ ? "," : ""), __print(i);
  cerr << "}";
}
void _print() { cerr << "]\n"; }
template <typename T, typename... V> void _print(T t, V... v) {
  __print(t);
  if (sizeof...(v))
    cerr << ", ";
  _print(v...);
}
#ifndef ONLINE_JUDGE
#define debug(x...)                                                            \
  cerr << "[" << #x << "] = [";                                                \
  _print(x)
#else
#define debug(x...)
#endif

enum class TiposToken{
  PALABRA_RESERVADA, 
  IDENTIFICADOR, 
  NUMERO_ENTERO, 
  CADENA,
  OPERADOR, 
  OPERADOR_ARITMETICO, 
  OPERADOR_LOGICO, 
  OPERADOR_RELACIONAL,
  OPERADOR_ASIGNACION,
  OPERADOR_ESPECIAL,
  PUNTO_COMA,
  COMA,
  PARENTESIS_IZQ,
  PARENTESIS_DER,
  LLAVE_IZQ,
  LLAVE_DER,
};

class Token{

private:
  unordered_map<string,int> codigo_palabra_reservada = {
    {"boolean",1},
    {"else",2},
    {"function",3},
    {"if",4},
    {"input",5},
    {"int",6},
    {"output",7},
    {"return",8},
    {"string",9},
    {"var",10},
    {"void",11},
  };



public:

  Token(string cadena){}

  Token(int valor){}

  Token(){}

  Token(TiposToken tipo){

  }
  
};

class AnalizadorLexico{

private:
  fstream programa;

  bool is_delimiter(char c){
    return (c == ' ' || c == '\n' || c == '\t' || c == EOF);
  }

  bool d(char c){
    return (c >= '0' && c <= '0');
  }

  bool l(char c){
    return ((c >='a' && c<='z') || (c >= 'A' && c<='Z'));
  }

  bool operador(char c){
    return (c == '=' || c == '%' || c == '-');
  }

  bool punto_coma(char c){
    return c == ';';
  }

  void leer(char first){

    char c = first;
    if(l(c)){

      string palabra="";

      while(l(c) || d(c)){
        programa.get();
        cout << "c: " << c << " " << (char) programa.peek() <<  endl;
        palabra+=c;
        c=programa.peek();
        cout << "c: " << c << " " << (char) programa.peek() <<  endl;
      }

      cout << palabra<< endl;
    }else if(d(c)){
      int num = 0;
      while(d(c)){
        c=programa.get();
        num = num*10 + (c - '0');
      }
      cout << num << endl;
    }


  }
  char leer_letra(){
    string palabra="";
    char c=programa.peek();
    while((l(programa.peek()) || d(programa.peek())) && !programa.eof()){
      palabra+=c;
      c=programa.get();
    }
    cerr << "Palabra: " << palabra << endl;
    return c;
  }
  char leer_numero(){
    int valor=0;
    char c=programa.get();
    while(d(c) && !programa.eof()){
      valor=valor*10 + (c - '0');
      c=programa.get();
    }
    cout << "Valor: " << valor << endl;
    return c;
  }
  char analizar(char c){

    if(is_delimiter(c)){
    }
    return 'a';
  }

public:
  AnalizadorLexico(string nombre){
    programa.open(nombre, ios::in);

    if(!programa.is_open()){
      cout << "[*] Error abriendo archivo" << endl;
      return;
    }
    int it = 0;
    string s = "" ;
    int number=0;
    char c = programa.peek();
    while (programa && !programa.eof() && it < 10) {

      c = programa.peek();

      if(operador(c)){
        cerr << "Operador: " << c << endl;
      }else if(is_delimiter(c)){
        cerr << "Delimiter: " << (char)c << endl;
      }else if(l(c)){
        c = leer_letra();
        debug(c);
        continue;
      }else if(d(c)){
        c = leer_numero();
        continue;
      }

      debug((char) programa.get());

    }
  }

};

int main(int argc, char * argv[]){

  string filename = "ejemplo.jsm";

  AnalizadorLexico lexico(filename);

  return 0;
}

