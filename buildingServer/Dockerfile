FROM ubuntu:bionic

USER root

RUN useradd -ms /bin/bash ueuser

WORKDIR /opt/ProVR

COPY LinuxServer ./

RUN chown -R ueuser:ueuser ./
RUN chmod 755 ./

EXPOSE 7777/udp 
#Change port here if multiple serves is to be deployed on the same VM

USER ueuser

CMD ["./ProVRServer.sh", "-Log", "-Messaging"]