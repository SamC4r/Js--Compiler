#include "Analizador_Sintactico.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
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

pair<string,string> token;

string AnalizadorSintactico::buscarTipoTS(int pos){
    cerr << "biuscando" << endl;
    TablaSimbolos* ts_actual = lexico.generator.queue->top();
    debug(token);
    debug(aux.top()->atributos->pos);
    if(ts_actual->posiciones.count(pos)){
        Entry* e = ts_actual->getPos(pos);
        if(e->tipo == ""){
            cerr << "No se ha asignado un tipo a la variable \'" << e->lexema << "\'" << " en la linea: " << lexico.generator.lineas <<  endl;
            return nullptr;
        }
        return e->tipo;
    }
    return nullptr;
}

int AnalizadorSintactico::insertarTipoTS(int pos,string tipo){

    TablaSimbolos* ts_actual = lexico.generator.queue->top();
    if(ts_actual->posiciones.count(pos)){
        Entry* e = ts_actual->getPos(pos);
        e->tipo = tipo;
        return 0;
    }
    return -1;
}

string Error(string msg){
    cerr << msg << endl;
    throw runtime_error(msg);
    return ("tipo_error");
}

void AnalizadorSintactico::ejecutarRegla(string s){
    if(s == "{1.1}"){

    }else if(s == "{x.x}"){ //T -> int {2}  {2}: {T.tipo = ent}
        aux.pop();
        aux.top()->atributos->tipo="entero";
    }
    if(s == "{Z->BZ}"){
        string Z1_tipo = aux.top()->atributos->tipo;
        debug(Z1_tipo);
       

        aux.pop();
        string B_tipo = aux.top()->atributos->tipo;
        debug(B_tipo);
        aux.pop();
        debug(aux.top()->symbol);
        string Z_tipo = "";
        if(B_tipo == "tipo_ok" && (Z1_tipo == "vacio" || Z1_tipo == "tipo_ok")){
            Z_tipo = "tipo_ok";
        }else{
            Z_tipo = "tipo_error";
        }
        aux.top()->atributos->tipo = Z_tipo;
    }
    else if(s == "{M->idV}"){
        string v_tipo = aux.top()->atributos->tipo;
        aux.pop();
        if(v_tipo == "vacio"){
            int id_pos = aux.top()->atributos->pos;
            aux.pop();
            string ret = buscarTipoTS(id_pos);
            aux.top()->atributos->tipo=ret;
        }else{
            //TODO: 
        }
    }else if(s == "{M->constanteEntera}"){
        aux.pop();
        debug(aux.top()->symbol);
        aux.top()->atributos->tipo="entero";
    } else if(s == "{V->lambda}"){
        aux.top()->atributos->tipo="vacio";
    }else if(s == "{B->varTid;}"){
    
        aux.pop();

        int id_pos = aux.top()->atributos->pos;
        aux.pop();
        string T_tipo = aux.top()->atributos->tipo;
        aux.pop(); //quita T
        int ret = insertarTipoTS(id_pos,T_tipo);
        aux.pop(); //quita var
        lexico.generator.zona_declaracion=false;
        aux.top()->atributos->tipo="tipo_ok";
    }else if(s == "{B->S}"){
        string S_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.top()->atributos->tipo=S_tipo; // B.tipo=S.tipo
    }else if(s == "{B->if(E)I}"){
      
        string I_tipo=aux.top()->atributos->tipo;
        string B_tipo=I_tipo;
        aux.pop();
        aux.pop();//quita )
        string E_tipo=aux.top()->atributos->tipo;
        debug(E_tipo,I_tipo);
        if(E_tipo != "logico"){
            cerr << "Error en la linea  "<< lexico.generator.lineas << " - la condicion de un if debe ser de tipo logico" << endl;
            throw runtime_error("Condicion del if incorrecta: solo se pueden comprobar condiciones logicas");
            B_tipo="error";
        }
        aux.pop(); //quita E
        aux.pop(); //quita (
        aux.pop(); //quita if

        aux.top()->atributos->tipo = B_tipo;
        debug(aux.top()->atributos->tipo);
        exit(0);
    } else if(s == "{dec_true}"){
        lexico.generator.zona_declaracion=true;
    }else if(s == "{dec_false}"){
        lexico.generator.zona_declaracion=false;
    }else if(s == "{V->lambda}" || s == "{Y->lambda}" || s == "{P->lambda}" || s == "{N->lambda}" || s == "{J->lambda}"){
        aux.top()->atributos->tipo="vacio";
    }else if(s == "{T->boolean}"){
        aux.pop();
        aux.top()->atributos->tipo="logico";
        aux.top()->atributos->ancho=1;
    }else if(s == "{T->int}"){
        aux.pop();
        aux.top()->atributos->tipo="entero";
        aux.top()->atributos->ancho=4;
    }else if(s == "{T->string}"){
        aux.pop();
        aux.top()->atributos->tipo="cadena";
        aux.top()->atributos->ancho=64;
    }else if(s == "{E->RN}"){
       
        string N_tipo=aux.top()->atributos->tipo;
        aux.pop();
        debug(aux.top()->symbol);
        string R_tipo=aux.top()->atributos->tipo;
        aux.pop();
        string E_tipo="";
        debug(N_tipo,R_tipo);
        if(R_tipo == "entero" && N_tipo == "logico"){
            E_tipo="logico";
        }else if (N_tipo == "vacio"){
            E_tipo=R_tipo;
        }else{
            E_tipo = Error("solo se pueden comparar enteros");
        }
       
        aux.top()->atributos->tipo=E_tipo;
    }else if(s == "{R->OP}"){
        string P_tipo=aux.top()->atributos->tipo;
        aux.pop();
        string O_tipo=aux.top()->atributos->tipo;
        aux.pop();

        string R_tipo="";

        if(P_tipo == "vacio"){
            R_tipo=O_tipo;
        }else if(O_tipo == "entero" && P_tipo == "entero"){
            R_tipo="entero";
        }else{
            R_tipo = Error("solo se pueden comparar numeros");
        }
 
        aux.top()->atributos->tipo=R_tipo;
    } else if(s == "{O->MY}"){
        string Y_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string M_tipo = aux.top()->atributos->tipo;
        aux.pop();
        debug(Y_tipo,M_tipo);

        string O_tipo = "";
        if(Y_tipo == "vacio"){
            O_tipo = M_tipo;
        }else if (M_tipo == "entero" && Y_tipo == "entero"){
            O_tipo = "entero";
        }else{
            O_tipo = Error("solo se pueden comparar enteros");
        }
 
        aux.top()->atributos->tipo = O_tipo;
    } else if(s == "{I->S}") {
        string S_tipo=aux.top()->atributos->tipo;
        debug(S_tipo);
        aux.pop();
        aux.top()->atributos->tipo=S_tipo;
    }else if(s == "{S->outputE;}"){
        
        aux.pop(); //quita ;
        string E_tipo=aux.top()->atributos->tipo;
        string S_tipo=(E_tipo == "tipo_error"? "tipo_error":"tipo_ok");
      
        aux.pop(); //quita E
        aux.pop();          //quita output
        aux.top()->atributos->tipo=S_tipo;
    }else if(s == "{S->inputD;}"){
        aux.pop();
        string D_tipo=aux.top()->atributos->tipo;
        string S_tipo=(D_tipo == "tipo_error"? "tipo_error":"tipo_ok");
        aux.pop();
        aux.pop();
    }else if(s == "{S->returnX;}"){
         aux.pop();
        string X_tipo=aux.top()->atributos->tipo;
        string S_tipo=(X_tipo == "tipo_error"? "tipo_error":"tipo_ok");
        aux.pop();
        aux.pop();
    }else if(s == "{S->idU}"){
        string U_tipo=aux.top()->atributos->tipo;
        string S_tipo;
        aux.pop();
        int id_pos = aux.top()->atributos->pos;

        if(buscarTipoTS(id_pos) == U_tipo || U_tipo == "vacio"){
            S_tipo=U_tipo;
        }else{
            string msg = ("la variable no es del tipo "+U_tipo);
            S_tipo=Error(msg);
        }
    }
}

void AnalizadorSintactico::error(string unexpected) {
    cerr << "Error en la linea  "<< lexico.lineas << " - no se esperaba recibir: " << unexpected << endl;
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
            if(token_char.count(a))a=token_char[a];
            if(token_char.count(X->symbol))X->symbol=token_char[X->symbol];
            // cout << "element: " << X->symbol << ' ' << a << endl;
            // debug(X->symbol);
            if(a == "EOF"){
                a= "$";
            }else if(esAccionSemantica(X->symbol)){
                 cout << "YEEEE " << X->symbol << endl;
                ejecutarRegla(X->symbol);
                debug(aux.top()->symbol);
                debug(aux.top()->atributos->tipo);
                pila.pop();
            }
            else if (X->symbol == a) {
                pila.pop();
                // cout << "top" << pila.top() << endl;

                //Meter X y sus atributos en AUX
                if(token.second != "")
                    X->atributos->pos = stoi(token.second); //add attribute
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
                    if(v[i] != "lambda"){
                        pila.push(new Simbolo(v[i]));
                    }
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
    auto s = lexico.getToken();
    token=s;
    cout << "token: " << s.first << endl;
    cout << "atrib" << s.second << endl;
    return s.first;
}
