# syntax=docker/dockerfile:1
FROM ghcr.io/userver-framework/ubuntu-22.04-userver-pg-dev:v3.2

WORKDIR /app
COPY . .

RUN --mount=type=cache,target=/app/build-release,sharing=locked \
    cmake --preset release \
    && cmake --build build-release -j"$(nproc)" --target foodgram \
    && cp build-release/foodgram /usr/local/bin/foodgram

EXPOSE 8080

CMD ["foodgram", "--config", "configs/static_config.yaml", "--config-vars", "configs/config_vars.yaml"]
