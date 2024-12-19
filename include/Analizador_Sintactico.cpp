#include "Analizador_Sintactico.h"
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


void AnalizadorSintactico::error(string unexpected) {
    cout << "Error en linea  "<< lexico.lineas << " -> no se esperaba: " << unexpected << endl;
    throw std::runtime_error("Error de Sintaxis");
}

AnalizadorSintactico::AnalizadorSintactico(AnalizadorLexico &lexico,
                                           GestorErrores &errores)
: lexico(lexico), errores(errores) {
    string a = "";
    parse.open("parse.txt", fstream::out);
    parse << "D";
    //while ((a = siguienteToken()) != "EOF") {}
    while ((a = siguienteToken())  != "EOF" && a != "$" ) {
        pila.push("$");
        pila.push("Z"); // axioma
        auto X = pila.top();
        while (X != "$") {
            if(token_char.count(a))a=token_char[a];
            if(token_char.count(X))X=token_char[X];
            cout << "element: " << X << ' ' << a << endl;
            if(a == "EOF") a= "$";
            if (X == a) {
                pila.pop();
                cout << "top" << pila.top() << endl;
                a = siguienteToken();
            } else if (terminales.count(X))
                error(X);
            else if (!M.count({X, a}))
                error(a);
            else if (M.count({X, a})) {
                cout << "esta!" << endl;
                string production = "";

                cout << "Produccion: [" << X << ", " << a << "]: " << X
                    << " -> " << (production = M[{X, a}]) << endl;


                int a = 23;
                string regla = X + " -> " + production;

                cerr << "Regla: " << regla << endl;
                parse << " " << producciones[regla];
                cerr << "contiene? " << producciones.count(regla) << " " << producciones[regla] << endl;
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
