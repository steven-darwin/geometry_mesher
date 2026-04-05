FROM ghcr.io/steven-darwin/scientific_computing/codebase:latest as preparation

WORKDIR /home/scientific_computing
RUN mkdir geometry_mesher
WORKDIR ./geometry_mesher
RUN mkdir lib

FROM scratch as dependency

# Resolve vcpkg package
WORKDIR /home/scientific_computing/geometry_mesher/lib
COPY ./lib/vcpkg.json ./lib/vcpkg-configuration.json ./
RUN vcpkg install

# Resolve OCCT library
WORKDIR /home/external
git clone https://github.com/Open-Cascade-SAS/OCCT.git
WORKDIR ./OCCT
RUN git checkout 36c80dbe7b199348e2f0a2d6932a462a37fd0b90

WORKDIR /home/scientific_computing/geometry_mesher/lib
RUN mkdir occt-7.9.1

FROM scratch as final

WORKDIR /home/scientific_computing/geometry_mesher
COPY ./CMakeLists.txt ./CMakePresets.json ./CMakeUserPresets.json ./
RUN mkdir include
COPY ./include ./include
RUN mkdir src
COPY ./src .src
RUN mkdir assets
COPY ./assets ./assets
RUN mkdir config
COPY ./config ./config
RUN mkdir scripts
COPY ./scripts ./scripts