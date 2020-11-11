FROM debian:buster-slim

RUN apt update
RUN apt install -y g++ cmake
