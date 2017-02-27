@echo off
pscp -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" *.cpp ubuntu@34.251.220.117:/home/ubuntu/socketServer 
pscp -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" *.h ubuntu@34.251.220.117:/home/ubuntu/socketServer 
plink -i "C:\Users\Stephen\private_keys\ArgoTeamD.ppk" ubuntu@34.251.220.117 "cd socketServer/; ./build.sh"

