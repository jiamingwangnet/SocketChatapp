# SocketChatapp

A thing I made that uses a webserver written in C++ that function as a chat app.

## How to use

The executable is built using Visual Studio.

Make sure the `frontend` folder is in the same directory as the executable.

The messsages will be stored in `data/data.txt` as plain text.

### Running the executable

The port and address can be specified either by using command line arguments or inputted after starting.

```
$ SocketChatapp 127.0.0.1 8080
```

or

```
$ SocketChatapp
Address: 127.0.0.1
Port: 8080
```

The webserver will be started on the specified IP address and port.

## Message format

The message format is `[name]:[message]`.
