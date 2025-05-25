#include "semant.h"

#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>

extern int semant_errors;
extern int curr_lineno;
extern void semant_error(Class_ c, const char *msg);
extern void semant_error(const char *msg);

bool SemanticAnalyzer::is_subtype(Symbol child, Symbol parent) {
  if (child == parent) return true;
  if (parent == idtable.add_string("Object")) return true;

  Symbol curr = child;
  while (curr != idtable.add_string("Object")) {
    if (parent_map.find(curr) == parent_map.end()) break;
    curr = parent_map[curr];
    if (curr == parent) return true;
  }

  return false;
}

bool SemanticAnalyzer::has_inheritance_cycle(Symbol class_name) {
  std::set<Symbol> seen;
  Symbol current = class_name;
  while (current != idtable.add_string("Object")) {
    if (seen.count(current)) return true;
    seen.insert(current);

    if (parent_map.find(current) == parent_map.end()) return false;
    current = parent_map[current];
  }
  return false;
}

Symbol SemanticAnalyzer::type_check_expr(Expression expr, Class_ cls) {
  if (auto wh = dynamic_cast<loop_class *>(expr)) {
    Symbol cond_type = type_check_expr(wh->get_pred(), cls);
    Symbol body_type = type_check_expr(wh->get_body(), cls);

    if (cond_type != idtable.add_string("Bool")) {
      semant_error(cls, "Predicate of while must be of type Bool.");
    }

    return idtable.add_string("Object");
  }

  if (auto plus = dynamic_cast<plus_class *>(expr)) {
    Symbol t1 = type_check_expr(plus->get_e1(), cls);
    Symbol t2 = type_check_expr(plus->get_e2(), cls);

    if (t1 != idtable.add_string("Int") || t2 != idtable.add_string("Int")) {
      semant_error(cls, "Operands of '+' must be of type Int.");
      return idtable.add_string("Object");
    }

    return idtable.add_string("Int");
  }

  if (auto sub = dynamic_cast<sub_class *>(expr)) {
    Symbol t1 = type_check_expr(sub->get_e1(), cls);
    Symbol t2 = type_check_expr(sub->get_e2(), cls);

    if (t1 != idtable.add_string("Int") || t2 != idtable.add_string("Int")) {
      semant_error(cls, "Operands of '-' must be of type Int.");
      return idtable.add_string("Object");
    }

    return idtable.add_string("Int");
  }

  if (auto mul = dynamic_cast<mul_class *>(expr)) {
    Symbol t1 = type_check_expr(mul->get_e1(), cls);
    Symbol t2 = type_check_expr(mul->get_e2(), cls);

    if (t1 != idtable.add_string("Int") || t2 != idtable.add_string("Int")) {
      semant_error(cls, "Operands of '*' must be of type Int.");
      return idtable.add_string("Object");
    }

    return idtable.add_string("Int");
  }

  if (auto divide = dynamic_cast<divide_class *>(expr)) {
    Symbol t1 = type_check_expr(divide->get_e1(), cls);
    Symbol t2 = type_check_expr(divide->get_e2(), cls);

    if (t1 != idtable.add_string("Int") || t2 != idtable.add_string("Int")) {
      semant_error(cls, "Operands of '/' must be of type Int.");
      return idtable.add_string("Object");
    }

    return idtable.add_string("Int");
  }

  if (auto int_const = dynamic_cast<int_const_class *>(expr)) {
    return idtable.add_string("Int");
  }

  if (auto str_const = dynamic_cast<string_const_class *>(expr)) {
    return idtable.add_string("String");
  }

  if (auto bool_const = dynamic_cast<bool_const_class *>(expr)) {
    return idtable.add_string("Bool");
  }

  if (auto obj = dynamic_cast<object_class *>(expr)) {
    Symbol name = obj->get_variable_name();

    if (name == idtable.add_string("self")) {
      return idtable.add_string("SELF_TYPE");
    }

    if (!scope.variable_exists(name)) {
      semant_error(cls,
                   (std::string("Undeclared identifier: ") + name->get_string())
                       .c_str());
      return idtable.add_string("Object");
    }

    return scope.get_variable_type(name);
  }

  if (auto dispatch = dynamic_cast<dispatch_class *>(expr)) {
    Symbol expr_type = type_check_expr(dispatch->get_expr(), cls);
    Symbol method_name = dispatch->get_variable_name();
    Expressions args = dispatch->get_actual();

    Symbol class_to_check = expr_type == idtable.add_string("SELF_TYPE")
                                ? cls->get_name()
                                : expr_type;

    if (class_map.find(class_to_check) == class_map.end()) {
      semant_error(cls, (std::string("Dispatch on undefined class") +
                         class_to_check->get_string())
                            .c_str());
      return idtable.add_string("Object");
    }

    method_class *found_method = nullptr;
    Features features = class_map[class_to_check]->get_features();

    for (int i = features->first(); features->more(i); i = features->next(i)) {
      if (auto m = dynamic_cast<method_class *>(features->nth(i))) {
        if (m->get_name() == method_name) {
          found_method = m;
          break;
        }
      }
    }

    if (!found_method) {
      semant_error(cls, (std::string("Method ") + method_name->get_string() +
                         std::string(" not found in class ") +
                         class_to_check->get_string())
                            .c_str());
      return idtable.add_string("Object");
    }

    Formals formals = found_method->get_formals();
    if (formals->len() != args->len()) {
      semant_error(cls,
                   (std::string("Wrong number of arguments passed to method ") +
                    method_name->get_string())
                       .c_str());
      return idtable.add_string("Object");
    }

    for (int i = 0; i < args->len(); ++i) {
      Symbol arg_type = type_check_expr(args->nth(i), cls);
      Symbol expected_type = formals->nth(i)->get_type();
      if (!is_subtype(arg_type, expected_type)) {
        semant_error(
            cls,
            "Type of Argument does not conform to declared parameter type.");
      }
    }

    return found_method->get_return_type();
  }

  return idtable.add_string("Object");
}

void SemanticAnalyzer::check_method_body(method_class *method, Class_ cls) {
  scope.enter_scope();

  Features features = cls->get_features();
  for (int i = features->first(); features->more(i); i = features->next(i)) {
    if (auto attr = dynamic_cast<attr_class *>(features->nth(i))) {
      scope.add_variable(attr->get_name(), attr->get_type());
    }
  }

  Formals formals = method->get_formals();
  for (int i = formals->first(); formals->more(i); i = formals->next(i)) {
    Formal formal = formals->nth(i);
    Symbol name = formal->get_name();
    Symbol type = formal->get_type();

    if (scope.is_redeclared_in_current_scope(name)) {
      semant_error(cls, std::string("Parameter redeclares existing variable '" +
                                    std::string(name->get_string()) + "'.")
                            .c_str());
    }

    scope.add_variable(name, type);
  }

  Expression body = method->get_body();
  Symbol body_type = type_check_expr(body, cls);
  Symbol declared_return = method->get_return_type();

  if (!is_subtype(body_type, declared_return)) {
    semant_error(
        cls,
        "Return type of method body does not conform to declared return type.");
  }

  scope.exit_scope();
}

void SemanticAnalyzer::check_method_params(method_class *method, Class_ cls) {
  std::set<std::string> param_names;
  Formals formals = method->get_formals();

  for (int i = formals->first(); formals->more(i); i = formals->next(i)) {
    Formal formal = formals->nth(i);
    Symbol fname = formal->get_name();

    if (param_names.count(fname->get_string())) {
      semant_error(cls, ("Duplicate method parameter name '" +
                         std::string(fname->get_string()) + "'.")
                            .c_str());
    } else {
      param_names.insert(fname->get_string());
    }
  }
}

void SemanticAnalyzer::check_class(Class_ cls) {
  std::set<std::string> feature_names;
  Features features = cls->get_features();

  for (int i = features->first(); features->more(i); i = features->next(i)) {
    Feature f = features->nth(i);

    if (dynamic_cast<method_class *>(f)) {
      method_class *method = dynamic_cast<method_class *>(f);
      Symbol fname = method->get_name();

      if (feature_names.count(fname->get_string())) {
        semant_error(cls, ("Duplicate method name '" +
                           std::string(fname->get_string()) + "' in class: '" +
                           std::string(cls->get_name()->get_string()) + "'")
                              .c_str());
      } else {
        feature_names.insert(fname->get_string());
      }

      check_method_params(method, cls);
      check_method_body(method, cls);
    } else if (dynamic_cast<attr_class *>(f)) {
      attr_class *attr = dynamic_cast<attr_class *>(f);
      Symbol fname = attr->get_name();

      if (feature_names.count(fname->get_string())) {
        semant_error(cls, ("Duplicate attribute name '" +
                           std::string(fname->get_string()) + "' in class: '" +
                           std::string(cls->get_name()->get_string()) + "'")
                              .c_str());
      } else {
        feature_names.insert(fname->get_string());
      }
    }
  }
}

void SemanticAnalyzer::analyze() {
  Classes classes = program->get_classes();
  std::set<std::string> class_names;
  bool has_main = false;

  for (int i = classes->first(); classes->more(i); i = classes->next(i)) {
    Class_ cls = classes->nth(i);
    Symbol name = cls->get_name();

    if (class_names.count(name->get_string())) {
      semant_error(cls,
                   std::string("Class '" + std::string(name->get_string()) +
                               "' is defined more than once.")
                       .c_str());
      continue;
    } else {
      class_names.insert(name->get_string());
    }

    class_map[name] = cls;
    parent_map[name] = cls->get_parent();

    if (name == idtable.add_string("Main")) has_main = true;

    check_class(cls);
  }

  for (auto &[name, parent] : parent_map) {
    if (class_map.find(parent) == class_map.end() &&
        parent != idtable.add_string("Object")) {
      semant_error(
          class_map[name],
          std::string("Parent class of '" + std::string(name->get_string()) +
                      "' not defined.")
              .c_str());
    }
  }

  for (auto &[name, _] : class_map) {
    if (has_inheritance_cycle(name)) {
      semant_error(class_map[name],
                   std::string("Inheritance cycle detected at class '" +
                               std::string(name->get_string()) + "'.")
                       .c_str());
    }
  }

  if (!has_main) {
    semant_error("Class Main is not defined.");
  }
}
