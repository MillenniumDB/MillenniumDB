# Build stage
FROM alpine:3.18 AS build
WORKDIR /mdb

RUN apk --no-cache add cmake \
                       make \
                       g++ \
                       openssl-dev \
                       boost1.82-dev \
                       ncurses-dev

COPY src                               src
COPY CMakeLists.txt                    CMakeLists.txt
COPY third_party/antlr4-runtime-4.13.1 third_party/antlr4-runtime-4.13.1

RUN cmake -B build -D CMAKE_BUILD_TYPE=Release -D CMAKE_INSTALL_PREFIX=./ && \
    cmake --build build -j $(($(getconf _NPROCESSORS_ONLN)-1)) --target install

COPY browser browser


# Final minimal stage (to minimize image size)
FROM alpine:3.18 AS final
WORKDIR /data

RUN apk --no-cache add libstdc++ \
                       libgcc \
                       openssl \
                       musl-locales \
                       libncursesw \
                       less \
                       bash

COPY --from=build /mdb/build/bin /usr/bin
COPY --from=build /mdb/browser /browser

VOLUME /data
ENV MDB_BROWSER=/browser
CMD ["bash"]
