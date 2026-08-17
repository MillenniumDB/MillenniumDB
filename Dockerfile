# Build stage
FROM alpine:3.20 AS build
WORKDIR /mdb

# Automatically populated by Docker Buildx ('amd64' or 'arm64')
ARG TARGETARCH

RUN apk --no-cache add cmake \
                       make \
                       g++ \
                       openssl-dev \
                       boost1.82-dev \
                       icu-dev \
                       ncurses-dev

COPY src                               src
COPY CMakeLists.txt                    CMakeLists.txt
COPY third_party/antlr4-runtime-4.13.1 third_party/antlr4-runtime-4.13.1

# Set appropriate architecture flag depending on the target platform
RUN if [ "$TARGETARCH" = "amd64" ]; then \
        TARGET_ARCH="x86-64-v2"; \
    elif [ "$TARGETARCH" = "arm64" ]; then \
        TARGET_ARCH="armv8-a"; \
    else \
        TARGET_ARCH=""; \
    fi && \
    cmake -B build \
        -DCMAKE_BUILD_TYPE=Release \
        -DTARGET_ARCH="${TARGET_ARCH}" \
        -DCMAKE_INSTALL_PREFIX=./ && \
    cmake --build build -j $(($(getconf _NPROCESSORS_ONLN)-1)) --target install

COPY browser browser


# Final minimal stage (to minimize image size)
FROM alpine:3.20 AS final
WORKDIR /data

RUN apk --no-cache add libstdc++ \
                       libgcc \
                       openssl \
                       musl-locales \
                       libncursesw \
                       less \
                       icu \
                       bash

COPY --from=build /mdb/build/bin /usr/bin
COPY --from=build /mdb/browser /browser

VOLUME /data
ENV MDB_BROWSER=/browser
CMD ["bash"]
