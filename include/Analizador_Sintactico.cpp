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


//Mejora? A la tabla anadirle numero de accion semantica y si es un numero correcto (pq es un map de numero -> accion semantica) aplicar la accion semantica con codigo. Por cada accion semantica se puede saber que simbolo modificar segun las pilas 

bool AnalizadorSintactico::esAccionSemantica(string s){
    return numero_accion_semantica.count(s);
}

void AnalizadorSintactico::ejecutarRegla(string s){
    if(s == "{1.1}"){

    }else if(s == "{x.x}"){ //T -> int {2}  {2}: {T.tipo = ent}
        aux.pop();
        aux.top()->atributos->tipo="entero";
    }
}

void AnalizadorSintactico::error(string unexpected) {
    cout << "Error en la linea  "<< lexico.lineas << " - no se esperaba recibir: " << unexpected << endl;
    throw std::runtime_error("Error de Sintaxis");
}

AnalizadorSintactico::AnalizadorSintactico(AnalizadorLexico &lexico, GestorErrores &errores) : lexico(lexico), errores(errores) {

    string a = "";
    parse.open("parse.txt", fstream::out);
    parse << "D";
    //while ((a = siguienteToken()) != "EOF") {}
    while ((a = siguienteToken())  != "EOF" && a != "$" ) {
        Simbolo* sp = new Simbolo("$");
        pila.push(sp);
        pila.push(new Simbolo("Z")); // axioma
        aux.push(new Simbolo("Z")); // axioma

        auto X = pila.top();
        while (X->symbol != "$") {
            // if(token_char.count(a))a=token_char[a];
            // if(token_char.count(X))X=token_char[X];
            // cout << "element: " << X->symbol << ' ' << a << endl;
            debug(X->symbol);
            if(a == "EOF"){
                a= "$";
            }else if(esAccionSemantica(X->symbol)){
                cout << "YEEEE" << endl;
                ejecutarRegla(X->symbol);
                pila.pop();
            }
            else if (X->symbol == a) {
                pila.pop();
                // cout << "top" << pila.top() << endl;

                //Meter X y sus atributos en AUX
                aux.push(X);
                a = siguienteToken();
            } else if (terminales.count(X->symbol))
                error(X->symbol);
            else if (!M.count({X->symbol, a}))
                error(a);
            else if (M.count({X->symbol, a})) {
                // cout << "esta!" << endl;
                string production = M[{X->symbol, a}];

                // cout << "Produccion: [" << X->symbol << ", " << a << "]: " << X->symbol << " -> " <<  (production = M[{X->symbol, a}])  << endl;

                int a = 23;
                string regla = X->symbol + " -> " + production;

                // cerr << "Regla: " << regla << endl;
                parse << " " << producciones[regla];
                // cerr << "contiene? " << producciones.count(regla) << " " << producciones[regla] << endl;
                pila.pop();

                // Meter X y sus atributos en AUX;
                aux.push(X);

                istringstream stream(production);
                vector<string> elementos;

                vector<string> v;

                while (stream >> production) {
                    v.push_back(production);
                }
                for (int i = v.size() - 1; i >= 0; i--) {
                    if(v[i] != "lambda")
                        pila.push(new Simbolo(v[i]));
                }
            }
            X = pila.top();
        }
    };
};


/**
 * Function to get the next token from the lexical analyzer.
 * @return The next token as a string.
 */
string AnalizadorSintactico::siguienteToken() {
    string s = lexico.getToken();
    // cout << "token: " << s << endl;
    return s;
}
