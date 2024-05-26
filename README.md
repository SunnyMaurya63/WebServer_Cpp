## Basic Web Server in C++
This project implements a simple web server in C++ using the Winsock2 library. The server listens on 127.0.0.1:8080 and responds with a "Hello World" HTML message to any HTTP request.Not Node.js, not PHP, not Django or Flask, we will create a server from scratch using C++ and windows socket API.This should give you some idea how servers exactly work under the hood. Besides, it's a fun project!

# Features
- Listens on localhost (127.0.0.1) at port 8080
- Responds with a simple HTML message: <html><h1>Hello World</h1></html>
- Basic error handling for socket operations
  
# Requirements
- Windows operating system
- MinGW or a compatible C++ compiler
- Winsock2 library (usually included with Windows SDK)


# Clone the repository or copy the source code into a directory:
```terminal
  git clone https://github.com/yourusername/simple-web-server.git
```

# Compile the source code:
```terminal
  g++ -o web_server web_server.cpp -lws2_32
```

# Run the server:
```terminal
  ./web_server
```

## Documentation :For more information on the Winsock2 library ( https://learn.microsoft.com/en-us/windows/win32/api/winsock2/ )
