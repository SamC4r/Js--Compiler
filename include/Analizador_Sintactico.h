#include "analizador_lexico.h"
#include <map>
#include <set>
#include <string>
#include <vector>

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
    map<string, int> producciones = {
        {"W -> Z {createTSGlobal()}",1},

        {"Z -> B Z", 2},
        {"Z -> F Z", 3},
        {"Z -> lambda {Z.tipo=vacio}", 4},

        {"B -> var T id ; {InsertaTipo(id.pos,T.tipo)}", 5},
        {"B -> if ( E ) I {B.tipo=if(E.tipo!=logico then error(\"no es un booleano\")) else if I.tipo!=tipo_ok then error(\"Sentencia If incorrecta\") else I.tipo}", 6},
        {"B -> S", 7},

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

        {"D -> id", 45},
        {"D -> ( id )", 46},

        {"X -> E", 47},
        {"X -> lambda {X.tipo=vacio}", 48},

        {"F -> function H id ( A ) { C }", 49},

        {"H -> T", 50},
        {"H -> void", 51},

        {"A -> T id K", 52},
        {"A -> void", 53},

        {"K -> , T id K", 54},
        {"K -> lambda {K.}", 55}
    };

    map<pair<string, string>, string> M = {
        {{"Z", "id"}, "B Z"},
        {{"Z", "var"}, "B Z"},
        {{"Z", "function"}, "F Z"},
        {{"Z", "input"}, "B Z"},
        {{"Z", "output"}, "B Z"},
        {{"Z", "return"}, "B Z"},
        {{"Z", "if"}, "B Z"},
        {{"Z", "$"}, "lambda"},
        {{"B", "id"}, "S"},
        {{"B", "var"}, "var T id ;"},
        {{"B", "input"}, "S"},
        {{"B", "output"}, "S"},
        {{"B", "return"}, "S"},
        {{"B", "if"}, "if ( E ) I"},
        {{"T", "boolean"}, "boolean"},
        {{"T", "int"}, "int"},
        {{"T", "string"}, "string"},
        {{"E", "id"}, "R N"},
        {{"E", "cadena"}, "R N"},
        {{"E", "constanteEntera"}, "R N"},
        {{"E", "("}, "R N"},
        {{"E", "!"}, "R N"},
        {{"E", "--"}, "R N"},
        {{"N", ")"}, "lambda"},
        {{"N", ";"}, "lambda"},
        {{"N", ">"}, "> R N"},
        {{"R", "id"}, "O P"},
        {{"R", "cadena"}, "O P"},
        {{"R", "constanteEntera"}, "O P"},
        {{"R", "("}, "O P"},
        {{"R", "!"}, "O P"},
        {{"R", "--"}, "O P"},
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
};
