FROM ubuntu:18.04
USER root

ARG DEBIAN_FRONTEND=noninteractive

RUN add-apt-repository ppa:deadsnakes/ppa
RUN apt install python3.7

RUN apt-get update && apt-get install -y make cmake libx11-dev \
                                                   libxtst-dev \
                                                   python3-pip \
                                                   software-properties-common \


RUN pip3 install clang-format-all

RUN apt-get install -y clang-format-10
RUN ln -s  /usr/bin/clang-format-10 /usr/bin/clang-format
