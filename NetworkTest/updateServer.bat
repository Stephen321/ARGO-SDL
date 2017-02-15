@echo off
pscp -P 2222 -pw argo main.cpp argo@127.0.0.1:/home/argo/socketServer && plink -P 2222 argo@127.0.0.1 -pw argo "cd socketServer/; ./build.sh"

