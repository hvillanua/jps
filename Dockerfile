FROM ubuntu:latest
WORKDIR /app/

RUN apt-get update && apt-get install -y libboost-all-dev
COPY jps .
ENTRYPOINT ["./jps"]