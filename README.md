# This is the branch of the project
# HTTP Server in C++

A lightweight HTTP server built using C++ and POSIX sockets, developed as part of the [CodeCrafters HTTP Server Challenge](https://app.codecrafters.io/courses/http-server/overview).

## Features
- **Concurrency**: Handles multiple client connections simultaneously using threads (`std::thread`).
- **Endpoint Handling**: Support for multiple endpoints with different response types.
- **File Serving**: Ability to serve and save files from/to a specified directory.
- **Header Parsing**: Case-insensitive HTTP header extraction.
- **Modern C++**: Built with the C++23 standard.

## Endpoints
The server handles the following endpoints:

- `GET /`: Returns a `200 OK` status with an empty body.
- `GET /tmp`: Returns a `200 OK` status (simple test endpoint).
- `GET /echo/{string}`: Returns the `{string}` in the response body with `text/plain` content type.
- `GET /user-agent`: Returns the value of the `User-Agent` header sent in the request.
- `GET /files/{filename}`: Retrieves the content of `{filename}` from the server's directory.
- `POST /files/{filename}`: Creates a new file named `{filename}` with the content from the request body.

## Prerequisites
- **Compiler**: A C++ compiler that supports C++23 (e.g., `g++` 12+ or `clang` 15+).
- **Build System**: [CMake](https://cmake.org/) (version 3.13 or higher).
- **Package Manager**: [vcpkg](https://vcpkg.io/) (for dependencies like `pthreads` and `zlib`).

## Building the Project

1. **Clone the repository**:
   ```bash
   git clone <https://github.com/irvingm45/HTTP_server.git>
   cd httpServer
   ```

2. **Configure with CMake**:
   Make sure you have `vcpkg` set up. If you are using the provided `vcpkg-configuration.json`, CMake should automatically detect it.
   ```bash
   cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
   ```

3. **Build**:
   ```bash
   cmake --build build
   ```

## Usage

Run the server by specifying the directory for file operations:

```bash
./build/http-server --directory /path/to/your/files
```

- `--directory` or `-d`: The base directory where the server will look for or save files.

The server will listen on port **4221**.

### Examples

**Echoing a string:**
```bash
curl -v http://localhost:4221/echo/hello-world
```

**Getting User-Agent:**
```bash
curl -v http://localhost:4221/user-agent
```

**Uploading a file:**
```bash
curl -v -X POST http://localhost:4221/files/example.txt -d "This is some file content"
```

**Downloading a file:**
```bash
curl -v http://localhost:4221/files/example.txt
```

## Technologies Used
- **C++23**
- **POSIX Sockets** (`sys/socket.h`, `arpa/inet.h`, etc.)
- **Threads** (`std::thread`)
- **CMake** & **vcpkg**
- **Zlib** (configured as dependency)
