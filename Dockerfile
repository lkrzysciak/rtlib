FROM debian:buster-slim

RUN apt update
RUN apt install -y g++ cmake git libboost-all-dev python3-pip gdb clang-format
RUN pip3 install matplotlib
