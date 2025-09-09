#ifndef SEMANT_H
#define SEMANT_H

#include "cool-tree.h"
#include "stringtab.h"

#include <map>
#include <set>
#include <vector>

class Scope
{
  private:
    std::vector<std::map<std::string, Symbol>> var_scopes;

  public:
    Scope()
    {
        enter_scope();
    }

    void enter_scope()
    {
        var_scopes.push_back(std::map<std::string, Symbol>());
    }
    void exit_scope()
    {
        if (!var_scopes.empty())
            var_scopes.pop_back();
    }

    void add_variable(Symbol name, Symbol type)
    {
        if (!var_scopes.empty())
            var_scopes.back()[name->get_string()] = type;
    }

    bool variable_exists(Symbol name) const
    {
        for (auto it = var_scopes.rbegin(); it != var_scopes.rend(); ++it)
        {
            if (it->count(name->get_string()))
                return true;
        }
        return false;
    }

    Symbol get_variable_type(Symbol name) const
    {
        for (auto it = var_scopes.rbegin(); it != var_scopes.rend(); ++it)
        {
            auto found = it->find(name->get_string());
            if (found != it->end())
                return found->second;
        }
        return nullptr;
    }

    bool is_redeclared_in_current_scope(Symbol name) const
    {
        if (var_scopes.empty())
            return false;
        return var_scopes.back().count(name->get_string()) > 0;
    }
};

class SemanticAnalyzer
{
  private:
    Program program;
    Scope scope;
    std::map<Symbol, Class_> class_map;
    std::map<Symbol, Symbol> parent_map;

  public:
    SemanticAnalyzer(Program p) : program(p)
    {
    }
    void analyze();

  private:
    bool has_inheritance_cycle(Symbol class_name);
    void check_class(Class_ cls);
    void check_method_params(method_class *method, Class_ cls);
    void check_method_body(method_class *method, Class_ cls);
    Symbol type_check_expr(Expression expr, Class_ cls);
    bool is_subtype(Symbol child, Symbol parent);
};

#endif
