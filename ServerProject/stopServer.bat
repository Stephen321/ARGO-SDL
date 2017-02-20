@echo off
plink -P 2222 argo@127.0.0.1 -pw argo "cd socketServer/; ./stop.sh"

