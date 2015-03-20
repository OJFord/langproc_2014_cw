#eval `ssh-agent`
#ssh-add ~/.ssh/id_armrsa
scp -r -i ~/.ssh/id_armrsa -P 55555 ./src/* root@localhost:~/CARM/src/
scp -r -i ~/.ssh/id_armrsa -P 55555 ./Makefile root@localhost:~/CARM/
ssh -i ~/.ssh/id_armrsa -p 55555 root@localhost << ENDCMDS
cd ~/CARM
make build
ENDCMDS
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/lexer/Scanner.h ./src/lexer/Scanner.h
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/lexer/Scanner.ih ./src/lexer/Scanner.ih
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/lexer/Scannerbase.h ./src/lexer/Scannerbase.h
scp -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/lexer/lex.cc ./src/lexer/lex.cpp
