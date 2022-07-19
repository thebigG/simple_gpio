FROM ubuntu:18.04
USER root

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y make cmake libx11-dev \
                                                   libxtst-dev \

RUN pip3 install cmake


RUN apt-get install -y clang-format-10
RUN ln -s  /usr/bin/clang-format-10 /usr/bin/clang-format
