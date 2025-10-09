FROM debian:trixie-slim

RUN apt update && apt install -y g++ cmake git libboost-all-dev python3-pip gdb clang-format python3-matplotlib ninja-build
