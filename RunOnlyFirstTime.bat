@echo off

REM START THIS FILE FROM INSIDE PROVR PROJECT FILE

powershell -Command "& {docker pull ubuntu:bionic}"
powershell -Command "& {docker build . -t provr-server}"
powershell -Command "& {docker run -p 7777:7777/udp --rm -it provr-server}"

REM powershell -Command "& {}"
REM open Powershell write following: 
REM docker run -p 7777:7777/udp --rm -it provr-server