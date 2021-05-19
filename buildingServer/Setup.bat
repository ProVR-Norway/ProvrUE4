@echo off

REM START THIS FILE FROM INSIDE PROVR PROJECT FILE


powershell -Command "& {docker pull ubuntu:bionic}"
powershell -Command "& {docker build . -t provr-server}" 

REM Multiple servers on same VM? change thhe name on the taged image on line 14-15 according to docker file

REM Uncomment line above if the image is to run locally
REM powershell -Command "& {docker run -p 7777:7777/udp --rm -it provr-server}"

powershell -Command "& {docker tag provr-server gcr.io/vr-collaboration-room/provr-server:latest}"
powershell -Command "& {docker push gcr.io/vr-collaboration-room/provr-server:latest}"


REM powershell -Command "& {powershell -Command “& {gcloud compute instances update-container provr-server          --container-image=gcr.io/vr-collaboration-room/provr-server:latest}”}"
