FROM alpine AS build

RUN apk add --update alpine-sdk boost-dev cmake

ADD ./ggml /build/ggml

RUN mkdir /build/ggml/build

WORKDIR  /build/ggml/build

RUN cmake -D LLAMA_STATIC=ON ..
RUN make codegen-serve

FROM alpine AS runtime


WORKDIR /app

COPY --from=build /build/ggml/build/bin/codegen-serve /app/codegen-serve

ENV THREADS=4

ENV MODEL="/models/codegen-2B-multi-ggml-4bit-quant.bin"

COPY ./run.sh /app/

EXPOSE 18080

CMD /app/run.sh