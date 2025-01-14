#include "analizador_lexico.h"
#include <map>
#include <set>
#include <string>
#include<vector>

using namespace std;

struct Atributo{
    string tipo;
    int ancho;
    int pos;
    string ret;
};

struct Id{
    int pos;
    string tipo;
    int ancho;
};

struct Simbolo{
    string symbol;
    Atributo* atributos;
    vector<Id> ids;
    int linea;

    Simbolo(string s){
        symbol=s;
        atributos=new Atributo();
    }
};

class AnalizadorSintactico {

    AnalizadorLexico &lexico;
    GestorErrores &errores;
    TablaSimbolos* ts_actual;
    fstream parse;

    stack<Simbolo*> pila;
    stack<Simbolo*> aux;

    string buscarTipoTS(int pos, int linea);
    string buscarTipoTSGlobal(int pos, int linea);
    bool esFuncion(int pos);
    int insertarTipoTS(int pos,string tipo,int ancho);
    int insertarTipoTSGlobal(int pos,string tipo,int ancho, vector<string>& params,string ret);
    void destruirTS(string name);
    void crearTSLocal();
    void error(string expected, string unexpected);

    string siguienteToken();

    set<string> terminales = {
        "var",   "function", "id",      ";",      ",",
        "if",    "else",     "(",       "{",      "}",
        ")",     "int",      "boolean", "string", "void",
        ">",     "-",        "%",       "!",      "return",
        "input", "output",   "=",       "--",     "constanteEntera",
        "cadena", "lambda"
    };
    set<string> noTerminales = { "Z", "B", "T", "E", "N", "R", "P", "O", "Y", "M", "V", "L", "Q", "I", "J", "C", "S", "U", "D", "X", "F", "H", "A", "K" };

    map<string,vector<string>> first = {
        {"V",{"(",")",":","%",">",",","-"}},
        {"Z", {"id","var","function","input","output","return","if","$"}},
        {"B", {"id","input","output","return","var","if"}},
        {"T", {"boolean","int","string"}},
        {"E", {"id","cadena","constanteEntera","(","!","--","-"}},
        {"N", {")",";",">",","}},
        {"R", {"id","cadena","constanteEntera","(","!","--","-"}},
        {"P", {")",";","-",">",","}},
        {"O", {"id","cadena","constanteEntera","(","!","--","-"}},
        {"Y", {")",";","-","%",">",","}},
        {"M", {"id","cadena","constanteEntera","(","!","--","-"}},
        {"L", {"id","cadena","constanteEntera","(","!", "--",")","-"}},
        {"Q", {")",","}},
        {"I", {"id","input","output","return","{"}},
        {"J", {"else","var","$","id","function","input","output","return","if"}},
        {"C", {"id","var","input","output","return","if","}"}},
        {"S", {"id","input","output","return"}},
        {"U", {"(","="}},
        {"D", {"id","("}},
        {"X", {"id","cadena","constanteEntera","(",";","!","--","-"}},
        {"F", {"function"}},
        {"H", {"boolean","int","string","void"}},
        {"A", {"boolean","int","string","void"}},
        {"K", {")",","}}
    };

    std::map<std::string, std::string> token_char = {
        {"operadorAsignacion", "="},
        {"coma", ","},
        {"puntoComa", ";"},
        {"parentesisIzda", "("},
        {"parentesisDcha", ")"},
        {"llavesIzda", "{"},
        {"llavesDcha", "}"},
        {"operadorMenos", "-"},
        {"operadorModulo", "%"},
        {"operadorMayor", ">"},
        {"operadorNegacion", "!"},
    };                          

       map<string, int> producciones = {
        {"Z -> B Z", 1},
        {"Z -> F Z", 2},
        {"Z -> lambda", 3},

        {"B -> var T id ;", 4},
        {"B -> if ( E ) I", 5},
        {"B -> S", 6},

        {"T -> int", 7},
        {"T -> boolean", 8},
        {"T -> string", 9},

        {"E -> R N", 10},

        {"N -> > R N", 11},
        {"N -> lambda", 12},

        {"R -> O P", 13},

        {"P -> - O P", 14},
        {"P -> lambda", 15},

        {"O -> M Y", 16},

        {"Y -> % M Y", 17},
        {"Y -> lambda", 18},

        {"M -> - M", 19},
        {"M -> ! M", 20},
        {"M -> id V", 21},
        {"M -> ( E )", 22},
        {"M -> constanteEntera", 23},
        {"M -> cadena", 24},
        {"M -> -- id", 25},

        {"V -> ( L )", 26},
        {"V -> lambda", 27},

        {"L -> E Q", 28},
        {"L -> lambda", 29},

        {"Q -> , E Q", 30},
        {"Q -> lambda", 31},

        {"I -> S", 32},
        {"I -> { C } J", 33},

        {"J -> else { C }", 34},
        {"J -> lambda", 35},

        {"C -> B C", 36},
        {"C -> lambda", 37},

        {"S -> id U", 38},
        {"S -> output E ;", 39},
        {"S -> input D ;", 40},
        {"S -> return X ;", 41},

        {"U -> = E ;", 42},
        {"U -> ( L ) ;", 43},

        {"D -> id", 44},
        {"D -> ( id )", 45},

        {"X -> E", 46},
        {"X -> lambda", 47},

        {"F -> function H id ( A ) { C }", 48},

        {"H -> T", 49},
        {"H -> void", 50},

        {"A -> T id K", 51},
        {"A -> void", 52},

        {"K -> , T id K", 53},
        {"K -> lambda", 54}
    };

    map<string,string> numero_accion_semantica = {
        {"{xx.yy}","{if E.tipo=logico}"},
        {"{xx.yy}","W -> {ts* =new TablaSimbolos(); zona_declaracion=true, dl* = new DesplazamientoGlobal} Z {destruirTS()}"},
        {"{Z->BZ}","Z -> {zona_declaracion=true} B Z1     {Z.tipo = if (B.tipo=tipo_ok AND (Z1.tipo = vacio OR Z1.tipo = tipo_ok)) then tipo_ok else tipo_error }"},
        {"{Z->FZ}","Z -> F Z1   {Z.tipo = if (F.tipo=tipo_ok AND (Z1.tipo = vacio OR Z1.tipo = tipo_ok) then tipo_ok else tipo_error}"},
        {"{Z->lambda}","Z -> lambda {Z.tipo=vacio}"},
        {"{B->varTid;}","B -> {zona_dec=true} var T id ; {InsertaTipo(id.pos,T.tipo), InsertaDesp(ts,*dl + T.ancho), B.tipo=tipo_ok,zona_dec=false}"},
        {"{;}",""},
        {"{dec_true}","B -> {zona_dec=true} var T id ; {InsertaTipo(id.pos,T.tipo), InsertaDesp(ts,*dl + T.ancho), B.tipo=tipo_ok}"},
        {"{B->if(E)I}","B -> if ( E ) I  {B.tipo=if(E.tipo!=logico then error(\"Condcion del if incorrecta: solo se pueden comprobar condiciones logicas\")) else I.tipo}"},
        {"{dec_false}","declaracion=false"},
        {"{dec_true}","declaracion=true"},
        {"{B->S}","B -> S {B.tipo=S.tipo}"},
        {"{T->int}","T -> int     {T.tipo=entero, T.ancho=4}"},
        {"{T->boolean}","T -> boolean    {T.tipo=logico, T.ancho = 1}"},
        {"{T->string}","T -> string  {T.tipo=cadena, T.ancho=64}"},
        {"{E->RN}","E -> R N  {E.tipo = if (R.tipo = entero AND N.tipo=logico) then logico else if (N.tipo = vacio) then R.tipo else error (“solo se pueden comparar enteros”)}"},
        {"{N->>RN}","N -> > R N1   {N.tipo = if ( R.tipo = entero AND (N1.tipo=logico OR N1.tipo = vacio)) then logico else error(“solo se pueden comparar enteros”)}"},
        {"{N->lambda}","N -> lambda  {N.tipo=vacio}"},
        {"{R->OP}","R -> O P {R.tipo = if (P.tipo = vacio) then O.tipo else if (O.tipo = P.tipo = entero) then entero else error (“Solo se pueden comparar numeros”)"},
        {"{P->-OP}","P -> - O P1   {P.tipo = if ((P1.tipo = vacio AND O.tipo=entero) OR O.tipo = P1.tipo = entero) then entero else error(“solo se puede restar entre numeros enteros”)"},
        {"{P->lambda}","P -> lambda  {P.tipo=vacio}"},
        {"{O->MY}","O ->  M Y  {O.tipo = if (Y.tipo = vacio then M.tipo else if M.tipo = Y.tipo = entero) then entero else error(“solo se puede calcular el resto entre numeros enteros”)"},
        {"{Y->%MY}","Y -> % M Y1   { Y.tipo = if (M.tipo = Y1.tipo = entero) then entero else error(“solo se puede calcular el resto entre numeros enteros”) }"},
        {"{Y->lambda}","Y -> lambda  {Y.tipo=vacio}"},
        {"{M->-M1}","M -> - M1     {M.tipo = if (M1.tipo != entero) then error(“solo se puede hacer aritmetica sobre numeros”) else entero}"},
        {"{M->!M1}","M -> ! M1     {M.tipo = if(M1.tipo != logico) then error(“Solo se pueden negar tipos logicos”) else logico}"},
        {"{M->idV}","M -> id V      {M.tipo = if (V.tipo = vacio then BuscaTipo(id.pos)) else if BuscaTipoTS(id.pos) != V.tipo->T then error(“funcion llamada con parametros incorrectos, esperados %d” else BuscaTipoTS(id.pos)) "},
        {"{M->(E)}","M -> ( E )   {M.tipo=E.tipo}"},
        {"{M->constanteEntera}","M -> constanteEntera   {M.tipo=entero}"},
        {"{M->cadena}","M -> cadena  {M.tipo=cadena}"},
        {"{M->--id}","M -> -- id {M.tipo = if (BuscaTipo(id.pos) != entero) then error(“solo se pueden predecrementar numeros”) else entero}"},
        {"{V->(L)}","V -> ( L )  {V.tipo=L.tipo}   "},
        {"{V->lambda}","V -> lambda  {V.tipo=vacio}"},
        {"{L->EQ}","L -> E Q  {L.tipo = if (Q.tipo != vacio) then E.tipo x K.tipo else E.tipo }"},
        {"{L->lambda}","L -> lambda  {L.tipo=vacio}"},
        {"{Q->,EQ}","Q -> , E Q1    {Q.tipo = if (Q1.tipo != vacio) then E.tipo x Q1.tipo else E.tipo}"},
        {"{Q->lambda}","Q -> lambda  {Q.tipo=vacio}"},
        {"{I->S}","I -> S {I.tipo = S.tipo}"},
        {"{I->{C}J}","I -> { C } J  {I.tipo = if (C.tipo = ok AND J.tipo != vacio) then J.tipo else C.tipo}"},
        {"{J->else{C}}","J -> else { C }    {J.tipo = C.tipo}"},
        {"{J->lambda}","J -> lambda  {J.tipo=vacio}"},
        {"{C->BC1}","C -> B C1    {C.tipo = if (B.tipo = tipo_ok AND (C1.tipo = tipo_ok OR C1.tipo=vacio)) then tipo_ok else error()}"},
        {"{C->lambda}","C -> lambda  {C.tipo=vacio}"},
        {"{S->idU}","S ->  id U   {if (U.tipo=vacio OR BuscaTipoTS(id.pos) = U.tipo->T) then tipo_ok else error(“Error! Se ha asignado un tipo incorrecto a la variable”)"},
        {"{S->outputE;}","S -> output E ;   {S.tipo = tipo_ok if E.tipo != tipo_error}"},
        {"{S->inputD;}","S -> input D ;   {S.tipo = tipo_ok if D.tipo != tipo_error}"},
        {"{S->returnX;}","S -> return X ;  {S.tipo = tipo_ok if X.tipo != tipo_error}"},
        {"{U->=E}","U -> = E ;  {U.tipo = E.tipo}"},
        {"{U->(L)}","U -> ( L ) ; {U.tipo = L.tipo}  "},
        {"{D->id}","D -> id    {D.tipo=BuscaTipo(id.pos)}"},
        {"{D->(id)}","D -> ( id )  {D.tipo=BuscaTipo(id.pos)}"},
        {"{X->E}","X -> E    {X.tipo = E.tipo}"},
        {"{X->lambda}","X -> lambda  {X.tipo=vacio}"},
        {"{xx.yy}","// TS global esta en memoria. Se accede mediante pointer para InsertarTipoTSGlobal. Asi  //no hay que crear pilas"},
        {"{crearTSLocal}","F -> function H {ts = crearTSLocal(), dl = creaDespLocal() despLocal=0, zona_decl=true}  id ( A {zona_dec=false}) {InsertarTipoTSGlobal(id.pos,A.tipo -> H.tipo} { C }  {F.tipo=C.tipo,liberarTabla(ts)}"},
        {"{dec_false}","id ( A {zona_dec=false}) {InsertarTipoTSGlobal(id.pos,A.tipo -> H.tipo} { C }  {F.tipo=C.tipo,liberarTabla(ts)}"},
        {"{InsertarTipoTSGlobal}","{InsertarTipoTSGlobal(id.pos,A.tipo -> H.tipo} { C }  {F.tipo=C.tipo,liberarTabla(ts)}"},
        {"{F.tipo}","F -> function H {ts = crearTSLocal(), dl = creaDespLocal() despLocal=0, zona_decl=true}  id ( A {zona_dec=false}) {InsertarTipoTSGlobal(id.pos,A.tipo -> H.tipo} { C }  {F.tipo=C.tipo,liberarTabla(ts)}"},
        {"{H->T}","H -> T     {H.tipo=T.tipo}"},
        {"{H->void}","H -> void   {H.tipo=vacio}"},
        {"{A->TidK}","A -> T id K    {A.tipo = if (K.tipo != vacio) then T.tipo x K.tipo else T.tipo }"},
        {"{A->void}","A -> void   {A.tipo=vacio}"},
        {"{K->,TidK1}","K ->  , T id K1 {K.tipo = if (K1.tipo != vacio) then T.tipo x K1.tipo else T.tipo}"},
        {"{K->lambda}","K -> lambda   {K.tipo=vacio}"},
        {"{no_desp}","inhabilitar desplazamiento"}
    };

    map<pair<string, string>, string> M = {
        {{"Z", "id"}, "B Z {Z->BZ}"},
        {{"Z", "var"}, "B Z {Z->BZ}"},
        {{"Z", "function"}, "F Z {Z->FZ}"},
        {{"Z", "input"}, "B Z {Z->BZ}"},
        {{"Z", "output"}, "B Z {Z->BZ}"},
        {{"Z", "return"}, "B Z {Z->BZ}"},
        {{"Z", "if"}, "B Z {Z->BZ}"},
        {{"Z", "$"}, "lambda {Z->lambda}"},
        {{"B", "id"}, "S {B->S}"},
        {{"B", "input"}, "S {B->S}"},
        {{"B", "output"}, "S {B->S}"},
        {{"B", "return"}, "S {B->S}"},
        {{"B", "var"}, "{dec_true} var T id {dec_false} {B->varTid;} ; {;}"},
        {{"B", "if"}, "if ( E ) I {B->if(E)I}"},
        {{"T", "boolean"}, "boolean {T->boolean}"},
        {{"T", "int"}, "int {T->int}"},
        {{"T", "string"}, "string {T->string}"},
        {{"E", "id"}, "R N {E->RN}"},
        {{"E", "cadena"}, "R N {E->RN}"},
        {{"E", "constanteEntera"}, "R N {E->RN}"},
        {{"E", "("}, "R N {E->RN}"},
        {{"E", "!"}, "R N {E->RN}"},
        {{"E", "--"}, "R N {E->RN}"},
        {{"N", ")"}, "lambda {N->lambda}"},
        {{"N", ";"}, "lambda {N->lambda}"},
        {{"N", ">"}, "> R N {N->>RN}"},
        {{"R", "id"}, "O P {R->OP}"},
        {{"R", "cadena"}, "O P {R->OP}"},
        {{"R", "constanteEntera"}, "O P {R->OP}"},
        {{"R", "("}, "O P {R->OP}"},
        {{"R", "!"}, "O P {R->OP}"},
        {{"R", "--"}, "O P {R->OP}"},
        {{"P", ")"}, "lambda {P->lambda}"},
        {{"P", ";"}, "lambda {P->lambda}"},
        {{"P", "-"}, "- O P {P->-OP}"},
        {{"P", ">"}, "{P->lambda} lambda"},
        {{"O", "id"}, "M Y {O->MY}"},
        {{"O", "cadena"}, "M Y {O->MY}"},
        {{"O", "constanteEntera"}, "M Y {O->MY}"},
        {{"O", "("}, "M Y {O->MY}"},
        {{"O", "!"}, "M Y {O->MY}"},
        {{"O", "--"}, "M Y {O->MY}"},
        {{"Y", ")"}, "lambda {Y->lambda}"},
        {{"Y", ";"}, "lambda {Y->lambda}"},
        {{"Y", "-"}, "lambda {Y->lambda}"},
        {{"Y", ">"}, "lambda {Y->lambda}"},
        {{"Y", "%"}, "% M Y {Y->%MY}"},
        {{"M", "id"}, "id V {M->idV}"},
        {{"M", "cadena"}, "cadena {M->cadena}"},
        {{"M", "constanteEntera"}, "constanteEntera {M->constanteEntera}"},
        {{"M", "("}, "( E ) {M->(E)}"},
        {{"M", "!"}, "! M {M->!M1}"},
        {{"M", "--"}, "-- id {M->--id}"},
        {{"V", "("}, "( L ) {V->(L)}"},
        {{"V", ")"}, "lambda {V->lambda}"},
        {{"V", ";"}, "lambda {V->lambda}"},
        {{"V", "-"}, "lambda {V->lambda}"},
        {{"V", "%"}, "lambda {V->lambda}"},
        {{"V", ">"}, "lambda {V->lambda}"},
        {{"L", "id"}, "E Q {L->EQ}"},
        {{"L", "cadena"}, "E Q {L->EQ}"},
        {{"L", "constanteEntera"}, "E Q {L->EQ}"},
        {{"L", "("}, "E Q {L->EQ}"},
        {{"L", ")"}, "lambda {L->lambda}"},
        {{"L", "!"}, "E Q {L->EQ}"},
        {{"L", "--"}, "E Q {L->EQ}"},
        {{"Q", ")"}, "lambda {Q->lambda}"},
        {{"Q", ","}, ", E Q {Q->,EQ}"},
        {{"I", "id"}, "S {I->S}"},
        {{"I", "input"}, "S {I->S}"},
        {{"I", "output"}, "S {I->S}"},
        {{"I", "return"}, "S {I->S}"},
        {{"I", "{"}, "{ C } J {I->{C}J}"},
        {{"J", "else"}, "else { C } {J->else{C}}"},
        {{"J", "var"}, "lambda {J->lambda}"},
        {{"J", "$"}, "lambda {J->lambda}"},
        {{"C", "id"}, "B C {C->BC1}"},
        {{"C", "var"}, "B C {C->BC1}"},
        {{"C", "input"}, "B C {C->BC1}"},
        {{"C", "output"}, "B C {C->BC1}"},
        {{"C", "return"}, "B C {C->BC1}"},
        {{"C", "if"}, "B C {C->BC1}"},
        {{"C", "}"}, "lambda {C->lambda}"},
        {{"S", "id"}, "id U {S->idU}"},
        {{"S", "input"}, "input D ; {S->inputD;}"},
        {{"S", "output"}, "output E ; {S->outputE;}"},
        {{"S", "return"}, "return X ; {S->returnX;}"},
        {{"U", "("}, "( L ) ; {U->(L)}"},
        {{"U", "="}, "= E ; {U->=E}"},
        {{"D", "id"}, "id {D->id}"},
        {{"D", "("}, "( id ) {D->(id))}"},
        {{"X", "id"}, "E {X->E}"},
        {{"X", "cadena"}, "E {X->E}"},
        {{"X", "constanteEntera"}, "E {X->E}"},
        {{"X", "("}, "E {X->E}"},
        {{"X", ";"}, "lambda {X->lambda}"},
        {{"X", "!"}, "E {X->E}"},
        {{"X", "--"}, "E {X->E}"},
        {{"F", "function"}, " function {no_desp} H id {dec_true} {no_desp} {crearTSLocal} ( A {dec_false} ) {InsertarTipoTSGlobal} { C } {F.tipo}"},
        {{"H", "boolean"}, "T {H->T}"},
        {{"H", "int"}, "T {H->T}"},
        {{"H", "string"}, "T {H->T}"},
        {{"H", "void"}, "void {H->void}"},
        {{"A", "boolean"}, "T id K {A->TidK}"},
        {{"A", "int"}, "T id K {A->TidK}"},
        {{"A", "string"}, "T id K {A->TidK}"},
        {{"A", "void"}, "void {A->void}"},
        {{"K", ")"}, "lambda {K->lambda}"},
        {{"K", ","}, ", T id K {K->,TidK1}"},
        {{"E", "-"}, "R N {E->RN}"},
        {{"N", ","}, "lambda {N->lambda}"},
        {{"R", "-"}, "O P {R->OP}"},
        {{"P", ","}, "lambda {P->lambda}"},
        {{"O", "-"}, "M Y {O->MY}"},
        {{"Y", ","}, "lambda {Y->lambda}"},
        {{"M", "-"}, "- M {M->-M1}"},
        {{"V", ","}, "lambda {V->lambda}"},
        {{"L", "-"}, "E Q {L->EQ}"},
        {{"J", "id"}, "lambda {J->lambda}"},
        {{"J", "function"}, "lambda {J->lambda}"},
        {{"J", "input"}, "lambda {J->lambda}"},
        {{"J", "output"}, "lambda {J->lambda}"},
        {{"J", "return"}, "lambda {J->lambda}"},
        {{"J", "if"}, "lambda {J->lambda}"},
        {{"X", "-"}, "E {X->E}"}
    };


public:
    AnalizadorSintactico(AnalizadorLexico &lexico, GestorErrores &errores);
    bool esAccionSemantica(string s);
    void ejecutarRegla(string s);
};
