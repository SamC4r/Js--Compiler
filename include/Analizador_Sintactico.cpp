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

template <typename T, typename V>
void __print(const pair<T, V> &x)
{
    cerr << '{';
    __print(x.first);
    cerr << ',';
    __print(x.second);
    cerr << '}';
}
template <typename T>
void __print(const T &x)
{
    int f = 0;
    cerr << '{';
    for (auto &i : x)
        cerr << (f++ ? "," : ""), __print(i);
    cerr << "}";
}
void _print() { cerr << "]\n"; }
template <typename T, typename... V>
void _print(T t, V... v)
{
    __print(t);
    if (sizeof...(v))
        cerr << ", ";
    _print(v...);
}
#ifndef ONLINE_JUDGE
#define debug(x...)               \
    cerr << "[" << #x << "] = ["; \
    _print(x)
#else
#define debug(x...)
#endif

bool AnalizadorSintactico::esAccionSemantica(string s)
{
    return numero_accion_semantica.count(s);
}

pair<string, string> token;
bool AnalizadorSintactico::esFuncion(int pos)
{
    TablaSimbolos *ts_global = lexico.generator.ts_global;
    if (ts_global->posiciones.count(pos))
    {
        Entry *e = ts_global->getPos(pos);
        return e->tipo == "function";
    }
    return false;
}
string AnalizadorSintactico::buscarTipoTSGlobal(int pos, int linea)
{
    TablaSimbolos *ts_global = lexico.generator.ts_global;
    if (ts_global->posiciones.count(pos))
    {
        Entry *e = ts_global->getPos(pos);
        if (e->tipo == "")
        {
            cerr << "No se ha asignado un tipo a la variable \'" << e->lexema << "\'" << " en la linea: " << to_string(linea) << endl;
            return "";
        }
        else if (e->tipo == "function")
        {
            string tipo_params = "";
            for (int i = 0; i < e->f.n_params; i++)
            {
                if (i != 0)
                    tipo_params += " ";
                tipo_params += e->f.tipo_params[i];
            }
            tipo_params += " " + e->f.ret; // return val
            return tipo_params;
        }
        return e->tipo;
    }
    return "";
}

string AnalizadorSintactico::buscarTipoTS(int pos, int linea)
{
    TablaSimbolos *ts_actual = lexico.generator.queue->top();
    TablaSimbolos *ts_global = lexico.generator.ts_global;
    if (ts_actual->posiciones.count(pos))
    {
        Entry *e = ts_actual->getPos(pos);
        if (e->tipo == "")
        {
            cerr << "No se ha asignado un tipo a la variable \'" << e->lexema << "\'" << " en la linea: " << lexico.generator.prev_lineas << endl;
            return "";
        }
        return e->tipo;
    }
    else
    {
        return buscarTipoTSGlobal(pos, linea);
    }
}

int AnalizadorSintactico::insertarTipoTS(int pos, string tipo, int ancho)
{

    TablaSimbolos *ts_actual = lexico.generator.queue->top();
    if (ts_actual->posiciones.count(pos))
    {
        Entry *e = ts_actual->getPos(pos);
        e->tipo = tipo;
        e->desp = ts_actual->global_desp;
        if (e->tipo != "function")
            ts_actual->global_desp += ancho;
        return 0;
    }
    return -1;
}
int AnalizadorSintactico::insertarTipoTSGlobal(int pos, string tipo, int ancho, vector<string> &params, string ret)
{
    // debug("-------------");
    // debug(pos, tipo);

    TablaSimbolos *ts_global = lexico.generator.ts_global;
    if (ts_global->posiciones.count(pos))
    {
        Entry *e = ts_global->getPos(pos);
        e->tipo = tipo;
        // debug(e->tipo, e->pos);
        if (e->tipo != "function")
            ts_global->global_desp += ancho;
        e->desp = ts_global->global_desp;
        e->f.n_params = 0;
        if (params[0] != "vacio")
        {
            e->f.tipo_params = params;
            e->f.n_params = params.size();
        }
        e->f.ret = ret;
        return 0;
    }
    return -1;
}

void AnalizadorSintactico::crearTSLocal()
{
    TablaSimbolos *ts_local = new TablaSimbolos();
    lexico.generator.queue->add(ts_local);
}

void AnalizadorSintactico::destruirTS(string name)
{
    lexico.generator.queue->print(name);
    lexico.generator.queue->pop();
}

string Error(string msg)
{
    cerr << "[X] Error [X]: " << msg << endl;
    throw runtime_error(msg);
    return ("tipo_error");
}
vector<std::string> split(const string &str, char delimiter)
{
    vector<std::string> result;
    istringstream stream(str);
    string token;

    while (getline(stream, token, delimiter))
    {
        result.push_back(token);
    }

    return result;
}

void AnalizadorSintactico::ejecutarRegla(string s)
{
    if (s == "{1.1}")
    {
    }
    else if (s == "{x.x}")
    { // T -> int {2}  {2}: {T.tipo = ent}
        aux.pop();
        aux.top()->atributos->tipo = "entero";
    }
    if (s == "{Z->BZ}")
    {
        string Z1_tipo = aux.top()->atributos->tipo;
        // debug(Z1_tipo);

        aux.pop();
        string B_tipo = aux.top()->atributos->tipo;
        // debug(B_tipo);
        aux.pop();
        // debug(aux.top()->symbol);
        string Z_tipo = "";
        if (B_tipo == "tipo_ok" && (Z1_tipo == "vacio" || Z1_tipo == "tipo_ok"))
        {
            Z_tipo = "tipo_ok";
        }
        else
        {
            Z_tipo = "tipo_error";
        }
        aux.top()->atributos->tipo = Z_tipo;
    }
    else if (s == "{Z->FZ}")
    {
        string Z1_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string F_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string Z_tipo = "";
        if (F_tipo == "tipo_ok" && (Z1_tipo == "vacio" || Z1_tipo == "tipo_ok"))
        {
            Z_tipo = "tipo_ok";
        }
        else
        {
            Z_tipo = "tipo_error";
        }
        aux.top()->atributos->tipo = Z_tipo;
    }
    else if (s == "{Z->lambda}")
    {
        aux.top()->atributos->tipo = "vacio";
    }
    else if (s == "{M->idV}")
    {
        string v_tipo = aux.top()->atributos->tipo;
        aux.pop();
        // debug(aux.top()->symbol);
        // debug(aux.top()->atributos->pos);
        int id_pos = aux.top()->atributos->pos;
        aux.pop();
        string ret = buscarTipoTS(id_pos, aux.top()->linea);
        string M_tipo = ret;
        int M_linea = aux.top()->linea;
        bool funcion = esFuncion(id_pos);
        if (v_tipo != "vacio")
        {
            if (!funcion)
            {
                string msg = "la variable " + aux.top()->symbol + " en la linea " + to_string(aux.top()->linea) + " no es una funcion";
                M_tipo = Error(msg);
            }
            ret = buscarTipoTSGlobal(id_pos, aux.top()->linea);
            vector<string> params = split(v_tipo, ' ');
            vector<string> args = split(ret, ' ');
            int p = 0;
            int a = 0;
            for (string s : params)
            {
                if (s != "vacio" && s != "")
                    p++;
            }
            for (int i = 0; i + 1 < args.size(); i++)
            {
                string s = args[i];
                if (s != "vacio" && s != "")
                    a++;
            }

            if (p != a)
            {
                string msg = "el numero de parametros de la llamada no coincide con el numero de argumentos esperados por la funcion en la linea: " + to_string(M_linea);
                M_tipo = Error(msg);
            }

            bool same = true;
            for (int i = 0; i < p && same; i++)
            {
                if (params[i] == "vacio")
                    continue;
                if (params[i] != args[i])
                {
                    same = false;
                    break;
                }
            }
            if (same)
            {
                M_tipo = args[params.size()];
            }
            else
            {
                string msg = "Los tipos de los parametros de la llamada a la funcion no coinciden con lo esperado en la linea " + to_string(M_linea);
                M_tipo = Error(msg);
            }
        }
        else if(funcion){
            string msg = "Se ha intentado llamar a una funcion sin parentesis en la linea " + to_string(M_linea);
            M_tipo = Error(msg);
        }
        aux.top()->atributos->tipo = M_tipo;
        // debug(aux.top()->atributos->tipo);
    }
    else if (s == "{M->(E)}")
    {
        aux.pop();
        string E_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.pop();
        string M_tipo = E_tipo;
        aux.top()->atributos->tipo = M_tipo;
    }
    else if (s == "{M->-M1}")
    {
        string M1_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.pop();
        string M_tipo = "";
        if (M1_tipo == "entero")
            M_tipo = "entero";
        else
        {
            string msg = "solo se puede obtener el negativo de numeros enteros \t Error en la linea " + to_string(aux.top()->linea);
            M_tipo = Error(msg);
        }
        aux.top()->atributos->tipo = M_tipo;
    }
    else if (s == "{M->!M1}")
    {
        string M1_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.pop();
        string M_tipo = "";
        if (M1_tipo == "logico")
            M_tipo = "logico";
        else
        {
            string msg = "solo se pueden negar un booleano\t Error en la linea " + to_string(aux.top()->linea);
            M_tipo = Error(msg);
        }
        aux.top()->atributos->tipo = M_tipo;
    }
    else if (s == "{M->constanteEntera}")
    {
        aux.pop();
        aux.top()->atributos->tipo = "entero";
    }
    else if (s == "{M->cadena}")
    {
        aux.pop();
        aux.top()->atributos->tipo = "cadena";
    }
    else if (s == "{M->--id}")
    {
        int id_pos = aux.top()->atributos->pos;
        aux.pop();
        aux.pop();
        string id_tipo = buscarTipoTS(id_pos, aux.top()->linea);
        string M_tipo = "";
        if (id_tipo == "entero")
            M_tipo = "entero";
        else
        {
            string msg = "solo se pueden predecrementar numeros enteros \t Error en la linea " + to_string(aux.top()->linea);
            M_tipo = Error(msg);
        }

        aux.top()->atributos->tipo = M_tipo;
    }
    else if (s == "{V->(L)}")
    {
        aux.pop();
        string L_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.pop();
        string V_tipo = L_tipo;
        aux.top()->atributos->tipo = V_tipo + " ";
    }
    else if (s == "{L->EQ}")
    {
        string Q_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string E_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string L_tipo;
        if (Q_tipo == "vacio")
            L_tipo = E_tipo;
        else
            L_tipo = E_tipo + " " + Q_tipo;
        aux.top()->atributos->tipo = L_tipo;
        // debug(L_tipo);
    }
    else if (s == "{Q->,EQ}")
    {
        string Q1_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string E_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.pop();
        string Q_tipo;
        if (Q1_tipo == "vacio")
            Q_tipo = E_tipo;
        else
            Q_tipo = E_tipo + " " + Q1_tipo;
        aux.top()->atributos->tipo = Q_tipo;
    }
    else if (s == "{B->varTid;}")
    {
        int id_pos = aux.top()->atributos->pos;
        aux.pop();
        // debug("AAAA", id_pos);
        string T_tipo = aux.top()->atributos->tipo;
        int T_ancho = aux.top()->atributos->ancho;
        aux.pop(); // quita T
        int ret = insertarTipoTS(id_pos, T_tipo, T_ancho);
        aux.pop(); // quita var
        lexico.generator.zona_declaracion = false;
        aux.top()->atributos->tipo = "tipo_ok";
    }
    else if (s == "{;}")
    {
        aux.pop();
    }
    else if (s == "{B->S}")
    {
        string S_tipo = aux.top()->atributos->tipo;
        string S_ret = aux.top()->atributos->ret;
        aux.pop();
        aux.top()->atributos->tipo = S_tipo; // B.tipo=S.tipo
        aux.top()->atributos->ret = S_ret;   // B.ret=S.ret
    }
    else if (s == "{B->if(E)I}")
    {

        string I_tipo = aux.top()->atributos->tipo;
        string I_ret = aux.top()->atributos->ret;
        aux.pop();
        string B_tipo = I_tipo;
        string B_ret = I_ret;
        aux.pop(); // quita )
        string E_tipo = aux.top()->atributos->tipo;
        int E_linea = aux.top()->linea;
        // debug(aux.top()->symbol);
        // debug(E_tipo, I_tipo);
        if (E_tipo != "logico")
        {
            cerr << "Error en la linea  " << E_linea << " - la condicion de un if debe ser de tipo logico" << endl;
            throw runtime_error("Condicion del if incorrecta: solo se pueden comprobar condiciones logicas");
            B_tipo = "error";
        }
        aux.pop(); // quita E
        aux.pop(); // quita (
        aux.pop(); // quita if

        aux.top()->atributos->tipo = B_tipo;
        aux.top()->atributos->ret = B_ret;
    }
    else if (s == "{dec_true}")
    {
        lexico.generator.zona_declaracion = true;
    }
    else if (s == "{dec_false}")
    {
        lexico.generator.zona_declaracion = false;
    }
    else if (s == "{V->lambda}" || s == "{Q->lambda}" || s == "{L->lambda}" || s == "{Y->lambda}" || s == "{P->lambda}" || s == "{N->lambda}" || s == "{J->lambda}" || s == "{X->lambda}" || s == "{K->lambda}" || s == "{C->lambda}")
    {
        aux.top()->atributos->tipo = "vacio";
    }
    else if (s == "{T->boolean}")
    {
        aux.pop();
        aux.top()->atributos->tipo = "logico";
        aux.top()->atributos->ancho = 1;
    }
    else if (s == "{T->int}")
    {
        aux.pop();
        aux.top()->atributos->tipo = "entero";
        aux.top()->atributos->ancho = 1;
    }
    else if (s == "{T->string}")
    {
        aux.pop();
        aux.top()->atributos->tipo = "cadena";
        aux.top()->atributos->ancho = 64;
    }
    else if (s == "{E->RN}")
    {

        string N_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string R_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string E_tipo = "";
        if (R_tipo == "entero" && N_tipo == "logico")
        {
            E_tipo = "logico";
        }
        else if (N_tipo == "vacio")
        {
            E_tipo = R_tipo;
        }
        else
        {
            string msg = "solo se pueden predecrementar numeros enteros \t Error en la linea " + to_string(aux.top()->linea);
            E_tipo = Error(msg);
        }
        aux.top()->atributos->tipo = E_tipo;
    }
    else if (s == "{N->>RN}")
    {
        string N1_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string R_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.pop(); //>
        string N_tipo = "";
        if (R_tipo == "entero" && (N1_tipo == "vacio" || N1_tipo == "logico"))
            N_tipo = "logico";
        else
            N_tipo = Error("solo se pueden comparar enteros");
        aux.top()->atributos->tipo = N_tipo;
    }
    else if (s == "{R->OP}")
    {
        string P_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string O_tipo = aux.top()->atributos->tipo;
        aux.pop();

        string R_tipo = "";

        if (P_tipo == "vacio")
        {
            R_tipo = O_tipo;
        }
        else if (O_tipo == "entero" && P_tipo == "entero")
        {
            R_tipo = "entero";
        }
        else
        {
            R_tipo = Error("solo se pueden operar con numeros");
        }

        aux.top()->atributos->tipo = R_tipo;
    }
    else if (s == "{P->-OP}")
    {
        string P1_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string O_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.pop();
        string P_tipo = "";
        if ((P1_tipo == "vacio" || P1_tipo == "entero") && O_tipo == "entero")
            P_tipo = "entero";
        else
            P_tipo = Error("Solo se puede restar entre numeros enteros");
        aux.top()->atributos->tipo = P_tipo;
    }
    else if (s == "{O->MY}")
    {
        string Y_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string M_tipo = aux.top()->atributos->tipo;
        aux.pop();
        // debug(Y_tipo,M_tipo);

        string O_tipo = "";
        if (Y_tipo == "vacio")
        {
            O_tipo = M_tipo;
        }
        else if (M_tipo == "entero" && Y_tipo == "entero")
        {
            O_tipo = "entero";
        }
        else
        {
            O_tipo = Error("solo se pueden operar enteros");
        }

        aux.top()->atributos->tipo = O_tipo;
    }
    else if (s == "{Y->%MY}")
    {
        string Y1_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string M_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.pop();
        string Y_tipo = "";
        if ((Y1_tipo == "vacio" || Y1_tipo == "entero") && M_tipo == "entero")
            Y_tipo = "entero";
        else
            Y_tipo = Error("solo se puede hacer modulo entre numeros enteros");
        aux.top()->atributos->tipo = Y_tipo;
    }
    else if (s == "{I->S}")
    {
        string S_tipo = aux.top()->atributos->tipo;
        string S_ret = aux.top()->atributos->ret;
        aux.pop();
        aux.top()->atributos->tipo = S_tipo;
        aux.top()->atributos->ret = S_ret;
    }
    else if (s == "{I->{C}J}")
    {
        string J_tipo = aux.top()->atributos->tipo;
        string J_ret = aux.top()->atributos->ret;
        aux.pop();
        aux.pop();
        string C_tipo = aux.top()->atributos->tipo;
        string C_ret = aux.top()->atributos->ret;
        aux.pop();
        aux.pop();
        int I_linea = aux.top()->linea;
        string I_tipo;
        string I_ret;
        if ((J_tipo == "tipo_ok" || J_tipo == "vacio") && (C_tipo == "tipo_ok" || C_tipo == "vacio"))
        {
            I_tipo = "tipo_ok";
        }
        else
            I_tipo = "tipo_error";
        if (C_ret == "")
            I_ret = J_ret;
        else if (J_ret == "")
            I_ret = C_ret;
        else if (C_ret == J_ret)
            I_ret = C_ret;
        else
        {
            string msg = "Returns de if y else distintos a partir de la linea " + to_string(I_linea);
            I_tipo = Error(msg);
            I_ret = "tipo_error";
        }
        aux.top()->atributos->tipo = I_tipo;
        aux.top()->atributos->ret = I_ret;
        // debug("_------------------");
        // debug(I_tipo);
    }
    else if (s == "{J->else{C}}")
    {
        aux.pop();
        string C_tipo = aux.top()->atributos->tipo;
        string C_ret = aux.top()->atributos->ret;
        aux.pop();
        aux.pop();
        aux.pop();
        aux.top()->atributos->tipo = C_tipo;
        aux.top()->atributos->ret = C_ret;
    }
    else if (s == "{C->BC1}")
    {
        string C1_tipo = aux.top()->atributos->tipo;
        string C1_ret = aux.top()->atributos->ret;
        int C1_linea = aux.top()->linea;
        aux.pop();
        string B_tipo = aux.top()->atributos->tipo;
        string B_ret = aux.top()->atributos->ret;

        // debug(B_tipo,C1_tipo);
        aux.pop();
        int C_linea = aux.top()->linea;
        string C_tipo = "";
        if (B_tipo == "tipo_ok" && (C1_tipo == "vacio" || C1_tipo == "tipo_ok"))
        {
            C_tipo = "tipo_ok";
        }
        else
        {
            C_tipo = "tipo_error";
        }
        string ret = B_ret;
        if (ret != "" && C1_ret != ret && C1_ret != "")
        {
            string msg = "returns de tipos distintos en el bloque que empieza en la linea " + to_string(C_linea);
            C_tipo = Error(msg);
        }
        if (ret == "")
            ret = C1_ret;
        if (ret == "tipo_error")
            C_tipo = "tipo_error";
        // if(ret == "")ret=Error("funcion no tiene un valor de retorno");
        aux.top()->atributos->tipo = C_tipo;
        aux.top()->atributos->ret = ret;
    }
    else if (s == "{S->outputE;}")
    {

        aux.pop(); // quita ;
        string E_tipo = aux.top()->atributos->tipo;
        string S_tipo = (E_tipo == "tipo_error" ? "tipo_error" : "tipo_ok");

        aux.pop(); // quita E
        aux.pop(); // quita output
        aux.top()->atributos->tipo = S_tipo;
    }
    else if (s == "{S->inputD;}")
    {
        aux.pop();
        string D_tipo = aux.top()->atributos->tipo;
        string S_tipo = (D_tipo == "tipo_error" ? "tipo_error" : "tipo_ok");
        aux.pop();
        aux.pop();
        aux.top()->atributos->tipo = S_tipo;
    }
    else if (s == "{S->returnX;}")
    {
        aux.pop(); //;
        string X_tipo = aux.top()->atributos->tipo;
        aux.pop(); // X
        int return_linea = aux.top()->linea;
        aux.pop(); // return
        string S_tipo = (X_tipo == "tipo_error" ? "tipo_error" : "tipo_ok");
        string S_return = X_tipo;
        if (lexico.generator.queue->size() != 2)
        {
            string msg = "return fuera de funcion en linea " + to_string(return_linea);
            S_tipo = Error(msg);
        }
        aux.top()->atributos->tipo = S_tipo;
        aux.top()->atributos->ret = S_return;
    }
    else if (s == "{S->idU}")
    {
        string U_tipo = aux.top()->atributos->tipo;
        int U_linea = aux.top()->linea;
        aux.pop();

        int id_pos = aux.top()->atributos->pos;
        aux.pop();
        // debug(id_pos);
        string S_tipo;
        int S_linea = aux.top()->linea;
        string local_tipo = buscarTipoTS(id_pos, S_linea);
        bool funcion = esFuncion(id_pos);
        string global_tipo = buscarTipoTSGlobal(id_pos, S_linea);
        vector<string> params = split(U_tipo, ' ');

        // debug(funcion, local_tipo,U_tipo,id_pos,global_tipo);

        if (local_tipo == U_tipo){
            S_tipo = "tipo_ok";
        }
        else if (funcion) //es funcion 
        {
            
            if(U_tipo.back() != ' '){//se intenta usar una funcion como una variable
                string msg="no se puede asignar un valor a una funcion. Error en la linea "+to_string(U_linea);
                S_tipo=Error(msg);
            }
            //es funcion y se usa como funcion
            vector<string> args = split(global_tipo, ' ');
            int p = 0;
            int a = 0;
            for (string s : params)
            {
                if (s != "vacio" && s != "")
                    p++;
            }
            for (int i = 0; i + 1 < args.size(); i++)
            {
                string s = args[i];
                if (s != "vacio" && s != "")
                    a++;
            }

            if (p != a)
            {
                string msg = "el numero de parametros de la llamada no coincide con el numero de argumentos esperados por la funcion en la linea: " + to_string(U_linea);
                S_tipo = Error(msg);
            }
            bool same = true;
            for (int i = 0; i < params.size() && same; i++)
            {
                if (params[i] == "vacio")
                    continue;
                if (params[i] != args[i])
                {
                    same = false;
                }
            }
            if (same)
            {
                S_tipo = "tipo_ok";
            }
            else
            {
                string msg = "Los tipos de los parametros de la llamada a la funcion no coinciden con lo esperado en la linea " + to_string(U_linea);
                S_tipo = Error(msg);
            }
        }
        else if (U_tipo.back() == ' ' || params.size() > 1){//no es funcion pero se intenta llamar a una funcion
            string msg = "\'" + lexico.generator.queue->top()->getPos(id_pos)->lexema + "\' no es una funcion en la linea " + to_string(U_linea);
            S_tipo = Error(msg);
        }
        else if (local_tipo != U_tipo) { //los tipos no coinciden 
            string msg = ("la variable no es del tipo " + U_tipo + " en linea " + to_string(S_linea));
           
            S_tipo = Error(msg);
        }
        aux.top()->atributos->tipo = S_tipo;
    }
    else if (s == "{U->=E}")
    {
        aux.pop(); //;
        string E_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string U_tipo = E_tipo;
        aux.pop(); //=
        aux.top()->atributos->tipo = U_tipo;
    }
    else if (s == "{U->(L)}") {
        aux.pop(); //;
        aux.pop(); //)
        string L_tipo = aux.top()->atributos->tipo;
        aux.pop(); // L
        string U_tipo = L_tipo;
        aux.pop(); //(
        aux.top()->atributos->tipo = U_tipo + " ";
    }
    else if (s == "{D->id}")
    {
        int id_pos = aux.top()->atributos->pos;
        aux.pop();
        int D_linea = aux.top()->linea;
        string id_tipo = buscarTipoTS(id_pos, D_linea);
        string D_tipo = id_tipo;
        aux.top()->atributos->tipo = D_tipo;
    }
    else if (s == "{D->(id)}")
    {
        aux.pop(); //)
        int id_pos = aux.top()->atributos->pos;
        aux.pop(); // id
        aux.pop(); //(
        int D_linea = aux.top()->linea;
        string id_tipo = buscarTipoTS(id_pos, D_linea);

        string D_tipo = id_tipo;
        aux.top()->atributos->tipo = D_tipo;
    }
    else if (s == "{X->E}")
    {
        string E_tipo = aux.top()->atributos->tipo;
        // debug("X");
        // debug(aux.top()->symbol);
        aux.pop();
        string X_tipo = E_tipo;
        aux.top()->atributos->tipo = X_tipo;
    }
    else if (s == "{crearTSLocal}")
    {
        crearTSLocal();
    }
    else if (s == "{InsertarTipoTSGlobal}")
    {
        aux.pop();
        string A_tipo = aux.top()->atributos->tipo;
        // debug(A_tipo); // [tipo1 tipo2 tipo3] --es string
        aux.pop();
        aux.pop();
        // debug(aux.top()->symbol);
        int id_pos = aux.top()->atributos->pos;
        // debug(id_pos,A_tipo);
        aux.pop();
        string H_tipo = aux.top()->atributos->tipo;
        aux.pop();
        aux.pop();
        string F_tipo = "function";
        vector<string> params = split(A_tipo, ' ');
        // debug(params);
        // debug("--------------------------------");
        // lexico.generator.ts_global->print();
        insertarTipoTSGlobal(id_pos, F_tipo, 0, params, H_tipo);
        aux.top()->atributos->ret = H_tipo;
    }
    else if (s == "{ss}")
    {
        aux.pop();
    }
    else if (s == "{F.tipo}")
    {
        aux.pop();
        string C_tipo = aux.top()->atributos->tipo;
        string C_ret = aux.top()->atributos->ret;
        // debug(aux.top()->symbol);
        // debug(aux.top()->atributos->tipo);
        aux.pop();
        aux.pop();
        // debug(aux.top()->symbol);
        string F_devuelto = aux.top()->atributos->ret;
        int F_linea = aux.top()->linea;
        // debug(F_devuelto);
        string F_tipo = C_tipo;
        // debug(C_ret);
        if (F_devuelto != C_ret && C_ret != "")
        {
            string msg = ("el tipo de retorno de la funcion no es del mismo tipo que la declaracion de la funcion \t Error en la linea " + to_string(F_linea));
            F_tipo = Error(msg);
        }
        aux.top()->atributos->tipo = F_tipo;
        destruirTS("funcion local");
    }
    else if (s == "{H->T}")
    {
        string T_tipo = aux.top()->atributos->tipo;
        aux.pop();
        string H_tipo = T_tipo;
        aux.top()->atributos->tipo = H_tipo;
    }
    else if (s == "{H->void}")
    {
        aux.pop();
        string H_tipo = "vacio";
        aux.top()->atributos->tipo = H_tipo;
    }
    else if (s == "{A->TidK}")
    {
        string K_tipo = aux.top()->atributos->tipo;
        vector<Id> ids = aux.top()->ids;
        aux.pop();
        int id_pos = aux.top()->atributos->pos;
        aux.pop();
        string T_tipo = aux.top()->atributos->tipo;
        int T_ancho = aux.top()->atributos->ancho;
        aux.pop();
        insertarTipoTS(id_pos, T_tipo, T_ancho);
        string A_tipo;
        if (K_tipo == "vacio")
            A_tipo = T_tipo;
        else
            A_tipo = T_tipo + " " + K_tipo;
        for (auto x : ids)
        {
            insertarTipoTS(x.pos, x.tipo, x.ancho);
        }
        aux.top()->atributos->tipo = A_tipo;
    }
    else if (s == "{A->void}")
    {
        aux.pop();
        string A_tipo = "vacio";
        aux.top()->atributos->tipo = A_tipo;
    }
    else if (s == "{K->,TidK1}")
    {
        string K1_tipo = aux.top()->atributos->tipo;
        vector<Id> ids = aux.top()->ids;
        aux.pop();
        // string K1_tipo = aux.top()->atributos->tipo;
        // vector<Id> ids = aux.top()->ids;
        // aux.pop();
        int id_pos = aux.top()->atributos->pos;
        aux.pop();
        string T_tipo = aux.top()->atributos->tipo;
        int T_ancho = aux.top()->atributos->ancho;
        aux.pop();
        aux.pop();
        string K_tipo;
        if (K1_tipo == "vacio")
        {
            K_tipo = T_tipo;
        }
        else
        {
            K_tipo = T_tipo + " " + K1_tipo;
        }
        aux.top()->ids = ids;
        aux.top()->ids.insert(aux.top()->ids.begin(), {id_pos, T_tipo, T_ancho});
        aux.top()->atributos->tipo = K_tipo;
    }
    else if (s == "{no_desp}")
    {
        lexico.generator.function = !lexico.generator.function;
    }
}

void AnalizadorSintactico::error(string expected, string unexpected)
{
    if (terminales.count(expected))
    {
        cerr << "Error en la linea  " << lexico.generator.prev_lineas << " - se esperaba recibir: \'" << (expected) << "\' Pero se obtuvo: \'" << unexpected << "\'" << endl;
    }
    else
    {
        vector<string> first_follow = first[expected];
        cerr << "Error en la linea  " << lexico.generator.prev_lineas << " - se esperaba recibir alguno de: [";
        bool first = true;
        for (string s : first_follow)
        {
            if (!first)
                cerr << ", ";
            cerr << "\'" << s << "\'";
            first = false;
        }
        cerr << ",] Pero se obtuvo: \'" << unexpected << "\'" << endl;
    }
    throw std::runtime_error("Error de Sintaxis");
}

AnalizadorSintactico::AnalizadorSintactico(AnalizadorLexico &lexico, GestorErrores &errores) : lexico(lexico), errores(errores)
{

    parse.open("parse.txt", fstream::out);
    parse << "D";
    Simbolo *sp = new Simbolo("$");
    pila.push(sp);
    pila.push(new Simbolo("Z")); // axioma

    string a = siguienteToken();
    auto X = pila.top();
    while ((X = pila.top())->symbol != "$")
    {
        X->linea = lexico.generator.lineas;
        // cout << "element " << X->symbol << ' ' << a << endl;
        // cout << "lineas: " << lexico.generator.prev_lineas << endl;
        if (terminales.count(X->symbol) || X->symbol == "$")
        {
            if (X->symbol == a)
            {
                pila.pop();
                // cout << "top" << pila.top()->symbol << endl;
                // Meter X y sus atributos en AUX
                if (token.second != "" && token.first != "cadena")
                    X->atributos->pos = stoi(token.second); // add attribute
                aux.push(X);
                a = siguienteToken();
            }
            else
            {
                error(X->symbol, a);
            }
        }
        else if (noTerminales.count(X->symbol))
        {
            // cout << "esta!" << endl;
            if (M.count({X->symbol, a}))
            {

                string production = M[{X->symbol, a}];

                // cout << "Produccion: [" << X->symbol << ", " << a << "]: " << X->symbol << " -> " <<  (production = M[{X->symbol, a}])  << endl;

                // string regla = X->symbol + " -> " + production;

                // cerr << "Regla: " << regla << endl;
                // parse << " " << producciones[regla];
                // cerr << "contiene? " << producciones.count(regla) << " " << producciones[regla] << endl;
                pila.pop();

                // Meter X y sus atributos en AUX;
                aux.push(X);
                istringstream stream(production);
                vector<string> elementos;

                vector<string> v;

                while (stream >> production)
                {
                    v.push_back(production);
                }
                string rule = "";
                for (int i = v.size() - 1; i >= 0; i--)
                {
                    if (v[i] != "lambda")
                    {
                        pila.push(new Simbolo(v[i]));
                    }
                    if (v[i][0] != '{' || v[i].size() == 1)
                    {
                        rule = v[i] + ' ' + rule;
                    }
                }
                string regla = X->symbol + " -> " + rule;
                // debug(regla);
                regla.pop_back();
                parse << " " << producciones[regla];
                if (producciones[regla] == 0)
                {
                    cout << regla << endl;
                    exit(0);
                }
            }
            else
            {
                error(X->symbol, a);
            }
        }
        else if (esAccionSemantica(X->symbol))
        {
            // cout << "YEEEE " << X->symbol << endl;
            ejecutarRegla(X->symbol);
            pila.pop();
        }
    }
    destruirTS("principal");
};

/**
 * Function to get the next token from the lexical analyzer.
 * @return The next token as a string.
 */
string AnalizadorSintactico::siguienteToken()
{
    auto s = lexico.getToken();
    // debug(s);
    token = s;
    // cout << "token: " << s.first << endl;
    // cout << "atrib" << s.second << endl;
    return s.first;
}
