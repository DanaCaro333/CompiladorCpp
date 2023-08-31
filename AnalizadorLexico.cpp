#include "AnalizadorLexico.h"

AnalizadorLexico::AnalizadorLexico() {}

void AnalizadorLexico::start(string file) {
  Archivo.open(file, ios::in);

  if (!Archivo.is_open()) {
    throw std::runtime_error("tipo failed");
  }
  leer();
}

void AnalizadorLexico::leer() {
  while (!Archivo.eof()) {
    getline(Archivo, linea);
    separate();
  }
}

void AnalizadorLexico::separate() {
  int i = 0;
  while (linea[i] != '\000') {
    while (isdigit(linea[i]) || isalpha(linea[i])) {
      _token += linea[i];
      i++;
    }
    if (_token == "") {
      _token += linea[i];
      i++;
    }
    evaluate();
  }
}
bool AnalizadorLexico::isNumber() {
  for (char const &c : _token) {
    if (std::isdigit(c) == 0) return false;
  }
  return true;
}
int AnalizadorLexico::LookTable(string valor) {
  for (int i = 0; i < Tabla.size(); i++) {
    if (Tabla[i].val == valor) {
      return Tabla[i].index;
    }
  }
  Hoja temp(valor, Tabla.size(), "nulo");
  Tabla.push_back(temp);
  return temp.index;
}

bool AnalizadorLexico::type(int i) {
  if (Tabla[i].tipo != "nulo") {
    return true;
  } else {
    return false;
  }
}

void AnalizadorLexico::set_type(int i, string type) { Tabla[i].tipo = type; }

void AnalizadorLexico::evaluate() {
  Token nuevo;
  if (_token == "begin") {
    nuevo.tipo = TokenType::BEGIN;
    nuevo.id = -1;
  } else if (_token == "end") {
    nuevo.tipo = TokenType::END;
    nuevo.id = -1;
  } else if (_token == "entero") {
    nuevo.tipo = TokenType::TIPO;
    nuevo.id = -1;
  } else if (_token == "real") {
    nuevo.tipo = TokenType::TIPO;
    nuevo.id = -1;
  } else if (_token == ",") {
    nuevo.tipo = TokenType::COMA;
    nuevo.id = -1;
  } else if (_token == ";") {
    nuevo.tipo = TokenType::PUNTO_Y_COMA;
    nuevo.id = -1;
  } else if (_token == "if") {
    nuevo.tipo = TokenType::CONDICION;
    nuevo.id = -1;
  } else if (_token == "else") {
    nuevo.tipo = TokenType::CONDICION_ELSE;
    nuevo.id = -1;
  } else if (_token == "(") {
    nuevo.tipo = TokenType::PARENTESIS;
    nuevo.id = -1;
  } else if (_token == ")") {
    nuevo.tipo = TokenType::PARENTESIS_END;
    nuevo.id = -1;
  } else if (_token == "+" || _token == "-" || _token == "*" || _token == "/") {
    nuevo.tipo = TokenType::OPERADOR_ARITMETICO;
    nuevo.id = -1;
  } else if (_token == "<" || _token == ">" || _token == "<=" ||
             _token == ">=" || _token == "<>" || _token == "=") {
    nuevo.tipo = TokenType::OPERADOR_BOOL;
    nuevo.id = -1;
  } else if (_token == ":") {
    nuevo.tipo = TokenType::DOS_PUNTOS;
    nuevo.id = -1;
  } else if (_token == ".") {
    nuevo.tipo = TokenType::PUNTO;
    nuevo.id = -1;
  } else if (_token == " ") {
    nuevo.tipo = TokenType::ESPACIO;
    nuevo.id = -1;
  } else if (_token == "while") {
    nuevo.tipo = TokenType::BUCLE;
    nuevo.id = -1;
  } else if (_token == "endwhile") {
    nuevo.tipo = TokenType::BUCLE_END;
    nuevo.id = -1;
  } else if (isNumber()) {
    nuevo.tipo = TokenType::NUMERO;
    nuevo.id = -1;
  } else if (isalpha(_token[0])) {
    nuevo.tipo = TokenType::IDENTIFICADOR;
    nuevo.id = LookTable(_token);
    total++;
  } else {
    throw std::runtime_error("evaluation failed");
  }

  nuevo.value = _token;
  List.insert(List.end(), nuevo);
  _token.clear();
}
