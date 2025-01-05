FROM ubuntu:latest

ENV MDB_HOME=/workspaces/MillenniumDB

RUN apt update && apt install -y git g++ cmake libssl-dev libncurses-dev locales less python3 python3-venv wget

RUN apt-get update && apt-get install -y build-essential


RUN wget -q --show-progress -O boost_1_82_0.tar.gz https://archives.boost.io/release/1.82.0/source/boost_1_82_0.tar.gz
RUN tar -xf boost_1_82_0.tar.gz && \
mkdir -p $MDB_HOME/third_party/boost_1_82/include && \
mv boost_1_82_0/boost $MDB_HOME/third_party/boost_1_82/include && \
rm -r boost_1_82_0.tar.gz boost_1_82_0


RUN git config --global user.email "matiutd888@gmail.com"
RUN git config --global user.name "Mateusz Nowakowski"


