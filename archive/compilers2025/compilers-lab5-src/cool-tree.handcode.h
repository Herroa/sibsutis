//
// The following include files must come first.

#ifndef COOL_TREE_HANDCODE_H
#define COOL_TREE_HANDCODE_H

#include <iostream>

#include "stringtab.h"
#include "tree.h"

#define yylineno curr_lineno

/* a type renaming */
typedef int Boolean;
class Entry;
typedef Entry *Symbol;

extern int yylineno;

inline Boolean copy_Boolean(Boolean b) { return b; }
inline void assert_Boolean(Boolean) {}
inline void dump_Boolean(std::ostream &stream, int padding, Boolean b) {
  stream << pad(padding) << (int)b << "\n";
}

void dump_Symbol(std::ostream &stream, int padding, Symbol b);
void assert_Symbol(Symbol b);
Symbol copy_Symbol(Symbol b);

class Program_class;
typedef Program_class *Program;
class Class__class;
typedef Class__class *Class_;
class Feature_class;
typedef Feature_class *Feature;
class Formal_class;
typedef Formal_class *Formal;
class Expression_class;
typedef Expression_class *Expression;
class Case_class;
typedef Case_class *Case;

typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;
typedef list_node<Feature> Features_class;
typedef Features_class *Features;
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;

#define Program_EXTRAS virtual void dump_with_types(std::ostream &, int) = 0;

#define program_EXTRAS void dump_with_types(std::ostream &, int);

#define Class__EXTRAS                  \
  virtual Symbol get_name() = 0;       \
  virtual Symbol get_parent() = 0;     \
  virtual Features get_features() = 0; \
  virtual Symbol get_filename() = 0;   \
  virtual void dump_with_types(std::ostream &, int) = 0;

#define class__EXTRAS                          \
  Symbol get_name() { return name; }           \
  Symbol get_parent() { return parent; }       \
  Features get_features() { return features; } \
  Symbol get_filename() { return filename; }   \
  void dump_with_types(std::ostream &, int);

#define Feature_EXTRAS                  \
  virtual bool is_method() = 0;         \
  virtual Symbol get_name() = 0;        \
  virtual Symbol get_type_decl() = 0;   \
  virtual Symbol get_return_type() = 0; \
  virtual Formals get_formals() = 0;    \
  virtual Expression get_init() = 0;    \
  virtual Expression get_expr() = 0;    \
  virtual void dump_with_types(std::ostream &, int) = 0;

#define method_EXTRAS                                       \
  bool is_method() override { return true; }                \
  Symbol get_name() override { return name; }               \
  Symbol get_return_type() override { return return_type; } \
  Formals get_formals() override { return formals; }        \
  Expression get_expr() override { return expr; }           \
  Symbol get_type_decl() override { return NULL; }          \
  Expression get_init() override { return NULL; }

#define attr_EXTRAS                                     \
  bool is_method() override { return false; }           \
  Symbol get_name() override { return name; }           \
  Symbol get_type_decl() override { return type_decl; } \
  Expression get_init() override { return init; }       \
  Symbol get_return_type() override { return NULL; }    \
  Formals get_formals() override { return NULL; }       \
  Expression get_expr() override { return NULL; }

#define Feature_SHARED_EXTRAS void dump_with_types(std::ostream &, int);

#define Formal_EXTRAS                 \
  virtual Symbol get_name() = 0;      \
  virtual Symbol get_type_decl() = 0; \
  virtual void dump_with_types(std::ostream &, int) = 0;

#define formal_EXTRAS                                   \
  Symbol get_name() override { return name; }           \
  Symbol get_type_decl() override { return type_decl; } \
  void dump_with_types(std::ostream &, int);

#define Case_EXTRAS                   \
  virtual Symbol get_name() = 0;      \
  virtual Symbol get_type_decl() = 0; \
  virtual Expression get_expr() = 0;  \
  virtual void dump_with_types(std::ostream &, int) = 0;

#define branch_EXTRAS                                   \
  Symbol get_name() override { return name; }           \
  Symbol get_type_decl() override { return type_decl; } \
  Expression get_expr() override { return expr; }       \
  void dump_with_types(std::ostream &, int);

#define Expression_EXTRAS                                \
  Symbol type;                                           \
  Symbol get_type() { return type; }                     \
  Expression set_type(Symbol s) {                        \
    type = s;                                            \
    return this;                                         \
  }                                                      \
  virtual void dump_with_types(std::ostream &, int) = 0; \
  void dump_type(std::ostream &, int);                   \
  Expression parent;                                     \
  virtual void set_body(const Expression e) {}           \
  Expression_class() { type = (Symbol)NULL; }            \
  virtual bool is_no_expr() { return false; }            \
  virtual bool is_int_const() { return false; }          \
  virtual bool is_bool_const() { return false; }         \
  virtual bool is_string_const() { return false; }       \
  virtual bool is_object() { return false; }             \
  virtual bool is_assign() { return false; }             \
  virtual bool is_static_dispatch() { return false; }    \
  virtual bool is_dispatch() { return false; }           \
  virtual bool is_cond() { return false; }               \
  virtual bool is_loop() { return false; }               \
  virtual bool is_typcase() { return false; }            \
  virtual bool is_block() { return false; }              \
  virtual bool is_let() { return false; }                \
  virtual bool is_plus() { return false; }               \
  virtual bool is_sub() { return false; }                \
  virtual bool is_mul() { return false; }                \
  virtual bool is_divide() { return false; }             \
  virtual bool is_neg() { return false; }                \
  virtual bool is_lt() { return false; }                 \
  virtual bool is_eq() { return false; }                 \
  virtual bool is_leq() { return false; }                \
  virtual bool is_comp() { return false; }               \
  virtual bool is_new() { return false; }                \
  virtual bool is_isvoid() { return false; }             \
  virtual Symbol get_name() { return NULL; }             \
  virtual Symbol get_type_name() { return NULL; }        \
  virtual Symbol get_type_decl() { return NULL; }        \
  virtual Expression get_expr() { return NULL; }         \
  virtual Expression get_pred() { return NULL; }         \
  virtual Expression get_then_exp() { return NULL; }     \
  virtual Expression get_else_exp() { return NULL; }     \
  virtual Expression get_init() { return NULL; }         \
  virtual Expression get_e1() { return NULL; }           \
  virtual Expression get_e2() { return NULL; }           \
  virtual Cases get_cases() { return NULL; }             \
  virtual Expressions get_actual() { return NULL; }      \
  virtual Symbol get_identifier() { return NULL; }       \
  virtual Expressions get_body() { return NULL; }

#define let_EXTRAS                             \
  void set_body(const Expression e) override { \
    e->parent = this;                          \
    body = e;                                  \
  }

#define Expression_SHARED_EXTRAS void dump_with_types(std::ostream &, int);

#endif
