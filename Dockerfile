FROM debian:buster-slim

RUN apt install cmake
RUN gcc -v
