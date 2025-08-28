FROM gcc:latest

# set directory
WORKDIR /usr/local/http_server

# Installs cmake
RUN pacman -Syu && pacman -S cmake 

# Copies source code
COPY . .

# Set up a user to not run as root
RUN useradd app
USER app

# exposes the port
EXPOSE 8080
