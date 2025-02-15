#include <fstream>
#include <queue>
#include <string>
#include <unordered_map>

#include "ColaTablaSimbolos.h"
#include "TiposToken.h"

using namespace std;

#pragma once
class Generator {

  fstream token_file;

public:
    TablaSimbolos* ts_global;
  ColaTablaSimbolos *queue;
  bool buscando;
    int lineas;
    int prev_lineas;
    bool zona_declaracion;
    bool function=false;
    string lastTokenType;
    string lastTokenAttribute;
    std::queue<pair<string,string>> q;

    unordered_map<string, int> codigo_palabra_reservada = {
        {"boolean", 1}, {"else", 2}, {"function", 3}, {"if", 4},
        {"input", 5},   {"int", 6},  {"output", 7},   {"return", 8},
        {"string", 9},  {"var", 10}, {"void", 11},
    };

    unordered_map<string, string> tipo_caracter_especial = {
        {"=", "operadorAsignacion"}, {",", "coma"},
        {";", "puntoComa"},          {"(", "parentesisIzda"},
        {")", "parentesisDcha"},     {"{", "llavesIzda"},
        {"}", "llavesDcha"},         {"-", "operadorMenos"},
        {"%", "operadorModulo"},     {">", "operadorMayor"},
        {"--", "operadorEspecial"},
        {"!", "operadorNegacion"},
    };



    unordered_map<string, string> operadoresEspeciales = {
        {"--", "operadorEspecial"},
    };

    template <typename T1, typename T2> void gen_token(T1 tipo, T2 atributo);
    template <typename T1> void gen_token(T1 tipo, string cadena);
    void gen_token(string tipo);

    void init(string token_file_name, ColaTablaSimbolos &queue);
    void Token(string identificador);
    void Token(char c);
    void Token(int valor);
    void Token(TiposToken tipo);
    void Token(string str, char del);
};
