@echo off
pscp -pw argo main.cpp argo@192.168.15.38:/home/argo/socketServer && plink argo@192.168.15.38 -pw argo "cd socketServer/; ./build.sh"

