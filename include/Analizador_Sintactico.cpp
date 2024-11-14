#include "Analizador_Sintactico.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

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
 

void error() {
  cout << "Error 1 " << endl;
  throw std::runtime_error("Error Sintactico");
}

AnalizadorSintactico::AnalizadorSintactico(AnalizadorLexico &lexico,
                                           GestorErrores &errores)
    : lexico(lexico), errores(errores) {
  string a;
  parse.open("parse.txt", fstream::out);

  while ((a = siguienteToken()) != "EOF") {
    pila.push("$");
    pila.push("Z"); // axioma

    auto X = pila.top();
    while (X != "$" && a != "EOF") {
      cout << "element: " << X << ' ' << a << endl;
      if (X == a) {
        pila.pop();
        cout << "top" << pila.top() << endl;
        a = siguienteToken();
      } else if (terminales.count(X))
        error();
      else if (!M.count({X, a}))
        error();
      else if (M.count({X, a})) {
        cout << "esta!" << endl;
        string production = "";

        cout << "Produccion: [" << X << ", " << a << "]: " << X << " -> "
             << (production = M[{X, a}]) << endl;

        string regla = X + " -> " + production;

        parse << producciones[regla] << ", " << endl;

        pila.pop();

        istringstream stream(production);
        vector<string> elementos;

        vector<string> v;

        while (stream >> production) {
          v.push_back(production);
        }

        for (int i = v.size() - 1; i >= 0; i--) {
          if(v[i] != "lambda")
            pila.push(v[i]);
        }
        debug(production);
      }
      X = pila.top();
    }
  };
};
string AnalizadorSintactico::siguienteToken() {

  string s = lexico.getToken();
  cout << "token: " << s << endl;
  return s;
}
