# http_server written in C/C++
A simple HTTP server written in C/C++ for learning about the HTTP
protocol and using dynamic memory allocation and deallocation. As
well as how to use Docker containers.

## Table of Contents
- [Installation](#installtion)
- [Usage](#usage)
- [License](#license)

## Installation
1. Clone the git repository:
```bash
git clone https://github.com/ErkanPerkan25/http_server.git
```
2. Install dependencies:

## Usage
### Build
To build and attach the Docker images run this command:
```bash
./scripts/docker_script.sh --pull
```
And then to run the container to run:
```bash
./scripts/docker_script.sh
```
You might have to use CMake in the repo before you run the container 
to compile the program:
```bash
cd build
cmake .
```
### Run
Within in the container go to the `build` directory:
```bash
cd build
```
And then run this command to start the server:
```bash
./http_server
```

## License
This project is licensed under the [MIT License](LICENSE).
