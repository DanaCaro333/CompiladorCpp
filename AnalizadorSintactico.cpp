#include "AnalizadorSintactico.h"

void AnalizadorSintactico::skip_spaces() {
  while (Tokens.List[i].tipo == TokenType::ESPACIO) {
    i++;
  }
}
void AnalizadorSintactico::print() { cout << codigo << "\n"; }

AnalizadorSintactico::AnalizadorSintactico(const string &cadena) {
  Tokens.start(cadena);
  try {
    TresDirecciones(programa()->right);
    for (Condicional i : condicionales) {
      cout << endl << i.label << endl;
      TresDirecciones(i.inicio);
    }
    cout << "SINTAXIS CORRECTA!!!" << endl;
  } catch (const std::exception &e) {
    cout << "sintaxis incorrecta " << e.what() << "\n";
  }
}

AnalizadorSintactico::~AnalizadorSintactico() {}

void AnalizadorSintactico::TipoOrden(Nodo *root, string t_left,
                                     string t_right) {
  if (Tokens.List[root->dato].tipo == TokenType::CONDICION ||
      Tokens.List[root->dato].tipo == TokenType::BUCLE) {
    cout << "IF " << t_left
         << " GOTO "
            "LABEL"
         << label << endl;
  } else if (Tokens.List[root->dato].tipo == TokenType::CONDICION_ELSE) {
    cout << "GOTO SIG" << sig << endl << endl;
  } else if (Tokens.List[root->dato].tipo == TokenType::BUCLE_END) {
    label++;
    cout << "LABEL" << label << endl << endl;
  } else if (Tokens.List[root->dato].tipo == TokenType::PARENTESIS_END) {
    sig++;
    cout << "SIG" << sig << endl;
  } else if (Tokens.List[root->dato].tipo == TokenType::NUMERO) {
    cout << "temp" << index << ": " << Tokens.List[root->dato].value << endl;
  } else if (Tokens.List[root->dato].tipo == TokenType::IDENTIFICADOR) {
    cout << Tokens.List[root->dato].value << endl;
  } else if (Tokens.List[root->dato].tipo == TokenType::OPERADOR_BOOL) {
    index++;
    cout << "temp" << index << ": " << t_left << " "
         << Tokens.List[root->dato].value << " " << t_right << endl;
  } else if (Tokens.List[root->dato].tipo == TokenType::PARENTESIS) {
    cout << t_left << ": " << t_left << " "
         << Tokens.List[root->right->dato].value << " " << t_right << endl;
  } else {
    cout << t_left << ": " << t_left << " " << Tokens.List[root->dato].value
         << " " << t_right << endl;
  }
}

string AnalizadorSintactico::TresDirecciones(Nodo *root) {
  string temp;
  string temporal1;
  string temporal2;
  cout.flush();
  if (root->left != NULL && !condition) {
    temporal1 = TresDirecciones(root->left);
  }
  condition = false;
  if (Tokens.List[root->dato].tipo == TokenType::CONDICION) {
    label++;
    TipoOrden(root, temporal1, temporal2);
    Condicional nuevo;
    nuevo.label = "LABEL" + to_string(label);
    nuevo.inicio = root->right->left;
    condicionales.push_back(nuevo);
    condition = true;
  }
  if (Tokens.List[root->dato].tipo == TokenType::BUCLE) {
    label++;
    cout << "LABEL" << label << endl;
  }
  if (root->right != NULL) {
    temporal2 = TresDirecciones(root->right);
  }
  if (root->left == NULL && root->right == NULL) {
    if (Tokens.List[root->dato].tipo == TokenType::NUMERO) {
      index++;
      TipoOrden(root, temporal1, temporal2);
      temp = "temp" + to_string(index);
    } else if (Tokens.List[root->dato].tipo == TokenType::CONDICION_ELSE) {
      TipoOrden(root, temporal1, temporal2);
    } else if (temporal != "") {
      cout << temp << ": " << temporal << endl;
      temporal.clear();
    } else {
      temp = Tokens.List[root->dato].value;
    }
  } else if (root->left != NULL && root->right == NULL) {
    if (Tokens.List[root->dato].tipo == TokenType::OPERADOR_ARITMETICO) {
      temporal =
          Tokens.List[root->dato].value + " " + "temp" + to_string(index);
    } else {
      if (Tokens.List[root->dato].tipo == TokenType::NUMERO) {
        index++;
        TipoOrden(root, temporal1, temporal2);
        cout << "temp" << index << ": "
             << "temp" << index << " " << temporal << endl;
      } else if (Tokens.List[root->dato].tipo != TokenType::PARENTESIS) {
        cout << "temp" << index << ": " << Tokens.List[root->dato].value << " "
             << temporal << endl;
      }
      temporal.clear();
    }
    temp = "temp" + to_string(index);
  } else if (root->left == NULL && root->right != NULL) {
    cout << Tokens.List[root->dato].value << endl;
    temp = "temp" + to_string(index);
  } else if (Tokens.List[root->dato].tipo != TokenType::CONDICION &&
             Tokens.List[root->dato].tipo != TokenType::PUNTO_Y_COMA &&
             Tokens.List[root->dato].tipo != TokenType::CONDICION_ELSE) {
    TipoOrden(root, temporal1, temporal2);
    temp = "temp" + to_string(index);
  }
  return temp;
}

void AnalizadorSintactico::coincidir(const TokenType &token) {
  if (Tokens.List[i].tipo != token) {
    throw std::runtime_error("unnexpected token: " + Tokens.List[i].value);
  } else {
    i++;
  }
}

Nodo *AnalizadorSintactico::programa() {
  Nodo *nuevo = new Nodo;
  nuevo->dato = i;
  coincidir(TokenType::BEGIN);
  skip_spaces();
  nuevo->left = declaraciones();
  skip_spaces();
  nuevo->right = ordenes();
  coincidir(TokenType::END);
  return nuevo;
}
Nodo *AnalizadorSintactico::declaraciones() {
  Nodo *nuevo = new Nodo;
  nuevo->left = declaracion();
  nuevo->dato = i;
  coincidir(TokenType::PUNTO_Y_COMA);
  skip_spaces();
  nuevo->right = sig_declaraciones();
  return nuevo;
}
Nodo *AnalizadorSintactico::sig_declaraciones() {
  Nodo *nuevo = new Nodo;
  if (Tokens.List[i].tipo == TokenType::TIPO) {
    skip_spaces();
    nuevo->left = declaracion();
    nuevo->dato = i;
    coincidir(TokenType::PUNTO_Y_COMA);
    nuevo->right = sig_declaraciones();
  } else {
    nuevo = NULL;
  }
  return nuevo;
}
Nodo *AnalizadorSintactico::declaracion() {
  Nodo *nuevo = new Nodo;
  nuevo->dato = tipo();
  nuevo->left = NULL;
  nuevo->right = lista_variables(nuevo->dato);
  return nuevo;
}
int AnalizadorSintactico::tipo() {
  int indice = i;
  if (Tokens.List[i].value == "entero") {
    codigo += Tokens.List[i].value;
  } else if (Tokens.List[i].value == "real") {
    codigo += Tokens.List[i].value;
  } else {
    throw std::runtime_error("tipo failed");
  }
  i++;
  skip_spaces();
  return indice;
}

Nodo *AnalizadorSintactico::lista_variables(int type) {
  Nodo *nuevo = new Nodo;
  coincidir(TokenType::IDENTIFICADOR);
  i--;
  nuevo->dato = i;
  Tokens.set_type(Tokens.List[nuevo->dato].id, Tokens.List[type].value);
  i++;
  nuevo->left = NULL;
  nuevo->right = sig_lista_variables(type);
  return nuevo;
}

Nodo *AnalizadorSintactico::sig_lista_variables(int type) {
  Nodo *nuevo = new Nodo;
  skip_spaces();
  if (Tokens.List[i].tipo == TokenType::COMA) {
    nuevo->dato = i;
    Tokens.set_type(Tokens.List[nuevo->dato].id, Tokens.List[type].value);
    coincidir(TokenType::COMA);
    nuevo->left = NULL;
    skip_spaces();
    nuevo->right = lista_variables(type);
  } else {
    nuevo = NULL;
  }
  return nuevo;
}

Nodo *AnalizadorSintactico::ordenes() {
  Nodo *nuevo = new Nodo;
  nuevo->left = orden();
  nuevo->dato = i;
  coincidir(TokenType::PUNTO_Y_COMA);
  nuevo->right = sig_ordenes();
  return nuevo;
}
Nodo *AnalizadorSintactico::sig_ordenes() {
  Nodo *nuevo = new Nodo;
  if (Tokens.List[i].tipo == TokenType::CONDICION ||
      Tokens.List[i].tipo == TokenType::BUCLE ||
      Tokens.List[i].tipo == TokenType::IDENTIFICADOR) {
    nuevo->left = orden();
    nuevo->dato = i;
    coincidir(TokenType::PUNTO_Y_COMA);
    nuevo->right = sig_ordenes();
  } else if (Tokens.List[i].tipo == TokenType::END ||
             Tokens.List[i].tipo == TokenType::BUCLE_END ||
             Tokens.List[i].tipo == TokenType::CONDICION_ELSE) {
    nuevo->fill(i, NULL, NULL);
  } else {
    nuevo = NULL;
  }
  return nuevo;
}
Nodo *AnalizadorSintactico::orden() {
  Nodo *nuevo = new Nodo;
  if (Tokens.List[i].tipo == TokenType::CONDICION) {
    codigo += Tokens.List[i].value;
    i++;
    skip_spaces();
    nuevo = condicion();
  } else if (Tokens.List[i].tipo == TokenType::BUCLE) {
    codigo += Tokens.List[i].value;
    i++;
    skip_spaces();
    nuevo = bucle_while();
  } else if (Tokens.List[i].tipo == TokenType::IDENTIFICADOR) {
    nuevo = asignar();
  } else {
    throw std::runtime_error("token failed, Orden no encontrada");
  }
  return nuevo;
}
Nodo *AnalizadorSintactico::condicion() {
  Nodo *nuevo = new Nodo;
  Nodo *der = new Nodo;
  nuevo->dato = i - 1;
  coincidir(TokenType::PARENTESIS);
  nuevo->left = comparacion();
  der->dato = i;
  coincidir(TokenType::PARENTESIS_END);
  skip_spaces();
  der->left = ordenes();
  der->right = sig_condicion();
  nuevo->right = der;
  return nuevo;
}
Nodo *AnalizadorSintactico::sig_condicion() {
  Nodo *nuevo = new Nodo;
  Nodo *der = new Nodo;
  if (Tokens.List[i].tipo == TokenType::CONDICION_ELSE) {
    nuevo->dato = i;
    coincidir(TokenType::CONDICION_ELSE);
    nuevo->left = ordenes();
    der->fill(i, NULL, NULL);
    nuevo->right = der;
    coincidir(TokenType::END);
  } else {
    nuevo->fill(i, NULL, NULL);
    coincidir(TokenType::END);
  }
  return nuevo;
}
Nodo *AnalizadorSintactico::comparacion() {
  Nodo *nuevo = new Nodo;
  skip_spaces();
  nuevo->left = operador();
  nuevo->dato = condicion_op();
  skip_spaces();
  nuevo->right = operador();
  return nuevo;
}
int AnalizadorSintactico::condicion_op() {
  skip_spaces();
  coincidir(TokenType::OPERADOR_BOOL);
  return (i - 1);
}
Nodo *AnalizadorSintactico::operador() {
  Nodo *nuevo = new Nodo;
  if (Tokens.List[i].tipo == TokenType::IDENTIFICADOR) {
    if (!Tokens.type(Tokens.List[i].id)) {
      throw std::runtime_error("token failed, variable" + Tokens.List[i].value +
                               "no declarada");  // verifica que este
                                                 // declarada
    }
    coincidir(TokenType::IDENTIFICADOR);
    nuevo->fill(i - 1, NULL, NULL);
  } else if (Tokens.List[i].tipo == TokenType::NUMERO) {
    nuevo = numeros();
  } else {
    throw std::runtime_error("operador failed");
  }
  return nuevo;
}
Nodo *AnalizadorSintactico::numeros() {
  Nodo *nuevo = new Nodo;
  nuevo->dato = i;
  if (Tokens.List[i + 1].tipo == TokenType::PUNTO) {
    i++;
    Tokens.List[nuevo->dato].value += Tokens.List[i].value;
    coincidir(TokenType::PUNTO);
    Tokens.List[nuevo->dato].value += Tokens.List[i].value;
    coincidir(TokenType::NUMERO);
  }
  nuevo->left = NULL;
  nuevo->right = NULL;
  return nuevo;
}

Nodo *AnalizadorSintactico::bucle_while() {
  Nodo *nuevo = new Nodo;
  nuevo->dato = i - 1;
  coincidir(TokenType::PARENTESIS);
  nuevo->left = comparacion();
  coincidir(TokenType::PARENTESIS_END);
  skip_spaces();
  nuevo->right = ordenes();
  coincidir(TokenType::BUCLE_END);
  return nuevo;
}
Nodo *AnalizadorSintactico::asignar() {
  Nodo *nuevo = new Nodo;
  Nodo *izq = new Nodo;
  if (!Tokens.type(Tokens.List[i].id)) {
    throw std::runtime_error("token failed, variable " + Tokens.List[i].value +
                             " no declarada");  // verifica que este declarada
  }
  izq->fill(i, NULL, NULL);
  nuevo->left = izq;
  coincidir(TokenType::IDENTIFICADOR);
  skip_spaces();
  nuevo->dato = i;
  coincidir(TokenType::DOS_PUNTOS);
  coincidir(TokenType::OPERADOR_BOOL);
  if (Tokens.List[i - 1].value != "=") {
    throw std::runtime_error("token failed, expected '='");
  } else {
    Tokens.List[i - 2].value += Tokens.List[i - 1].value;
  }
  skip_spaces();
  nuevo->right = expresion_arit();
  return nuevo;
}
Nodo *AnalizadorSintactico::expresion_arit() {
  Nodo *nuevo = new Nodo;
  if (Tokens.List[i].tipo == TokenType::PARENTESIS) {
    nuevo->dato = i;
    coincidir(TokenType::PARENTESIS);
    skip_spaces();
    Nodo *izq = new Nodo;
    nuevo->left = expresion_arit();
    coincidir(TokenType::PARENTESIS_END);
    skip_spaces();
    nuevo->right = exp_arit();
  } else if (Tokens.List[i].tipo == TokenType::IDENTIFICADOR) {
    if (!Tokens.type(Tokens.List[i].id)) {
      throw std::runtime_error("token failed, variable " +
                               Tokens.List[i].value +
                               " no declarada");  // verifica que este declarada
    }
    nuevo->dato = i;
    coincidir(TokenType::IDENTIFICADOR);
    skip_spaces();
    nuevo->left = exp_arit();
    nuevo->right = NULL;
  } else if (Tokens.List[i].tipo == TokenType::NUMERO) {
    nuevo->dato = numeros()->dato;
    coincidir(TokenType::NUMERO);
    skip_spaces();
    nuevo->left = exp_arit();
    nuevo->right = NULL;
  } else {
    throw std::runtime_error("token failed, No coincide con la gramatica");
  }
  return nuevo;
}
Nodo *AnalizadorSintactico::exp_arit() {
  Nodo *nuevo = new Nodo;
  if (Tokens.List[i].tipo == TokenType::OPERADOR_ARITMETICO) {
    nuevo->dato = operador_arit();
    nuevo->left = expresion_arit();
    nuevo->right = exp_arit();
  } else {
    nuevo = NULL;
  }
  return nuevo;
}
int AnalizadorSintactico::operador_arit() {
  int indice = i;
  coincidir(TokenType::OPERADOR_ARITMETICO);
  skip_spaces();
  return indice;
}
