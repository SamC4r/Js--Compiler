#include "analizador_lexico.h"
#include "ColaTablaSimbolos.h"
#include "Generator.h"
#include "TablaSimbolos.h"
#include "TiposToken.h"
#include <fstream>
#include <ios>
#include <iostream>
#include <string>
#include <unordered_map>

#define endl "\n"

using namespace std;
int cnt = 0;
int lineas=0;
string fuente="";

template <typename T1, typename T2>
void Generator::gen_token(T1 tipo, T2 atributo) {
  token_file << "<" << tipo << ", " << atributo << ">" << endl;
}
template <typename T1> void Generator::gen_token(T1 tipo) {
  token_file << "<" << tipo << ",>" << endl;
}
template <typename T1> void Generator::gen_token(T1 tipo, string cadena) {
  token_file << "<" << tipo << ",\'" << cadena << "\'>" << endl;
}

void Generator::init(string token_file_name, ColaTablaSimbolos &queue,
                     string ts_file_name) {

  token_file.open(token_file_name, fstream::out);
  ts_file.open(ts_file_name, fstream::out);

  // Esto solo por ahora
  ts_file << "Contenidos de la Tabla # 1 :" << endl;

  this->queue = queue;

  TablaSimbolos global;
  this->queue.add(global);
  // cout << "global added" << endl;
}

void Generator::Token(string identificador) {

  if (codigo_palabra_reservada.count(identificador)) {
    gen_token("palabraReservada", codigo_palabra_reservada[identificador]);
  } else {

    TablaSimbolos *simbolos = queue.top();
    if (simbolos->get(identificador) == -1) {
      simbolos->add(identificador);
      ts_file << "  * LEXEMA : \'" << simbolos->getEntry(identificador).lexema
              << '\'' << endl;
    }
    gen_token("id", simbolos->get(identificador));
  }
}

void Generator::Token(char c) { gen_token(tipo_caracter_especial[c]); }

void Generator::Token(int valor) { gen_token("constanteEntera", valor); }

void Generator::Token(TiposToken tipo) {
  if (tipo == TiposToken::OPERADOR_ESPECIAL)
    gen_token("operadorEspecial");
}

void Generator::Token(string str, char del) {
  if (del == '\'')
    gen_token("cadena", str);
}

bool AnalizadorLexico::is_delimiter(char c) {
  return (c == ' ' || c == '\n' || c == '\t');
}

bool AnalizadorLexico::is_eof(char c) { return (c == EOF); }

bool AnalizadorLexico::d(char c) { return (c >= '0' && c <= '9'); }

bool AnalizadorLexico::l(char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_');
}

bool AnalizadorLexico::caracter_especial(char c) {
  return (c == '=' || c == '%' || c == '-' || c == '(' || c == ')' ||
          c == '{' || c == '}' || c == ';' || c == ',' || c == '%' ||
          c == '>' || c == '!');
}

bool AnalizadorLexico::cadena(char c) { return (c == '\''); }

bool AnalizadorLexico::menos(char c) { return (c == '-'); }

char AnalizadorLexico::leer_digito() {
  int number = 0;
  char c;
  while (d(c = programa.peek()) && !programa.eof()) {
    number = number * 10 + (c - '0');
    c = programa.get();
  }
  // cerr << "Numero: " << number << endl;
  if (number >= (1 << 15)) {
    string error="\n[-] Error Lexico [-]\n Numero fuera de rango [-2^15,2^15-1] --> " + to_string(number);
    error+="\n - "+fuente+":"+to_string(lineas);
    error+="\n\twhat()\n\n ************************* \n";

    cerr << error << endl;
  }
  generator.Token(number);
  return c;
}

char AnalizadorLexico::leer_letra() {
  string palabra = "";
  char c;
  while ((l(c = programa.peek()) || d(c)) && !programa.eof()) {
    palabra += c;
    c = programa.get();
  }
  // cerr << "Palabra: " << palabra << endl;
  generator.Token(palabra);
  return c;
}

char AnalizadorLexico::predecremento() {
  string decremento = "";
  char c;
  int cnt = 0;

  while (menos(c = programa.peek()) && !programa.eof()) {
    decremento += c;
    c = programa.get();
    cnt++;
  }

  if (cnt > 2) {
    throw runtime_error("Expresion expected");
  } else if (cnt == 2) {
    // cerr << "predecremento: " << decremento << endl;
    generator.Token(TiposToken::OPERADOR_ESPECIAL);
  } else {
    // operador menos
    // cerr << "<menos,->" << ' ' << c << endl;
    generator.Token(c);
  }
  return c;
}

char AnalizadorLexico::cadena() {
  string str = "";
  char c = programa.get(); // to skip '  --> place pointer on it. Then test for
                           // the next ones
  int cnt = 1;
  while (!cadena(c = programa.peek()) && !programa.eof()) {
    str += c;
    c = programa.get();
  }

  cnt+=(c == '\'');
  if (cnt < 2) {
    string error="\n[-] Error Lexico [-]\n Cadena no cerrada";
    error+="\n - "+fuente+":"+to_string(lineas);
    error+="\n\twhat()\n\n ************************* \n";

    cerr << error << endl;
  }
  if (str.size() > 64) {
    //TODO: Anadir en gestor de errores

    int s=str.size();
    string error="\n[-] Error Lexico [-]\n Cadena Demasiado larga (len=" + to_string(s) + " > 64) --> \'";
    error+=str;
    error+="\n - "+fuente+":"+to_string(lineas);
    error+="\n\twhat()\n\n ************************* \n";

    cerr << error << endl;
  }

  // operador menos
  generator.Token(str, '\'');

  return c;
}

/*

Programa principal llama al constructor del analizador lexico pasandole la cola
de tabla de simbolos y el nombre del archivo para extraer los tokens

PARAMETROS:

  AnalizadorLexico(token_file,
Direccion_Estructura_Que_Almacena_Las_Tablas_De_Simbolos)

FUNCIONAMIENTO:
  Recorre el fichero caracter por caracter y transita segun los estados del
automata


TODO:
  No debe leer todo sino, esperar a que A.semantico pida token
*/

AnalizadorLexico::AnalizadorLexico(string nombre, string token_file,
                                   string ts_file_name,
                                   ColaTablaSimbolos &queue) {

  programa.open(nombre, ios::in);
  generator.init(token_file, queue, ts_file_name);
  fuente=nombre;

  if (!programa.is_open()) {
    cerr << "[+] Error abriendo archivo" << endl;
    return;
  }

  char c;
  bool negativo = false;

  // TODO: Comentarios

  bool comentario = false;
  bool posible_comentario = false;
  bool posible_fin_comentario = false;

  while (programa && !programa.eof()) {

    c = programa.peek();

    if (comentario && posible_fin_comentario && c == '/') {
      comentario = false;
      posible_fin_comentario = false;
      posible_comentario = false;
    }

    if (comentario && c == '*') {
      posible_fin_comentario = true;
    }

    if (comentario)
      goto next;

    if (posible_comentario && c == '*') {
      comentario = true;
    } else {
      posible_comentario = false;
    }

    if (c == '/') {
      posible_comentario = true;
      goto next;
    }

    if (l(c)) {
      c = leer_letra(); // Devuelve el caracter en el que se encuentra el
                        // puntero de lectura
    } else if (d(c)) {
      c = leer_digito();
    } else if (menos(c)) {
      c = predecremento();
    } else if (cadena(c)) {
      c = cadena();
    }

    if (is_delimiter(c)) { // En este caso hay que saltarlo
      // cerr << "Delimiter: " << (int)c << endl;
      goto next;
    } else if (caracter_especial(c)) {
      if (menos(c)) {
        negativo = true;
        continue;
        // SI es negativo hay que comprobar si se puede dar el caso de --  . Por
        // eso, vuelo
      }
      if (cadena(c))
        continue;
      // cerr << "especial: " << c << endl;
      generator.Token(c);
    } else if (d(c) || l(c))
      continue;
  next:
    if(c == '\n')lineas++;
    c = programa.get();
  }
}
