# Usamos a versão 'slim' do Debian 12 (Bookworm).
# É estável, segura e muito menor que a versão completa.
FROM debian:bookworm-slim

# Evita perguntas interativas durante a instalação (ex: fuso horário)
ENV DEBIAN_FRONTEND=noninteractive

# --- Instalação de Ferramentas ---
# 1. 'apt-get update' baixa a lista de pacotes.
# 2. Instalamos tudo com '--no-install-recommends' para evitar lixo desnecessário (docs, plugins extras).
# 3. 'rm -rf /var/lib/apt/lists/*' apaga o cache do apt no final para a imagem ficar pequena.
RUN apt-get update && apt-get install -y --no-install-recommends \
    git \
    python3 \
    python3-pip \
    build-essential \
    make \
    cmake \
    ninja-build \
    clang \
    clangd \
    # Toolchain ARM
    gcc-arm-none-eabi \
    libnewlib-arm-none-eabi \
    libstdc++-arm-none-eabi-newlib \
    # Debuggers e Ferramentas USB
    gdb-multiarch \
    openocd \
    usbutils \
    usbip \
    # Utilitários básicos
    ca-certificates \
    sudo \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# --- Configuração do Usuário ---
# Cria o usuário 'vscode' para evitar problemas de permissão com arquivos do Windows
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \
    && echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

# Define o usuário padrão
USER $USERNAME