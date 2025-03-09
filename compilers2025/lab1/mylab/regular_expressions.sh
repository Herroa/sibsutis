RED='\033[31m'
END='\033[0m'

echo -e "${RED}IDENTIFICATORS:${END}"
grep -oE "\b[a-zA-Z_][a-zA-Z0-9_]*\b" *.cpp

echo -e "${RED}DEC:${END}"
grep -oE "\b[1-9][0-9'uUlL]*\b" *.cpp

echo -e "${RED}OCT:${END}"
grep -oE "\b0[0-7'uUlL]*\b" *.cpp

echo -e "${RED}HEX:${END}"
grep -oE "\b0[xX][0-9a-fA-F'uUlL]*\b" *.cpp

echo -e "${RED}INTEGER:${END}"
grep -oE "\b(0[bB][01'uUlL]*|0[0-7'uUlL]*|0[xX][0-9a-fA-F'uUlL]*|[1-9][0-9'uUlL]*)\b" *.cpp

echo -e "${RED}FLOAT:${END}"
grep -oE "\b[0-9']*\.?[0-9']+([eE][+-]?[0-9']+)?[fFlL]?\b|\b[0-9']+[eE][+-]?[0-9']+[fFlL]?\b|(^|[^0-9a-zA-Z_])\.[0-9']+([eE][+-]?[0-9']+)?[fFlL]?" *.cpp

echo -e "${RED}STRING:${END}"
grep -oE "\"([^\"]|\\.)*\"" *.cpp

echo -e "${RED}DIRECTIVES:${END}"
grep -oE "^\s*#(include|define|undef|if|ifdef|ifndef|else|elif|endif|line|error|pragma)" *.cpp
