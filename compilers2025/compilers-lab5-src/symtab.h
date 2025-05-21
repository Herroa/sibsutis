#ifndef SYMTAB_H
#define SYMTAB_H

#include <list>
#include <vector>

class Entry;
typedef Entry* Symbol;

template <class Elem>
class SymbolTable {
 private:
  struct Scope {
    std::list<std::pair<Symbol, Elem>> symbols;
    Scope() {}
  };
  std::vector<Scope*> scopes;
  Scope* current_scope;

 public:
  SymbolTable() {
    current_scope = new Scope();
    scopes.push_back(current_scope);
  }

  ~SymbolTable() {
    for (auto scope : scopes) {
      delete scope;
    }
  }

  void enterscope() {
    current_scope = new Scope();
    scopes.push_back(current_scope);
  }

  void exitscope() {
    if (scopes.size() > 1) {
      delete current_scope;
      scopes.pop_back();
      current_scope = scopes.back();
    }
  }

  void addid(Symbol name, Elem value) {
    current_scope->symbols.push_back(std::make_pair(name, value));
  }

  Elem lookup(Symbol name) {
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
      for (auto& pair : (*it)->symbols) {
        if (pair.first == name) {
          return pair.second;
        }
      }
    }
    return nullptr;
  }
};

#endif