#eval `ssh-agent`
#ssh-add ~/.ssh/id_armrsa
scp -r -i ~/.ssh/id_armrsa -P 55555 ./src/* root@localhost:~/CARM/src/
ssh -p 55555 root@localhost << ENDCMDS
cd ~/CARM/src
make lexer.o
ENDCMDS
