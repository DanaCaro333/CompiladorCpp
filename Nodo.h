#ifndef NODO_H
#define NODO_H

#pragma once
#include <fstream>
#include <string>
#include <vector>

using namespace std;
class Nodo {
 public:
  Nodo();
  void fill(int indice, Nodo *d, Nodo *i) {
    dato = indice;
    right = d;
    left = i;
  }
  ~Nodo();
  int dato;
  void setR(Nodo *r) { right = r; }
  Nodo *right;
  Nodo *left;

 private:
};

#endif