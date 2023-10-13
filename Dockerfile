FROM debian:buster-slim

RUN apt update && apt install -y g++ cmake git libboost-all-dev python3-pip gdb clang-format
RUN apt install -y python3-matplotlib
