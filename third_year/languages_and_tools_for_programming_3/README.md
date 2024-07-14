# BOOTP Protocol Implementation Project

This project involves implementing a BOOTP (Bootstrap Protocol) client and server in a functional programming language. The client and server implementations are designed to work together, demonstrating the communication between the two.

## Task Description

The goal is to create implementations of both the BOOTP client and server that can successfully interact. The BOOTP protocol is used for assigning IP addresses to devices on a network, and your implementation will simulate this process.

### Objectives

1. **Implement BOOTP Server:**
    - The server should be capable of receiving requests from BOOTP clients and responding with appropriate IP address assignments.
  
2. **Implement BOOTP Client:**
    - The client should be able to send requests to the BOOTP server and handle the responses to obtain an IP address.

## Project Structure

- `bootp_client.ml`: Contains the implementation of the BOOTP client.
- `bootp_server.ml`: Contains the implementation of the BOOTP server.
- `Makefile`: Provides build instructions for compiling the client and server.

## Getting Started

### Prerequisites

Ensure you have the following installed on your system:
- OCaml compiler
- Make

### Building the Project

To compile the client and server, use the provided `Makefile`. Simply run:

```bash
make
```

This will produce two executables: `bootp_client` and `bootp_server`.

### Running the Server

Start the BOOTP server using the following command:

```bash
./bootp_server
```

### Running the Client

In a separate terminal, run the BOOTP client with:

```bash
./bootp_client
```

## Cleaning Up

To clean up the build artifacts, run:

```bash
make clean
```

## Usage

### BOOTP Server

The server listens for incoming BOOTP requests and responds with IP address assignments. Ensure the server is running before starting the client.

### BOOTP Client

The client sends a BOOTP request to the server and waits for a response. Upon receiving a response, it processes the assigned IP address.

## Additional Information

The implementations provided here are basic and meant for educational purposes. In a real-world scenario, BOOTP has been largely superseded by DHCP (Dynamic Host Configuration Protocol), which offers more features and flexibility.

For detailed information on the BOOTP protocol, refer to [RFC 951](https://tools.ietf.org/html/rfc951).
