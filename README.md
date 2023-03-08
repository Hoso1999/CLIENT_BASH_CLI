# CLIENT_SHELL_CLI

# Usage

```bash
cmake -B BUILD_DIR && make -C BUILD_DIR
```
After build it generates server and client executables in BUILD_DIR/build folder

# Run

server
```bash
server <port>
```

client
```bash
client
```
client has 3 commands
```bash
connect <IP_ADDRESS> <PORT>
```
```bash
disconnect
```
```bash
shell <COMMAND>
```

# Bugs
Shell command not works if command waits to stdin

