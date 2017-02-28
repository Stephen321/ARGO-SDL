@echo off
REM pscp -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" *.cpp ubuntu@34.251.127.190:/home/ubuntu/socketServer 
REM pscp -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" *.h ubuntu@34.251.127.190:/home/ubuntu/socketServer 
REM plink -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" ubuntu@34.251.127.190 "cd socketServer/; ./build.sh"

pscp -P 2222 -pw argo *.cpp argo@127.0.0.1:/home/argo/socketServer 
pscp -P 2222 -pw argo *.h argo@127.0.0.1:/home/argo/socketServer 
plink -P 2222 -pw argo argo@127.0.0.1 "cd socketServer/; ./build.sh"

