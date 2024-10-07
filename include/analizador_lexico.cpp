#include "analizador_lexico.h"
#include <ios>
#include<iostream>
#include <fstream>
#include<string>
#include <unordered_map>
#include "Generator.h"
#include "TablaSimbolos.h"
#include "ColaTablaSimbolos.h"

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


int cnt=0;

template<typename T1, typename T2>
void Generator::gen_token(T1 tipo, T2 atributo){
  token_file << "<" << tipo << ", " << atributo << ">" << endl;
}


void Generator::init(string file_name,ColaTablaSimbolos &queue){
  token_file.open(file_name,fstream::out);
  this->queue=queue;

  TablaSimbolos global;
  this->queue.add(global);
  cout << "global added" << endl;
}

void Generator::Token(string identificador){

  if(codigo_palabra_reservada.count(identificador)){
    gen_token("palabraReservada",codigo_palabra_reservada[identificador]);
  }else{

    TablaSimbolos* simbolos = queue.top();
    if(simbolos->get(identificador) == -1){
      simbolos->add(identificador);
      cout << "simbolo nuevo: " << simbolos->get(identificador) << endl;
    }
    gen_token("id",simbolos->get(identificador));
  }
}

void Generator::Token(char c){
  gen_token(tipo_caracter_especial[c],'-');
}

void Generator::Token(int valor){
  gen_token("constanteEntera",valor);
}

void Generator::Token(TiposToken tipo){
  if(tipo == TiposToken::OPERADOR_ESPECIAL)
    gen_token("operadorEspecial",'-');

}

void Generator::Token(string str,char del){
  if(del == '\'') gen_token("cadena",str);
}



bool AnalizadorLexico::is_delimiter(char c){
  return (c == ' ' || c == '\n' || c == '\t');
}

bool AnalizadorLexico::is_eof(char c){
  return (c == EOF);
}

bool AnalizadorLexico::d(char c){
  return (c >= '0' && c <= '9');
}

bool AnalizadorLexico::l(char c){
  return ((c >='a' && c<='z') || (c >= 'A' && c<='Z') || c == '_');
}

bool AnalizadorLexico::caracter_especial(char c){
  return (c == '=' || c == '%' || c == '-' || c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',' || c == '%' || c == '>' || c == '!');
}

bool AnalizadorLexico::cadena(char c){
  return (c == '\'');
}


bool AnalizadorLexico::menos(char c){
  return (c == '-');
}

char AnalizadorLexico::leer_digito(){
  int number=0;
  char c;
  while(d(c=programa.peek()) && !programa.eof()){
    number = number * 10 + (c - '0');
    c=programa.get();
  }
  cerr << "Numero: " << number << endl;
  if(number >= (1 << 15)){

    throw runtime_error("Numero demasiado grande");
  }
  generator.Token(number);
  return c;
}

char AnalizadorLexico::leer_letra(){
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


char AnalizadorLexico::predecremento(){
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
    cerr << "<menos,->" << ' ' << c << endl;
    generator.Token(c);
  }
  return c;
}

char AnalizadorLexico::cadena(){
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
  }
  if(str.size() > 64){
    throw runtime_error("Cadena demasiado larga");
  }
  
  //operador menos
  generator.Token(str,'\'');
 
  return c;
}

AnalizadorLexico::AnalizadorLexico (string nombre,string token_file,ColaTablaSimbolos &queue){
  programa.open(nombre, ios::in);
  generator.init(token_file,queue);
  

  if(!programa.is_open()){
    cout << "[+] Error abriendo archivo" << endl;
    return;
  }
  string s = "";
  char c;
  cout << "jijiji" << endl;
  bool negativo=false;
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
      debug(c);
    }

    if(is_delimiter(c)){
      cerr << "Delimiter: " << (int) c << endl;
    }else if(caracter_especial(c)){

      if(menos(c)){
        negativo=true;
        continue;
      }
      if(cadena(c))continue;

      cerr << "especial: " << c << endl;
      generator.Token(c);
    }else if(d(c) || l(c))continue;
    c=programa.get();
  }
}


