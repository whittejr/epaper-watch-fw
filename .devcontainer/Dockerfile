# Use uma tag de versão específica em vez de 'latest' para evitar
# quebras de cache inesperadas quando a 'latest' for atualizada.
# Ex: archlinux:20240101 (verificar tags no Docker Hub)
# Mas, por enquanto, manteremos 'latest' para simplicidade.
FROM archlinux:latest

# --- Camada de Atualização ---
# Atualiza os repositórios de pacotes.
# Esta camada só será refeita se a imagem base (latest) mudar
# ou se você rodar o build com --no-cache.
RUN pacman -Syu --noconfirm

# --- Camada de Ferramentas ---
# Instala TODAS as dependências de uma vez e limpa o cache do pacman
# no *mesmo* comando RUN para manter a camada menor.
# Se você precisar adicionar outra ferramenta (ex: 'vim'), adicione
# na lista abaixo. Apenas ESTA camada será refeita.
RUN pacman -S --noconfirm \
    git \
    python \
    base-devel \
    make \
    cmake \
    ninja \
    clang \
    arm-none-eabi-gcc \
    arm-none-eabi-binutils \
    arm-none-eabi-newlib \
    arm-none-eabi-gdb \
    openocd \
    usbutils \
    usbip && \
    pacman -Scc --noconfirm

# --- Camada de Configuração do Usuário ---
# Esta camada raramente mudará e usará o cache das camadas anteriores.
RUN useradd -m -s /bin/bash vscode && \
    echo "vscode ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Define o usuário padrão para o restante do build e para
# quando o container for executado
USER vscode

# WORKDIR /home/vscode/project
# ENTRYPOINT ["..."]