#ifndef COOLLEXER_H
#define COOLLEXER_H

#include <fstream>
#include <iostream>

#undef yyFlexLexer
#include <FlexLexer.h>

class CoolLexer : public yyFlexLexer {
public:
  CoolLexer(std::istream &arg_yyin, std::ostream &arg_yyout)
      : yyFlexLexer{arg_yyin, arg_yyout}, out{arg_yyout}, lineno{0},
        comment_level{0} {}
  virtual int yylex();

private:
  void Error(const char *msg) const;

  std::ostream &out;
  int lineno;
  int comment_level;
};

#endif
