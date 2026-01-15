# UDP Packet Transmitter

A client-server application demonstrating UDP socket programming with custom packet serialization in C++ using Winsock.

## Overview

This project implements a UDP-based communication system where a client reads text data from a file, encapsulates it into custom packets, and transmits them to a server over UDP protocol. The server receives and processes these packets.

## Architecture

### Client
- Reads lines from `InputFile.txt`
- Wraps each line in a custom packet format
- Sends packets via UDP to localhost on port 27000
- Uses Winsock for network communication

### Server
- Listens on UDP port 27000
- Receives incoming packets from clients
- Deserializes and processes packet data
- Displays received information

## Packet Structure

Each packet consists of three main components:

**Header (2 bytes)**
- Source ID: 4 bits - Identifies the packet source
- Line Number: 4 bits - Tracks the line number from input file
- Length: 1 byte - Number of characters in the data field

**Data (Variable length)**
- Contains the actual text content from the input file
- Length determined by the header's Length field

**CRC (4 bytes)**
- Cyclic Redundancy Check for data integrity
- Currently set to 0xFF00FF00

## Requirements

- Windows operating system
- Visual Studio 2019 or later
- Winsock2 library (Ws2_32.lib)

## Building the Project

1. Open `UDP-Packet-Transmitter.sln` in Visual Studio
2. Build the solution (F7)
3. Both Client and Server executables will be generated in the `x64/Debug` or `x64/Release` directory

## Usage

1. Create an `InputFile.txt` in the Client directory with text content (one line per packet)
2. Run the Server executable first
3. Run the Client executable
4. The server will display received packets with their details

## Project Structure

```
UDP-Packet-Transmitter/
├── Client/
│   ├── client.cpp          # Client implementation
│   ├── Packet.h            # Packet class definition
│   ├── Client.vcxproj      # Client project file
│   └── InputFile.txt       # Input data file
├── Server/
│   ├── server.cpp          # Server implementation
│   └── Server.vcxproj      # Server project file
└── UDP-Packet-Transmitter.sln  # Visual Studio solution file
```

## Technical Details

### Packet Serialization
The `Packet` class handles serialization and deserialization:
- `SetData()`: Stores data and updates header
- `SerializeData()`: Converts packet to byte stream for transmission
- `CalculateCRC()`: Computes checksum for integrity verification
- Constructor with `char*`: Deserializes received byte stream back into packet structure

### Network Communication
- Protocol: UDP (User Datagram Protocol)
- Port: 27000
- Address: 127.0.0.1 (localhost)
- Socket Type: SOCK_DGRAM

## Notes

- This is a demonstration project for educational purposes
- UDP does not guarantee packet delivery or ordering
- The CRC implementation is simplified for demonstration
- Designed for Windows environment using Winsock API
