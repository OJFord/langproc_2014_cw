#eval `ssh-agent`
#ssh-add ~/.ssh/id_armrsa
scp -r -i ~/.ssh/id_armrsa -P 55555 ./src/* root@localhost:~/CARM/src/
ssh -p 55555 root@localhost << ENDCMDS
cd ~/CARM/src
make CARM-Compiler.o
ENDCMDS
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/lexer/Scanner.h ./src/lexer/Scanner.h
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/lexer/Scanner.ih ./src/lexer/Scanner.ih
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/lexer/Scannerbase.h ./src/lexer/Scannerbase.h
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/lexer/lex.cc ./src/lexer/lex.cc
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/parser/Parser.h ./src/parser/Parser.h
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/parser/Parser.ih ./src/parser/Parser.ih
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/parser/Parserbase.h ./src/parser/Parserbase.h
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/parser/parse.cc ./src/parser/parse.cc
