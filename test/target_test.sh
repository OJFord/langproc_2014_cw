# One observes that if you were to:
# 1. scp run_all.sh got/*.s and got/*.input to the ARM
# 2. ssh to the arm, and run ./run_all.sh
# 3. scp got/*.output and got/*.log back from the ARM
#then you might have a nice automated test framework

#One observes, one does
scp -i ../.arm_key -P 55555 -r run_all.sh ojf13@155.198.117.236:~/test/
scp -i ../.arm_key -P 55555 -r got/*.s ojf13@155.198.117.236:~/test/got/
ssh -i ../.arm_key -p 55555 ojf13@155.198.117.236 << ENDCMDS
cd ~/test
./run_all.sh
ENDCMDS
scp -i ../.arm_key -P 55555 ojf13@155.198.117.236:~/test/got/*.{output,log} got/
