@echo off
REM pscp -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" *.cpp ubuntu@34.250.8.240:/home/ubuntu/socketServer 
REM pscp -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" *.h ubuntu@34.250.8.240:/home/ubuntu/socketServer 
REM plink -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" ubuntu@34.250.8.240 "cd socketServer/; ./build.sh"

pscp -i "E:\ArgoTeamD.ppk" *.cpp ubuntu@34.250.8.240:/home/ubuntu/socketServer 
pscp -i "E:\ArgoTeamD.ppk" *.h ubuntu@34.250.8.240:/home/ubuntu/socketServer 
plink -i "E:\ArgoTeamD.ppk" ubuntu@34.250.8.240 "cd socketServer/; ./build.sh"

REM pscp -P 2222 -pw argo *.cpp argo@127.0.0.1:/home/argo/socketServer 
REM pscp -P 2222 -pw argo *.h argo@127.0.0.1:/home/argo/socketServer 
REM plink -P 2222 -pw argo argo@127.0.0.1 "cd socketServer/; ./build.sh"

