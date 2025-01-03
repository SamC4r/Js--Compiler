#include "analizador_lexico.h"
#include <map>
#include <set>
#include <string>

using namespace std;

struct Atributo{
    string tipo;
    string ancho;
};

struct Simbolo{
    string symbol;
    Atributo* atributos;

    Simbolo(string s){
        symbol=s;
        atributos=NULL;
    }
};

class AnalizadorSintactico {

    AnalizadorLexico &lexico;
    GestorErrores &errores;
    fstream parse;

    stack<Simbolo*> pila;
    stack<Simbolo*> aux;

    string siguienteToken();

    set<string> terminales = {
        "var",   "function", "id",      ";",      ",",
        "if",    "else",     "(",       "{",      "}",
        ")",     "int",      "boolean", "string", "void",
        ">",     "-",        "%",       "!",      "return",
        "input", "output",   "=",       "--",     "constanteEntera",
        "cadena", "lambda"
    };

    set<string> noTerminales = {"Z", "B", "T", "E", "O", "M", "P", "L", "Q", "I",
        "J", "C", "S", "D", "X", "F", "H", "A", "K"};

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
    map<string,int> pruducciones_semanticas = {
        {"W -> {ts=new TablaSimbolos(); ptrTablaSimbolos=ts; zona_declaracion=true} Z {destruirTS()}",1},

        {"Z -> B Z", 2},
        {"Z -> F Z", 3},
        {"Z -> lambda {Z.tipo=vacio}", 4},

        {"B -> var T id ; {InsertaTipo(id.pos,T.tipo)}", 5},
        {"B -> if ( E ) I {B.tipo=if(E.tipo!=logico then error(\"no es un booleano\")) else if I.tipo!=tipo_ok then error(\"Sentencia If incorrecta\") else I.tipo}", 6},
        {"B -> {zona_declaracion=false} S", 7},

        {"T -> int {T.tipo=ent}", 8},
        {"T -> boolean {T.tipo=logico}", 9},
        {"T -> string {T.tipo=cadena}", 10},

        {"E -> R N", 11},

        {"N -> > R N", 12},
        {"N -> lambda {N.tipo=vacio}", 13},

        {"R -> O P", 14},

        {"P -> - O P", 15},
        {"P -> lambda {P.tipo=vacio}", 16},

        {"O -> M Y", 17},

        {"Y -> % M Y", 18},
        {"Y -> lambda {Y.tipo=vacio}", 19},

        {"M -> - M", 20},
        {"M -> ! M", 21},
        {"M -> id V", 22},
        {"M -> ( E )", 23},
        {"M -> constanteEntera", 24},
        {"M -> cadena", 25},
        {"M -> -- id", 26},

        {"V -> ( L )", 27},
        {"V -> lambda {V.tipo=vacio}", 28},

        {"L -> E Q", 29},
        {"L -> lambda {L.tipo=vacio}", 30},

        {"Q -> , E Q", 31},
        {"Q -> lambda {Q.tipo=vacio}", 32},

        {"I -> S", 33},
        {"I -> { C } J", 34},

        {"J -> else { C }", 35},
        {"J -> lambda {J.tipo=vacio}", 36},

        {"C -> B C", 37},
        {"C -> lambda {C.tipo=vacio}", 38},

        {"S -> id U", 39},
        {"S -> output E ;", 40},
        {"S -> input D ;", 41},
        {"S -> return X ;", 42},

        {"U -> = E ;", 43},
        {"U -> ( L ) ;", 44},

        {"D -> id {D.tipo=BuscaTipo(id.pos)}", 45},
        {"D -> ( id ) {D.tipo=BuscaTipo(id.pos)}", 46},

        {"X -> E", 47},
        {"X -> lambda {X.tipo=vacio}", 48},

        {"F -> function H id {crearTSLocal(),despLocal=0, zona_declaracion=true} ( A ) { C } {DestruirTSLocal()}", 49},

        {"H -> T {H.tipo=T.tipo}", 50},
        {"H -> void {H.tipo=vacio}", 51},

        {"A -> T id K", 52},
        {"A -> void {A.tipo=vacio}", 53},

        {"K -> , T id K ", 54},
        {"K -> lambda {K.tipo=vacio}", 55}

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

        {"F -> function H id ( A ) { C }", 49},

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
        {"{dec_true}","B -> {zona_dec=true} var T id ; {InsertaTipo(id.pos,T.tipo), InsertaDesp(ts,*dl + T.ancho), B.tipo=tipo_ok}"},
        {"{B->if(E)I}","B -> if ( E ) I  {B.tipo=if(E.tipo!=logico then error(\"Condcion del if incorrecta: solo se pueden comprobar condiciones logicas\")) else I.tipo}"},
        {"{dec_false}","declaracion=false"},
        {"{dec_true}","declaracion=true"},
        {"{B->S}","B -> S {B.tipo=S.tipo}"},
        {"{T->int}","T -> int     {T.tipo=ent, T.ancho=4}"},
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
        {"{M->constantaEntera}","M -> constanteEntera   {M.tipo=entero}"},
        {"{M->cadena}","M -> cadena  {M.tipo=cadena}"},
        {"{M->--id}","M -> -- id {M.tipo = if (BuscaTipo(id.pos) != entero) then error(“solo se pueden predecrementar numeros”) else entero}"},
        {"{V->(L)}","V -> ( L )  {V.tipo=L.tipo}   "},
        {"{V->lambda}","V -> lambda  {V.tipo=vacio}"},
        {"{L->EQ}","L -> E Q  {L.tipo = if (Q.tipo != vacio) then E.tipo x K.tipo else E.tipo }"},
        {"{L->lambda}","L -> lambda  {L.tipo=vacio}"},
        {"{Q->,EQ}","Q -> , E Q1    {Q.tipo = if (Q1.tipo != vacio) then E.tipo x Q1.tipo else E.tipo}"},
        {"{Q->lambda}","Q -> lambda  {Q.tipo=vacio}"},
        {"{I->S}","I -> S {I.tipo = S.tipo}"},
        {"{I->{C}J","I -> { C } J"},
        {"{xx.yy}","J -> else { C }    "},
        {"{xx.yy}","J -> lambda  {J.tipo=vacio}"},
        {"{xx.yy}","C -> B C1    {C.tipo = if (B.tipo = tipo_ok AND (C1.tipo = tipo_ok OR C1.tipo=vacio)) then tipo_ok else error()}"},
        {"{xx.yy}","C -> lambda  {C.tipo=vacio}"},
        {"{xx.yy}","S ->  id U   {if (U.tipo=vacio OR BuscaTipoTS(id.pos) = U.tipo->T) then tipo_ok else error(“Error! Se ha asignado un tipo incorrecto a la variable”)"},
        {"{xx.yy}","S -> output E ;   {S.tipo = tipo_ok if E.tipo != tipo_error}"},
        {"{xx.yy}","S -> input D ;   {S.tipo = tipo_ok if D.tipo != tipo_error}"},
        {"{xx.yy}","S -> return X ;  {S.tipo = tipo_ok if X.tipo != tipo_error}"},
        {"{xx.yy}","U -> = E ;  {U.tipo = E.tipo}"},
        {"{xx.yy}","U -> ( L ) ; {U.tipo = L.tipo}  "},
        {"{xx.yy}","D -> id    {D.tipo=BuscaTipo(id.pos)}"},
        {"{xx.yy}","D -> ( id )  {D.tipo=BuscaTipo(id.pos)}"},
        {"{xx.yy}","X -> E    {X.tipo = E.tipo}"},
        {"{xx.yy}","X -> lambda  {X.tipo=vacio}"},
        {"{xx.yy}","// TS global esta en memoria. Se accede mediante pointer para InsertarTipoTSGlobal. Asi  //no hay que crear pilas"},
        {"{xx.yy}","F -> function H {ts = crearTSLocal(), dl = creaDespLocal() despLocal=0, zona_decl=true}  id ( A {zona_dec=false}) {InsertarTipoTSGlobal(id.pos,A.tipo -> H.tipo} { C }  {F.tipo=C.tipo,liberarTabla(ts)}"},
        {"{xx.yy}","H -> T     {H.tipo=T.tipo}"},
        {"{xx.yy}","H -> void   {H.tipo=vacio}"},
        {"{xx.yy}","A -> T id K    {A.tipo = if (K.tipo != vacio) then T.tipo x K.tipo else T.tipo }"},
        {"{xx.yy}","A -> void   {A.tipo=vacio}"},
        {"{xx.yy}","K ->  , T id K1 {K.tipo = if (K1.tipo != vacio) then T.tipo x K1.tipo else T.tipo}"},
        {"{xx.yy}","K -> lambda   {K.tipo=vacio}"},
    };

    map<pair<string, string>, string> M = {
        {{"Z", "id"}, "B {Z->BZ} Z"},
        {{"Z", "var"}, "B {Z->BZ} Z"},
        {{"Z", "function"}, "F Z {Z->FZ}"},
        {{"Z", "input"}, "B {Z->BZ} Z"},
        {{"Z", "output"}, "B {Z->BZ} Z"},
        {{"Z", "return"}, "B {Z->BZ} Z"},
        {{"Z", "if"}, "B {Z->BZ} Z"},
        {{"Z", "$"}, "lambda {Z->lambda}"},
        {{"B", "id"}, "S {B->S}"},
        {{"B", "input"}, "S {B->S}"},
        {{"B", "output"}, "S {B->S}"},
        {{"B", "return"}, "S {B->S}"},
        {{"B", "var"}, "{dec_true} var T id ; {B->varTid;}"},
        {{"B", "if"}, "if ( E ) I {B->if(E)I}"},
        {{"T", "boolean"}, "boolean {T->int}"},
        {{"T", "int"}, "int {T->int}"},
        {{"T", "string"}, "string {T->string}"},
        {{"E", "id"}, "R N"},
        {{"E", "cadena"}, "R N"},
        {{"E", "constanteEntera"}, "R N"},
        {{"E", "("}, "R N {E->RN}"},
        {{"E", "!"}, "R N {E->RN}"},
        {{"E", "--"}, "R N {E->RN}"},
        {{"N", ")"}, "lambda {N->lambda}"},
        {{"N", ";"}, "lambda {N->lambda}"},
        {{"N", ">"}, "> R N {N->>RN}"},
        {{"R", "id"}, "O P"},
        {{"R", "cadena"}, "O P"},
        {{"R", "constanteEntera"}, "O P"},
        {{"R", "("}, "O P {R->OP}"},
        {{"R", "!"}, "O P {R->OP}"},
        {{"R", "--"}, "O P {R->OP}"},
        {{"P", ")"}, "lambda"},
        {{"P", ";"}, "lambda"},
        {{"P", "-"}, "- O P"},
        {{"P", ">"}, "lambda"},
        {{"O", "id"}, "M Y"},
        {{"O", "cadena"}, "M Y"},
        {{"O", "constanteEntera"}, "M Y"},
        {{"O", "("}, "M Y"},
        {{"O", "!"}, "M Y"},
        {{"O", "--"}, "M Y"},
        {{"Y", ")"}, "lambda"},
        {{"Y", ";"}, "lambda"},
        {{"Y", "-"}, "lambda"},
        {{"Y", ">"}, "lambda"},
        {{"Y", "%"}, "% M Y"},
        {{"M", "id"}, "id V"},
        {{"M", "cadena"}, "cadena"},
        {{"M", "constanteEntera"}, "constanteEntera"},
        {{"M", "("}, "( E )"},
        {{"M", "!"}, "! M"},
        {{"M", "--"}, "-- id"},
        {{"V", "("}, "( L )"},
        {{"V", ")"}, "lambda"},
        {{"V", ";"}, "lambda"},
        {{"V", "-"}, "lambda"},
        {{"V", "%"}, "lambda"},
        {{"V", ">"}, "lambda"},
        {{"L", "id"}, "E Q"},
        {{"L", "cadena"}, "E Q"},
        {{"L", "constanteEntera"}, "E Q"},
        {{"L", "("}, "E Q"},
        {{"L", ")"}, "lambda"},
        {{"L", "!"}, "E Q"},
        {{"L", "--"}, "E Q"},
        {{"Q", ")"}, "lambda"},
        {{"Q", ","}, ", E Q"},
        {{"I", "id"}, "S"},
        {{"I", "input"}, "S"},
        {{"I", "output"}, "S"},
        {{"I", "return"}, "S"},
        {{"I", "{"}, "{ C } J"},
        {{"J", "else"}, "else { C }"},
        {{"J", "var"}, "lambda"},
        {{"J", "$"}, "lambda"},
        {{"C", "id"}, "B C"},
        {{"C", "var"}, "B C"},
        {{"C", "input"}, "B C"},
        {{"C", "output"}, "B C"},
        {{"C", "return"}, "B C"},
        {{"C", "if"}, "B C"},
        {{"C", "}"}, "lambda"},
        {{"S", "id"}, "id U"},
        {{"S", "input"}, "input D ;"},
        {{"S", "output"}, "output E ;"},
        {{"S", "return"}, "return X ;"},
        {{"U", "("}, "( L ) ;"},
        {{"U", "="}, "= E ;"},
        {{"D", "id"}, "id"},
        {{"D", "("}, "( id )"},
        {{"X", "id"}, "E"},
        {{"X", "cadena"}, "E"},
        {{"X", "constanteEntera"}, "E"},
        {{"X", "("}, "E"},
        {{"X", ";"}, "lambda"},
        {{"X", "!"}, "E"},
        {{"X", "--"}, "E"},
        {{"F", "function"}, "function H id ( A ) { C }"},
        {{"H", "boolean"}, "T"},
        {{"H", "int"}, "T"},
        {{"H", "string"}, "T"},
        {{"H", "void"}, "void"},
        {{"A", "boolean"}, "T id K"},
        {{"A", "int"}, "T id K"},
        {{"A", "string"}, "T id K"},
        {{"A", "void"}, "void"},
        {{"K", ")"}, "lambda"},
        {{"K", ","}, ", T id K"},
        {{"E", "-"}, "R N"},
        {{"N", ","}, "lambda"},
        {{"R", "-"}, "O P"},
        {{"P", ","}, "lambda"},
        {{"O", "-"}, "M Y"},
        {{"Y", ","}, "lambda"},
        {{"M", "-"}, "- M"},
        {{"V", ","}, "lambda"},
        {{"L", "-"}, "E Q"},
        {{"J", "id"}, "lambda"},
        {{"J", "function"}, "lambda"},
        {{"J", "input"}, "lambda"},
        {{"J", "output"}, "lambda"},
        {{"J", "return"}, "lambda"},
        {{"J", "if"}, "lambda"},
        {{"X", "-"}, "E"}
    };

    void error(string unexpected);

public:
    AnalizadorSintactico(AnalizadorLexico &lexico, GestorErrores &errores);
    bool esAccionSemantica(string s);
    void ejecutarRegla(string s);
};
