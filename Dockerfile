FROM alpine:3.18

RUN apk --no-cache add cmake make g++
RUN apk --no-cache add openssl-dev
RUN apk --no-cache add boost1.82-dev
RUN apk --no-cache add musl-locales

COPY ./src /MillenniumDB/src
COPY ./third_party/antlr4-runtime-4.9.3 /MillenniumDB/third_party/antlr4-runtime-4.9.3
COPY ./CMakeLists.txt /MillenniumDB/CMakeLists.txt

WORKDIR /MillenniumDB

RUN cmake -Bbuild/Release -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build/Release/ -j $(($(getconf _NPROCESSORS_ONLN)-1))

VOLUME ["/data"]
