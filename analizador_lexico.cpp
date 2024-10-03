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

};

struct Generator{

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

  unordered_map<char, string> tipo_caracter_especial ={
    {'=',"operadorAsignacion"},
    {',',"coma"},
    {';',"puntoComa"},
    {'(',"parentesisIzda"},
    {')',"parentesisDcha"},
    {'{',"llavesIzda"},
    {'}',"llavesDcha"},
    {'-',"operadorMenos"},
    {'%',"operadorModulo"},
    {'>',"opeardorMayor"},
    {'!',"operadorNegacion"},
  };

  unordered_map<string,string> operadoresEspeciales = {
    {"--","operadorEspecial"},
  };

  int cnt=0;
  fstream token_file;
  template<typename T1, typename T2>
  void gen_token(T1 tipo, T2 atributo){
    token_file << "<" << tipo << ", " << atributo << ">" << endl;
  }


  void init(string file_name){
    token_file.open(file_name,fstream::out);
  }

  void Token(string identificador){

    if(codigo_palabra_reservada.count(identificador)){
      gen_token("palabraReservada",codigo_palabra_reservada[identificador]);
    }else{
      gen_token("id",++cnt);
    }
  }

  void Token(char c){
    gen_token(tipo_caracter_especial[c],'-');
  }

  void Token(int valor){
    gen_token("constanteEntera",valor);
  }

  void Token(TiposToken tipo){
    gen_token("operadorEspecial",'-');
  }

  void Token(string str,char del){
    if(del == '\'') gen_token("cadena",str);
  }

};

class AnalizadorLexico{

private:
  fstream programa;
  Generator generator;

  bool is_delimiter(char c){
    return (c == ' ' || c == '\n' || c == '\t');
  }

  bool is_eof(char c){
    return (c == EOF);
  }

  bool d(char c){
    return (c >= '0' && c <= '9');
  }

  bool l(char c){
    return ((c >='a' && c<='z') || (c >= 'A' && c<='Z'));
  }

  bool caracter_especial(char c){
    return (c == '=' || c == '%' || c == '-' || c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',' || c == '%' || c == '>' || c == '!');
  }

  bool cadena(char c){
    return (c == '\'');
  }


  bool menos(char c){
    return (c == '-');
  }

  char leer_digito(){
    int number=0;
    char c;
    while(d(c=programa.peek()) && !programa.eof()){
      number = number * 10 + (c - '0');
      c=programa.get();
    }
    cerr << "Numero: " << number << endl;
    generator.Token(number);
    return c;
  }

  char leer_letra(){
    string palabra="";
    char c;
    while((l(c = programa.peek()) || d(c)) && !programa.eof()){
      palabra+=c;
      c=programa.get();
    }
    cerr << "Palabra: " << palabra << endl;
    generator.Token(palabra);
    return c;
  }


  char predecremento(){
    string decremento = "";
    char c;
    int cnt=0;
    while(menos(c=programa.peek()) && !programa.eof()){
      decremento+=c;
      c=programa.get();
      cnt++;
    }
    if(cnt > 2){
      throw runtime_error("Expresion expected");
    }else if(cnt == 2){
      cerr << "predecremento: " << decremento << endl;
      generator.Token(TiposToken::OPERADOR_ESPECIAL);
    }else{
      //operador menos
      generator.Token(c);
    }
    return c;
  }

  char cadena(){
    string str = "";
    char c = programa.get(); //to skip '  --> place pointer on it. Then test for the next ones
    int cnt=1;
    while(!cadena(c=programa.peek()) && !programa.eof()){
      str+=c;
      c=programa.get();
    }
    debug(str);
    if(cnt > 2){
      throw runtime_error("Cadena no cerrada");
    }else{
      //operador menos
      generator.Token(str,'\'');
    }
    return c;

  }

public:
  AnalizadorLexico(string nombre,string token_file){
    programa.open(nombre, ios::in);
    generator.init(token_file);

    if(!programa.is_open()){
      cout << "[+] Error abriendo archivo" << endl;
      return;
    }
    string s = "";
    char c;
    while (programa && !programa.eof()){
      c=programa.peek();
      if(l(c)){
        c=leer_letra();
      }else if(d(c)){
        c=leer_digito();
      }else if(menos(c)){
        c=predecremento();
      }else if(cadena(c)){
        c=cadena();
      }

      if(is_delimiter(c)){
        cerr << "Delimiter: " << (int) c << endl;
      }else if(caracter_especial(c)){

        if(menos(c))continue;
        if(cadena(c))continue;

        cerr << "especial: " << c << endl;
        generator.Token(c);
      }
      c=programa.get();
    }
  }
};

int main(int argc, char * argv[]){

  string filename = "ejemplo.jsm";
  string token_file="tokens.txt";

  AnalizadorLexico lexico(filename,token_file);



  return 0;
}

