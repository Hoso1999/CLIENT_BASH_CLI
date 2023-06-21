# CLIENT_SHELL_CLI
This project aims to create a server-client communication system in C, allowing clients to connect to the server and execute shell commands remotely.

## Features

- Establish a TCP/IP-based server-client communication.
- Authenticate clients connecting to the server.
- Execute shell commands on the server from the client-side.
- Handle simultaneous connections from multiple clients.
- Gracefully handle connect and disconnect events.

## Prerequisites

- C compiler (e.g., GCC)
- cmake
- Operating System that supports TCP/IP sockets (e.g., Linux, MACOS)

## Usage

Clone the repository:

   ```shell
   git clone https://github.com/your-username/server-client-communication.git
  ```
This project about create client-cli that can run shell commands in server and get output in client.
Compile with cmake

```bash
cmake -B BUILD_DIR && make -C BUILD_DIR
```
After build it generates server and client executables in BUILD_DIR/build folder

# Run

Run the server
```bash
server <port>
```

Run the client
```bash
client
```
## Client CLI implemented commands
### Connect to the server with ip addpress and port
```bash
connect <IP_ADDRESS> <PORT>
```
### Disconnect from server
```bash
disconnect
```
### Execute shell command in server
```bash
shell <COMMAND>
```

## Bugs
The shell command will send an error if the command expects standard input
## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.
