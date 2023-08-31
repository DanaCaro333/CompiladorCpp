#ifndef ANALIZADORSINTACTICO_H
#define ANALIZADORSINTACTICO_H
#include <iostream>
#include <string>
#include <vector>

#include "AnalizadorLexico.h"
#include "Nodo.h"
#pragma once
using namespace std;

class AnalizadorSintactico {
 public:
  explicit AnalizadorSintactico(const string &cadena);
  ~AnalizadorSintactico();
  void print();

 private:
  struct Condicional {
    string label;
    Nodo *inicio;
  };

  AnalizadorLexico Tokens;
  string m_cadena;
  string temporal;
  string codigo;
  string dato_str;
  int n = 0;
  int i = 0;
  bool condition = false;
  int index = 0;
  int sig = 0;
  vector<Condicional> condicionales;
  vector<string> instruccion;
  string TresDirecciones(Nodo *root);
  int label = 0;
  void TipoOrden(Nodo *root, string t_left, string t_right);
  void coincidir(const TokenType &token);
  void skip_spaces();
  Nodo *programa();
  Nodo *declaraciones();
  Nodo *sig_declaraciones();
  Nodo *declaracion();
  int tipo();
  Nodo *lista_variables(int type);
  Nodo *sig_lista_variables(int type);
  Nodo *ordenes();
  Nodo *sig_ordenes();
  Nodo *orden();
  Nodo *condicion();
  Nodo *sig_condicion();
  Nodo *comparacion();
  int condicion_op();
  Nodo *operador();
  Nodo *numeros();
  Nodo *bucle_while();
  Nodo *asignar();
  Nodo *expresion_arit();
  Nodo *exp_arit();
  int operador_arit();
};

#endif