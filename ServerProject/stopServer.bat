@echo off
plink argo@192.168.15.38 -pw argo "cd socketServer/; ./stop.sh"

