
FROM ubuntu:20.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    tzdata \
    openssh-server \
    python3 \
    python3-pip \
    sudo && \
    ln -fs /usr/share/zoneinfo/Europe/Moscow /etc/localtime && \
    dpkg-reconfigure --frontend noninteractive tzdata



RUN mkdir /var/run/sshd
RUN echo 'root:password' | chpasswd


RUN sed -i 's/#PermitRootLogin prohibit-password/PermitRootLogin yes/' /etc/ssh/sshd_config


RUN ssh-keygen -A


EXPOSE 22


CMD ["/usr/sbin/sshd", "-D"]
