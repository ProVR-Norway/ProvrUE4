FROM ubuntu:bionic

USER root

RUN useradd -ms /bin/bash ueuser

WORKDIR /opt/ProVR

COPY LinuxServer ./

RUN chown -R ueuser:ueuser ./
RUN chmod 755 ./

EXPOSE 7777/udp

USER ueuser

CMD ["./ProVRServer.sh", "-Log", "-Messaging"]