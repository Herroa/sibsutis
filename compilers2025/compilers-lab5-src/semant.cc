#include "semant.h"

#include <iostream>

#include "utilities.h"

extern int semant_debug;
extern char* curr_filename;

ClassTable::ClassTable(Classes classes)
    : semant_errors(0), error_stream(std::cerr) {
  install_basic_classes();

  // Добавляем пользовательские классы
  for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
    Class_ c = classes->nth(i);
    Symbol name = c->get_name();

    if (this->classes.find(name) != this->classes.end()) {
      semant_error(c) << "Class " << name << " was previously defined."
                      << std::endl;
    } else {
      this->classes[name] = c;
      inheritance_graph[name] = std::set<Symbol>();
    }
  }

  // Проверяем граф наследования
  for (auto& pair : this->classes) {
    Class_ c = pair.second;
    Symbol parent = c->get_parent();

    if (this->classes.find(parent) == this->classes.end()) {
      semant_error(c) << "Class " << c->get_name()
                      << " inherits from an undefined class " << parent << "."
                      << std::endl;
    } else {
      inheritance_graph[parent].insert(c->get_name());
    }
  }

  // Проверяем циклы в наследовании
  std::set<Symbol> visited;
  for (auto& pair : this->classes) {
    if (check_inheritance_cycle(pair.first, visited)) {
      semant_error(this->classes[pair.first])
          << "Class " << pair.first << " participates in an inheritance cycle."
          << std::endl;
    }
  }

  // Проверяем наличие метода main
  check_main_method();

  // Проверяем уникальность имен
  check_unique_names();

  // Проверяем типы
  check_types();
}

void ClassTable::install_basic_classes() {
  Symbol Object = idtable.add_string("Object");
  Symbol IO = idtable.add_string("IO");
  Symbol Int = idtable.add_string("Int");
  Symbol Bool = idtable.add_string("Bool");
  Symbol String = idtable.add_string("String");
  Symbol Main = idtable.add_string("Main");
  Symbol no_type = idtable.add_string("_no_type");

  // Добавляем базовые классы
  classes[Object] =
      class_(Object, no_type, nil_Features(), stringtable.add_string("Object"));
  classes[IO] =
      class_(IO, Object, nil_Features(), stringtable.add_string("IO"));
  classes[Int] =
      class_(Int, Object, nil_Features(), stringtable.add_string("Int"));
  classes[Bool] =
      class_(Bool, Object, nil_Features(), stringtable.add_string("Bool"));
  classes[String] =
      class_(String, Object, nil_Features(), stringtable.add_string("String"));
  classes[Main] =
      class_(Main, Object, nil_Features(), stringtable.add_string("Main"));
}

bool ClassTable::check_inheritance_cycle(Symbol class_name,
                                         std::set<Symbol>& visited) {
  if (visited.find(class_name) != visited.end()) {
    return true;
  }

  visited.insert(class_name);

  for (Symbol child : inheritance_graph[class_name]) {
    if (check_inheritance_cycle(child, visited)) {
      return true;
    }
  }

  visited.erase(class_name);
  return false;
}

bool ClassTable::is_subtype(Symbol child, Symbol parent) {
  if (child == parent) return true;
  if (child == idtable.add_string("_no_type")) return true;

  Class_ c = classes[child];
  Symbol current = c->get_parent();

  while (current != idtable.add_string("_no_type")) {
    if (current == parent) return true;
    c = classes[current];
    current = c->get_parent();
  }

  return false;
}

Symbol ClassTable::lub(Symbol a, Symbol b) {
  if (a == b) return a;
  if (a == idtable.add_string("_no_type")) return b;
  if (b == idtable.add_string("_no_type")) return a;

  std::set<Symbol> a_ancestors;
  Symbol current = a;
  while (current != idtable.add_string("_no_type")) {
    a_ancestors.insert(current);
    current = classes[current]->get_parent();
  }

  current = b;
  while (current != idtable.add_string("_no_type")) {
    if (a_ancestors.find(current) != a_ancestors.end()) {
      return current;
    }
    current = classes[current]->get_parent();
  }

  return idtable.add_string("Object");
}

void ClassTable::check_main_method() {
  bool found_main = false;
  for (auto& pair : classes) {
    Class_ c = pair.second;
    if (c->get_name() == idtable.add_string("Main")) {
      Features features = c->get_features();
      for (int i = features->first(); features->more(i);
           i = features->next(i)) {
        Feature f = features->nth(i);
        if (f->is_method() && f->get_name() == idtable.add_string("main")) {
          found_main = true;
          break;
        }
      }
    }
  }

  if (!found_main) {
    semant_error()
        << "Class Main is not defined or does not have a 'main' method."
        << std::endl;
  }
}

void ClassTable::check_unique_names() {
  for (auto& pair : classes) {
    Class_ c = pair.second;
    std::set<Symbol> method_names;
    std::set<Symbol> attr_names;

    Features features = c->get_features();
    for (int i = features->first(); features->more(i); i = features->next(i)) {
      Feature f = features->nth(i);
      Symbol name = f->get_name();

      if (f->is_method()) {
        if (method_names.find(name) != method_names.end()) {
          semant_error(c) << "Method " << name << " is multiply defined."
                          << std::endl;
        }
        method_names.insert(name);
      } else {
        if (attr_names.find(name) != attr_names.end()) {
          semant_error(c) << "Attribute " << name << " is multiply defined."
                          << std::endl;
        }
        attr_names.insert(name);
      }
    }
  }
}

void ClassTable::check_types() {
  for (auto& pair : classes) {
    Class_ c = pair.second;
    Features features = c->get_features();

    for (int i = features->first(); features->more(i); i = features->next(i)) {
      Feature f = features->nth(i);

      if (f->is_method()) {
        // Проверяем типы параметров метода
        Formals formals = f->get_formals();
        for (int j = formals->first(); formals->more(j); j = formals->next(j)) {
          Formal formal = formals->nth(j);
          Symbol type = formal->get_type_decl();

          if (classes.find(type) == classes.end()) {
            semant_error(c) << "Method " << f->get_name()
                            << " has parameter of undefined type " << type
                            << "." << std::endl;
          }
        }

        // Проверяем тип возвращаемого значения
        Symbol return_type = f->get_return_type();
        if (classes.find(return_type) == classes.end()) {
          semant_error(c) << "Method " << f->get_name()
                          << " has undefined return type " << return_type << "."
                          << std::endl;
        }

        // Проверяем тело метода
        Expression expr = f->get_expr();
        Symbol expr_type = typecheck_expression(expr, c);

        if (!is_subtype(expr_type, return_type)) {
          semant_error(c) << "Inferred return type " << expr_type
                          << " of method " << f->get_name()
                          << " does not conform to declared return type "
                          << return_type << "." << std::endl;
        }
      } else {
        // Проверяем тип атрибута
        Symbol type = f->get_type_decl();
        if (classes.find(type) == classes.end()) {
          semant_error(c) << "Attribute " << f->get_name()
                          << " has undefined type " << type << "." << std::endl;
        }

        // Проверяем инициализатор атрибута
        Expression init = f->get_init();
        if (init->is_no_expr()) {
          continue;
        }

        Symbol init_type = typecheck_expression(init, c);
        if (!is_subtype(init_type, type)) {
          semant_error(c) << "Inferred type " << init_type
                          << " of initialization of attribute " << f->get_name()
                          << " does not conform to declared type " << type
                          << "." << std::endl;
        }
      }
    }
  }
}

Symbol ClassTable::typecheck_expression(Expression e, Class_ c) {
  if (e->is_no_expr()) {
    return idtable.add_string("_no_type");
  }

  if (e->is_int_const()) {
    return idtable.add_string("Int");
  }

  if (e->is_bool_const()) {
    return idtable.add_string("Bool");
  }

  if (e->is_string_const()) {
    return idtable.add_string("String");
  }

  if (e->is_object()) {
    return typecheck_object(e->get_name(), c);
  }

  if (e->is_assign()) {
    return typecheck_assign(e->get_name(), e->get_expr(), c);
  }

  if (e->is_static_dispatch()) {
    return typecheck_static_dispatch(e->get_expr(), e->get_type_name(),
                                     e->get_name(), e->get_actual(), c);
  }

  if (e->is_dispatch()) {
    return typecheck_dispatch(e->get_expr(), e->get_name(), e->get_actual(), c);
  }

  if (e->is_cond()) {
    return typecheck_cond(e->get_pred(), e->get_then_exp(), e->get_else_exp(),
                          c);
  }

  if (e->is_loop()) {
    return typecheck_loop(e->get_pred(), e->get_body(), c);
  }

  if (e->is_typcase()) {
    return typecheck_typcase(e->get_expr(), e->get_cases(), c);
  }

  if (e->is_block()) {
    return typecheck_block(e->get_body(), c);
  }

  if (e->is_let()) {
    return typecheck_let(e->get_identifier(), e->get_type_decl(), e->get_init(),
                         e->get_body(), c);
  }

  if (e->is_plus() || e->is_sub() || e->is_mul() || e->is_divide()) {
    return typecheck_arithmetic(e->get_e1(), e->get_e2(), c);
  }

  if (e->is_lt() || e->is_leq()) {
    return typecheck_comparison(e->get_e1(), e->get_e2(), c);
  }

  if (e->is_eq()) {
    return typecheck_eq(e->get_e1(), e->get_e2(), c);
  }

  if (e->is_neg()) {
    return typecheck_neg(e->get_e1(), c);
  }

  if (e->is_comp()) {
    return typecheck_comp(e->get_e1(), c);
  }

  if (e->is_new()) {
    return typecheck_new(e->get_type_name(), c);
  }

  if (e->is_isvoid()) {
    return typecheck_isvoid(e->get_e1(), c);
  }

  return idtable.add_string("_no_type");
}

Symbol ClassTable::typecheck_object(Symbol name, Class_ c) {
  Symbol type = object_env.lookup(name);
  if (type == NULL) {
    semant_error(c) << "Undeclared identifier " << name << "." << std::endl;
    return idtable.add_string("_no_type");
  }
  return type;
}

Symbol ClassTable::typecheck_assign(Symbol name, Expression expr, Class_ c) {
  Symbol type = object_env.lookup(name);
  if (type == NULL) {
    semant_error(c) << "Undeclared identifier " << name << "." << std::endl;
    return idtable.add_string("_no_type");
  }

  Symbol expr_type = typecheck_expression(expr, c);
  if (!is_subtype(expr_type, type)) {
    semant_error(c)
        << "Type " << expr_type
        << " of assigned expression does not conform to declared type " << type
        << " of identifier " << name << "." << std::endl;
  }

  return type;
}

Symbol ClassTable::typecheck_static_dispatch(Expression e, Symbol type_name,
                                             Symbol method_name,
                                             Expressions actual, Class_ c) {
  Symbol expr_type = typecheck_expression(e, c);

  if (classes.find(type_name) == classes.end()) {
    semant_error(c) << "Static dispatch to undefined class " << type_name << "."
                    << std::endl;
    return idtable.add_string("_no_type");
  }

  if (!is_subtype(expr_type, type_name)) {
    semant_error(c) << "Expression type " << expr_type
                    << " does not conform to declared static dispatch type "
                    << type_name << "." << std::endl;
    return idtable.add_string("_no_type");
  }

  return typecheck_method_call(e, method_name, actual, c);
}

Symbol ClassTable::typecheck_dispatch(Expression e, Symbol method_name,
                                      Expressions actual, Class_ c) {
  Symbol expr_type = typecheck_expression(e, c);
  if (expr_type == idtable.add_string("_no_type")) {
    return idtable.add_string("_no_type");
  }

  return typecheck_method_call(e, method_name, actual, c);
}

Symbol ClassTable::typecheck_method_call(Expression e, Symbol method_name,
                                         Expressions actual, Class_ c) {
  Symbol expr_type = typecheck_expression(e, c);
  if (expr_type == idtable.add_string("_no_type")) {
    return idtable.add_string("_no_type");
  }

  Class_ class_def = classes[expr_type];
  Features features = class_def->get_features();

  bool found = false;
  Symbol return_type = idtable.add_string("_no_type");

  for (int i = features->first(); features->more(i); i = features->next(i)) {
    Feature f = features->nth(i);
    if (f->is_method() && f->get_name() == method_name) {
      found = true;
      return_type = f->get_return_type();

      Formals formals = f->get_formals();
      if (formals->len() != actual->len()) {
        semant_error(c) << "Method " << method_name
                        << " called with wrong number of arguments."
                        << std::endl;
        return idtable.add_string("_no_type");
      }

      for (int j = formals->first(); formals->more(j); j = formals->next(j)) {
        Formal formal = formals->nth(j);
        Expression arg = actual->nth(j);
        Symbol arg_type = typecheck_expression(arg, c);

        if (!is_subtype(arg_type, formal->get_type_decl())) {
          semant_error(c) << "In call of method " << method_name << ", type "
                          << arg_type << " of parameter " << formal->get_name()
                          << " does not conform to declared type "
                          << formal->get_type_decl() << "." << std::endl;
        }
      }

      break;
    }
  }

  if (!found) {
    semant_error(c) << "Dispatch to undefined method " << method_name << "."
                    << std::endl;
    return idtable.add_string("_no_type");
  }

  return return_type;
}

Symbol ClassTable::typecheck_cond(Expression pred, Expression then_exp,
                                  Expression else_exp, Class_ c) {
  Symbol pred_type = typecheck_expression(pred, c);
  if (pred_type != idtable.add_string("Bool")) {
    semant_error(c) << "Predicate of 'if' does not have type Bool."
                    << std::endl;
  }

  Symbol then_type = typecheck_expression(then_exp, c);
  Symbol else_type = typecheck_expression(else_exp, c);

  return lub(then_type, else_type);
}

Symbol ClassTable::typecheck_loop(Expression pred, Expressions body, Class_ c) {
  Symbol pred_type = typecheck_expression(pred, c);
  if (pred_type != idtable.add_string("Bool")) {
    semant_error(c) << "Loop condition does not have type Bool." << std::endl;
  }

  Symbol type = idtable.add_string("_no_type");
  for (int i = body->first(); body->more(i); i = body->next(i)) {
    type = typecheck_expression(body->nth(i), c);
  }
  return idtable.add_string("Object");
}

Symbol ClassTable::typecheck_typcase(Expression expr, Cases cases, Class_ c) {
  Symbol expr_type = typecheck_expression(expr, c);

  std::set<Symbol> case_types;
  Symbol return_type = idtable.add_string("_no_type");

  for (int i = cases->first(); cases->more(i); i = cases->next(i)) {
    Case case_branch = cases->nth(i);
    Symbol branch_type = case_branch->get_type_decl();

    if (classes.find(branch_type) == classes.end()) {
      semant_error(c) << "Class " << branch_type
                      << " of case branch is undefined." << std::endl;
      continue;
    }

    if (case_types.find(branch_type) != case_types.end()) {
      semant_error(c) << "Duplicate branch " << branch_type
                      << " in case statement." << std::endl;
      continue;
    }

    case_types.insert(branch_type);

    if (!is_subtype(expr_type, branch_type)) {
      semant_error(c) << "Expression type " << expr_type
                      << " does not conform to case branch type " << branch_type
                      << "." << std::endl;
    }

    object_env.enterscope();
    object_env.addid(case_branch->get_name(), branch_type);
    Symbol branch_expr_type = typecheck_expression(case_branch->get_expr(), c);
    object_env.exitscope();

    if (return_type == idtable.add_string("_no_type")) {
      return_type = branch_expr_type;
    } else {
      return_type = lub(return_type, branch_expr_type);
    }
  }

  return return_type;
}

Symbol ClassTable::typecheck_block(Expressions body, Class_ c) {
  Symbol type = idtable.add_string("_no_type");

  for (int i = body->first(); body->more(i); i = body->next(i)) {
    type = typecheck_expression(body->nth(i), c);
  }

  return type;
}

Symbol ClassTable::typecheck_let(Symbol identifier, Symbol type_decl,
                                 Expression init, Expressions body, Class_ c) {
  if (classes.find(type_decl) == classes.end()) {
    semant_error(c) << "Class " << type_decl << " of let-bound identifier "
                    << identifier << " is undefined." << std::endl;
    return idtable.add_string("_no_type");
  }

  object_env.enterscope();

  if (!init->is_no_expr()) {
    Symbol init_type = typecheck_expression(init, c);
    if (!is_subtype(init_type, type_decl)) {
      semant_error(c) << "Inferred type " << init_type
                      << " of initialization of " << identifier
                      << " does not conform to identifier's declared type "
                      << type_decl << "." << std::endl;
    }
  }

  object_env.addid(identifier, type_decl);
  Symbol type = idtable.add_string("_no_type");
  for (int i = body->first(); body->more(i); i = body->next(i)) {
    type = typecheck_expression(body->nth(i), c);
  }

  object_env.exitscope();
  return type;
}

Symbol ClassTable::typecheck_arithmetic(Expression e1, Expression e2,
                                        Class_ c) {
  Symbol type1 = typecheck_expression(e1, c);
  Symbol type2 = typecheck_expression(e2, c);

  if (type1 != idtable.add_string("Int") ||
      type2 != idtable.add_string("Int")) {
    semant_error(c) << "non-Int arguments: " << type1 << " + " << type2
                    << std::endl;
    return idtable.add_string("_no_type");
  }

  return idtable.add_string("Int");
}

Symbol ClassTable::typecheck_comparison(Expression e1, Expression e2,
                                        Class_ c) {
  Symbol type1 = typecheck_expression(e1, c);
  Symbol type2 = typecheck_expression(e2, c);

  if (type1 != idtable.add_string("Int") ||
      type2 != idtable.add_string("Int")) {
    semant_error(c) << "non-Int arguments: " << type1 << " < " << type2
                    << std::endl;
    return idtable.add_string("_no_type");
  }

  return idtable.add_string("Bool");
}

Symbol ClassTable::typecheck_eq(Expression e1, Expression e2, Class_ c) {
  Symbol type1 = typecheck_expression(e1, c);
  Symbol type2 = typecheck_expression(e2, c);

  if (type1 == idtable.add_string("Int") ||
      type1 == idtable.add_string("Bool") ||
      type2 == idtable.add_string("Int") ||
      type2 == idtable.add_string("Bool")) {
    if (type1 != type2) {
      semant_error(c) << "Illegal comparison with a basic type." << std::endl;
      return idtable.add_string("_no_type");
    }
  }

  return idtable.add_string("Bool");
}

Symbol ClassTable::typecheck_neg(Expression e1, Class_ c) {
  Symbol type = typecheck_expression(e1, c);

  if (type != idtable.add_string("Int")) {
    semant_error(c) << "Argument of '~' has type " << type << " instead of Int."
                    << std::endl;
    return idtable.add_string("_no_type");
  }

  return idtable.add_string("Int");
}

Symbol ClassTable::typecheck_comp(Expression e1, Class_ c) {
  Symbol type = typecheck_expression(e1, c);

  if (type != idtable.add_string("Bool")) {
    semant_error(c) << "Argument of 'not' has type " << type
                    << " instead of Bool." << std::endl;
    return idtable.add_string("_no_type");
  }

  return idtable.add_string("Bool");
}

Symbol ClassTable::typecheck_new(Symbol type_name, Class_ c) {
  if (classes.find(type_name) == classes.end()) {
    semant_error(c) << "'new' used with undefined class " << type_name << "."
                    << std::endl;
    return idtable.add_string("_no_type");
  }

  return type_name;
}

Symbol ClassTable::typecheck_isvoid(Expression e1, Class_ c) {
  typecheck_expression(e1, c);
  return idtable.add_string("Bool");
}

std::ostream& ClassTable::semant_error() {
  semant_errors++;
  return error_stream;
}

std::ostream& ClassTable::semant_error(Class_ c) {
  return semant_error(c->get_filename(), c);
}

std::ostream& ClassTable::semant_error(Symbol filename, tree_node* t) {
  error_stream << filename << ":" << t->get_line_number() << ": ";
  return semant_error();
}

std::ostream& ClassTable::semant_error(Expression e) {
  error_stream << curr_filename << ":" << e->get_line_number() << ": ";
  return semant_error();
}

std::ostream& ClassTable::semant_error(int line_number) {
  error_stream << curr_filename << ":" << line_number << ": ";
  return semant_error();
}

std::ostream& ClassTable::semant_error(Class_ c, Expression e) {
  return semant_error(c->get_filename(), e);
}

std::ostream& ClassTable::semant_error(Symbol filename, tree_node* t,
                                       Expression e) {
  error_stream << filename << ":" << t->get_line_number() << ": ";
  return semant_error();
}