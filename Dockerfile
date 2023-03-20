# Build: docker build -t mdb.backend .

# Create Database:
# cd into the folder where the txt to be imported is (created db folder will be there too)
# docker run --volume "$(pwd)":/data mdb.backend /MillenniumDB/build/Release/bin/create_db /data/example-db.txt /data/docker_example

# Run server:
# docker run --volume "$(pwd)":/data --network="host" mdb.backend /MillenniumDB/build/Release/bin/server /data/docker_example

FROM alpine:3.14

RUN apk --no-cache add cmake make g++ boost-dev

COPY ./src /MillenniumDB/src
COPY ./third_party /MillenniumDB/third_party
COPY ./CMakeLists.txt /MillenniumDB/CMakeLists.txt

WORKDIR /MillenniumDB

RUN cmake -H. -Bbuild/Release -DCMAKE_BUILD_TYPE=Release
RUN cmake --build build/Release/

VOLUME ["/data"]
