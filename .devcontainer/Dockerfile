FROM debian:bookworm-slim as build

RUN apt-get update && apt-get install -y wget xz-utils

WORKDIR /tmp

RUN wget --progress=dot:giga https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz

# 3. EXTRAÇÃO BLINDADA (A Correção)
# Em vez de extrair e tentar mover (mv), criamos a pasta de destino e extraímos direto nela.
# --strip-components=1 remove a primeira pasta do arquivo compactado, jogando os binários direto em /opt/arm-gcc
RUN mkdir -p /opt/arm-gcc \
    && tar -xf arm-gnu-toolchain-13.2.rel1-x86_64-arm-none-eabi.tar.xz -C /opt/arm-gcc --strip-components=1

WORKDIR /opt/arm-gcc
RUN rm -rf share/doc share/man share/info \
    && rm -rf lib/gcc/arm-none-eabi/*/plugin

FROM debian:bookworm-slim

RUN apt-get update && apt-get install -y --no-install-recommends \
    git \
    cmake \
    build-essential \
    ninja-build \
    clang \
    clangd \
    # Debuggers e Ferramentas USB
    gdb-multiarch \
    openocd \
    usbutils \
    ncdu \
    tree \
    # Utilitários básicos
    ca-certificates \
    sudo \
    && apt-get clean && rm -rf /var/lib/apt/lists/*
    
    COPY --from=build /opt/arm-gcc /opt/arm-gcc

# --- Configuração do Usuário ---
# Cria o usuário 'vscode' para evitar problemas de permissão com arquivos do Windows
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME