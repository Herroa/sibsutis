#ifndef SEMANT_H_
#define SEMANT_H_

#include <iostream>
#include <map>
#include <set>
#include <unordered_map>

#include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"

typedef SymbolTable<Symbol> ObjectEnvironment;

class ClassTable {
 private:
  int semant_errors;
  void install_basic_classes();
  std::ostream& error_stream;
  std::unordered_map<Symbol, Class_> classes;
  std::unordered_map<Symbol, std::set<Symbol>> inheritance_graph;
  ObjectEnvironment object_env;

  // Вспомогательные методы
  bool check_inheritance_cycle(Symbol class_name, std::set<Symbol>& visited);
  bool is_subtype(Symbol child, Symbol parent);
  Symbol lub(Symbol a, Symbol b);
  void check_main_method();
  void check_unique_names();
  void check_types();

  // Методы для проверки типов
  Symbol typecheck_expression(Expression e, Class_ c);
  Symbol typecheck_method_call(Expression e, Symbol method_name,
                               Expressions actual, Class_ c);
  Symbol typecheck_static_dispatch(Expression e, Symbol type_name,
                                   Symbol method_name, Expressions actual,
                                   Class_ c);
  Symbol typecheck_dispatch(Expression e, Symbol method_name,
                            Expressions actual, Class_ c);
  Symbol typecheck_cond(Expression pred, Expression then_exp,
                        Expression else_exp, Class_ c);
  Symbol typecheck_loop(Expression pred, Expressions body, Class_ c);
  Symbol typecheck_typcase(Expression expr, Cases cases, Class_ c);
  Symbol typecheck_block(Expressions body, Class_ c);
  Symbol typecheck_let(Symbol identifier, Symbol type_decl, Expression init,
                       Expressions body, Class_ c);
  Symbol typecheck_arithmetic(Expression e1, Expression e2, Class_ c);
  Symbol typecheck_comparison(Expression e1, Expression e2, Class_ c);
  Symbol typecheck_eq(Expression e1, Expression e2, Class_ c);
  Symbol typecheck_neg(Expression e1, Class_ c);
  Symbol typecheck_comp(Expression e1, Class_ c);
  Symbol typecheck_new(Symbol type_name, Class_ c);
  Symbol typecheck_isvoid(Expression e1, Class_ c);
  Symbol typecheck_object(Symbol name, Class_ c);
  Symbol typecheck_assign(Symbol name, Expression expr, Class_ c);

 public:
  ClassTable(Classes);
  int errors() { return semant_errors; }
  std::ostream& semant_error();
  std::ostream& semant_error(Class_ c);
  std::ostream& semant_error(Symbol filename, tree_node* t);
  std::ostream& semant_error(Expression e);
  std::ostream& semant_error(Class_ c, Expression e);
  std::ostream& semant_error(Symbol filename, tree_node* t, Expression e);
  std::ostream& semant_error(int line_number);
};

#endif