#eval `ssh-agent`
#ssh-add ~/.ssh/id_armrsa
scp -r -i ~/.ssh/id_armrsa -P 55555 ./src/* root@localhost:~/CARM/src/
ssh -p 55555 root@localhost << ENDCMDS
cd ~/CARM/src
make parser.o
ENDCMDS
scp -r -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/lexer/gen/* ./src/lexer/gen/
scp -r -i ~/.ssh/id_armrsa -P 55555 root@localhost:~/CARM/src/parser/gen/* ./src/parser/gen/
