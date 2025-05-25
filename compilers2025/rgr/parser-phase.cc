#include <unistd.h>

#include <cstdio>

#include "cool-parse.h"
#include "cool-tree.h"
#include "semant.h"
#include "utilities.h"

std::FILE *token_file = stdin;
extern Classes parse_results;
extern Program ast_root;

extern int curr_lineno;
const char *curr_filename = "<stdin>";
extern int parse_errors;

// Debug flags
// int yy_flex_debug = 0;
extern int cool_yydebug;
int lex_verbose = 0;

int semant_errors = 0;

void semant_error(Class_ c, const char *msg) {
  std::cerr << c->get_filename() << ":" << c->get_line_number() << ": " << msg
            << std::endl;
  semant_errors++;
}

void semant_error(const char *err) {
  std::cerr << err << std::endl;
  semant_errors++;
}

void semant_error(Class_ c, Expression e, const char *msg) {
  std::cerr << c->get_filename() << ":" << e->get_line_number() << ": " << msg
            << std::endl;
  semant_errors++;
}

extern int cool_yyparse();

int main(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    token_file = std::fopen(argv[i], "r");
    if (token_file == NULL) {
      std::cerr << "Error: can not open file " << argv[i] << std::endl;
      std::exit(1);
    }
    curr_filename = argv[i];
    curr_lineno = 1;

    cool_yyparse();
    if (parse_errors != 0) {
      std::cerr << "Error: parse errors\n";
      std::exit(1);
    }

    std::cerr << "\n Print tree\n";
    ast_root->dump_with_types(std::cerr, 1);
    std::cerr << "\n Print idteble\n";
    idtable.print();
    std::cerr << "\n Print inttable\n";
    inttable.print();
    std::cerr << "\n Print stringtable\n";
    stringtable.print();

    std::cout << "\nSemantic analyze:\n";
    SemanticAnalyzer analyzer(ast_root);
    analyzer.analyze();
    if (semant_errors) {
      std::cerr << "Semantic analyzer were found " << semant_errors
                << " semantic errors!!\n";
    }
    std::fclose(token_file);
  }

  return 0;
}
