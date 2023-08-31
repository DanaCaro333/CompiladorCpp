#ifndef HOJA_H
#define HOJA_H

#pragma once
#include <string>
#include <vector>

using namespace std;
class Hoja {
 public:
  Hoja(string id, int entrada, string type) {
    val = id;
    index = entrada;
    tipo = type;
  }
  ~Hoja();
  string val;
  int index;
  string tipo;

 private:
};

#endif