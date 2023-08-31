#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H
#include <iostream>
#include <string>
#include <vector>
#pragma once
#include <fstream>

#include "Hoja.h"
using namespace std;
enum class TokenType {
  ESPACIO,
  NUMERO,
  IDENTIFICADOR,
  BEGIN,
  END,
  TIPO,
  COMA,
  PUNTO,
  ASIGNACION,
  PUNTO_Y_COMA,
  DOS_PUNTOS,
  PARENTESIS,
  PARENTESIS_END,
  BUCLE,
  BUCLE_END,
  CONDICION,
  CONDICION_ELSE,
  OPERADOR_BOOL,
  OPERADOR_ARITMETICO,
};

class AnalizadorLexico {
 private:
  fstream Archivo;
  string linea;
  string _token;

  void leer();
  bool isNumber();
  void separate();
  void evaluate();

 public:
  struct Token {
    TokenType tipo;
    string value;
    int id;
  };

  int total = 0;
  AnalizadorLexico();
  vector<Hoja> Tabla;
  vector<Token> List;
  int LookTable(string valor);
  bool type(int i);
  void set_type(int i, string type);
  void start(string file);
};

#endif