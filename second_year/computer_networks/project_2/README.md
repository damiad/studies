# Internet Radio Transmitter and Receiver (Assignment 2)

This is the English translation of the second big assignment for the course "Computer Networks".

### Task Description
The task is to extend the functionality of the internet radio transmitter and receiver from assignment 1.

### Constants Used in the Assignment
The following constants are used in the assignment:

- `MCAST_ADDR`: The multicast address used for broadcasting, set as the mandatory parameter `-a` of the transmitter.
- `DISCOVER_ADDR`: The address used by the receiver to discover active transmitters, set as the `-d` parameter of the receiver. The default value is 255.255.255.255.
- `DATA_PORT`: The UDP port used for data transmission, set as the `-P` parameter of both the transmitter and receiver. The default value is `20000 + (student_number % 10000)`.
- `CTRL_PORT`: The UDP port used for control packet transmission, set as the `-C` parameter of both the transmitter and receiver. The default value is `30000 + (student_number % 10000)`.
- `UI_PORT`: The TCP port used for a simple text-based interface for switching between stations, set as the `-U` parameter of the receiver. The default value is `10000 + (student_number % 10000)`.
- `PSIZE`: The packet size in bytes of the `audio_data` field, set as the `-p` parameter of the transmitter. The default value is 512B.
- `BSIZE`: The buffer size in bytes, set as the `-b` parameter of the receiver. The default value is 64kB (65536B).
- `FSIZE`: The size in bytes of the transmitter's FIFO queue, set as the `-f` parameter of the transmitter. The default value is 128kB.
- `RTIME`: The time (in milliseconds) between sending consecutive reports about missing packets (for receivers) and the time between consecutive packet retransmissions, set as the `-R` parameter. The default value is 250.
- `NAME`: The name of the transmitter, set as the `-n` parameter. The default value is "Nienazwany Nadajnik" (Unnamed Transmitter).

### Part A (Transmitter)
The transmitter is responsible for sending a data stream received on standard input to the receivers. The transmitter should receive a data stream on standard input at the rate that the receivers can process the data and then send the data packed in UDP datagrams to the `DATA_PORT` on the multicast address `MCAST_ADDR`. The data should be transmitted in packets of size `PSIZE` bytes according to the protocol described below. The transmission speed matches the input data rate, and the transmitter does not interfere with it in any way.

The transmitter should keep a FIFO queue of the last `FSIZE` bytes read from the input so that it can retransmit packets requested by the receivers.

The transmitter continuously collects retransmission requests from the receivers. It collects them for a duration of `RTIME` and then sends a series of retransmissions (while still collecting requests for the next series). It repeats this process periodically.

The transmitter listens for UDP on the `CTRL_PORT`, accepting broadcast packets as well. It should recognize two types of messages:

- LOOKUP (identification requests): It immediately responds to these requests with a REPLY message according to the protocol specification below.
- REXMIT (packet retransmission requests): It does not respond directly to these requests. Instead, it periodically retransmits packets as described above.

After sending the entire content of the standard input, the transmitter exits with an exit code of 0. If the size of the read data is not divisible by `PSIZE`, the last (incomplete) packet is discarded and not sent.

### Transmitter Execution
For example, to send a favorite song in CD quality MP3 format, you can use the following command:
```
sox -S "05 Muzyczka.mp3" -r 44100 -b 16 -e signed-integer -c 2 -t raw - | pv -q -L \$((44100\*4)) | ./sikradio-sender -a 239.10.11.12 -n "Radio Muzyczka"
```
The first part of the command converts the MP3 file to a stream of raw data (44100 4-byte samples per second of the input file), and the second part limits the data transfer rate to the transmitter to match the receiver's processing capacity.

To send data from a microphone (in the same format as above), you can use the following command:
```
arecord -t raw -f cd | ./sikradio-sender -a 239.10.11.12 -n "Radio Podcast"
```
The `arecord` command can be found in the `alsa-utils` package.

### Part B (Receiver)
The receiver receives data sent by the transmitter and outputs it to the standard output.

The receiver periodically sends a LOOKUP message to the `DISCOVER_ADDR` on the `CTRL_PORT` to discover active transmitters. Based on the received REPLY messages, it creates a list of available radio stations. A station that hasn't sent a REPLY message for 20 seconds is removed from the list. If the currently playing station is removed, the receiver starts playing another station. If the `-n` parameter is provided, the receiver starts playing the station with the specified name as soon as it is discovered. If no `-n` argument is provided, the receiver starts playing the first discovered station.

The receiver has a buffer of size `BSIZE` bytes, which is used to store data from a maximum of ⌊BSIZE/PSIZE⌋ consecutive packets.

When starting playback, the receiver:
- Clears the buffer, discarding any data it contains but has not yet output.
- If necessary, leaves the previous multicast group address and joins the new one.
- After receiving the first audio packet, it saves the value of the `session_id` field and the number of the first received byte (let's call it `BYTE0`; see the protocol specification below) and starts sending retransmission requests as described below.

Until it receives a byte with a number greater than or equal to `BYTE0 + ⌊BSIZE*3/4⌋`, the receiver does not output any data to the standard output. However, as soon as this condition is met, it starts outputting data to the standard output as fast as the standard output allows.

The above procedure should be applied wherever the assignment mentions starting playback.

If the receiver is about to output data that is missing from the buffer, even if it's just a single packet, it restarts playback. NOTE: This requirement was not present in assignment 1.

If the receiver receives a new packet with a number greater than the previously received packet, it stores it in the buffer and reserves space for any missing packets that should be located before it. If removing old data that has not yet been output is necessary to make room for missing packets, it should be done.

The receiver sends retransmission requests for missing packets. It should send a retransmission request for packet number n at moments t + k * RTIME,

where t is the time of receiving the first packet with a number greater than n, and k = 1, 2, ... (indefinitely, as long as the station is on the list). The receiver does not send retransmission requests for packets containing bytes earlier than `BYTE0` or packets that are too old and won't fit in the buffer.

The receiver listens for TCP connections on the `UI_PORT`. If a user connects to that port, for example, using the Telnet program, they should see a simple text-based interface where they can navigate between stations using the up/down arrows (no need to press Enter). If there are multiple connections, all should display the same content, and changes made in one should be visible in the others. Similarly, the displayed station list should update if new stations are detected or existing ones become unavailable. The interface should look as follows:

```
------------------------------------------------------------------------

 SIK Radio

------------------------------------------------------------------------

PR1

Radio "357"

 > Radio "Disco Pruszkow"

------------------------------------------------------------------------
```
Stations should be sorted alphabetically by name. Each time there is a change in the state (active station, list of available stations, etc.), the complete list should be displayed again. This allows automatic testing of program functionality.

### Receiver Execution
```
./sikradio-receiver | play -t raw -c 2 -r 44100 -b 16 -e signed-integer --buffer 32768 -
```
The `play` command should be found in the package containing the `sox` program.

### Audio Data Transmission Protocol
Data exchange occurs using UDP. Communication is one-way: the transmitter sends audio packets, and the receiver receives them.

#### Datagram Format
Binary data is sent in datagrams according to the following format:

- `uint64 session_id`
- `uint64 first_byte_num`
- `byte[] audio_data`

The `session_id` field remains constant throughout the transmitter's runtime. At the beginning of its operation, it is initialized with the current time expressed in seconds since the epoch.

The receiver remembers the `session_id` value from the first packet it received after starting playback. If it receives a packet with:
- A smaller `session_id`, it ignores the packet.
- A larger `session_id`, it restarts playback.

The bytes read by the transmitter from standard input are numbered starting from zero. The transmitter includes the number of the first byte among the bytes contained in `audio_data` in the `first_byte_num` field.

The transmitter sends packets where the `audio_data` field has exactly `PSIZE` bytes (and `first_byte_num` is divisible by `PSIZE`).

### Control Protocol
Data exchange occurs using UDP.

Text data is sent in datagrams according to the following format:

- Each message is a single line of text terminated by a Unix newline character (`\n`).
- In addition to the newline character, only ASCII-encoded characters with numbers between 32 and 127 are allowed.
- Fields within a message are separated by single spaces. The last field (e.g., the station name in a REPLY message) may contain spaces.

The LOOKUP message is as follows:
```
ZERO_SEVEN_COME_IN
```

The REPLY message is as follows:
```
BOREWICZ_HERE [MCAST_ADDR] [DATA_PORT] [station_name]
```
The maximum length of the station name is 64 characters.

The REXMIT message is as follows:
```
LOUDER_PLEASE [comma-separated list of packet numbers]
```
The packet number corresponds to the value of its `first_byte_num` field, e.g.:
```
LOUDER_PLEASE 512,1024,1536,5632,3584
```

### Additional Specifications
- Programs must support communication using IPv4. IPv6 support is not required.
- Large message queues, event queues, etc., should be dynamically allocated in the implementation.
- Programs must be resilient to error conditions that allow them to continue running. The intention is for the programs to be able to run continuously without the need for restarting, even in cases of communication issues, temporary network unavailability, ordinary changes in network configuration, etc.
- Programs should be written in a clear and understandable manner. Valuable guidelines on this matter can be found in the following link: [Linux Kernel Coding Style](https://www.kernel.org/doc/html/v5.6/process/coding-style.html).
- In both applications, large queues, event queues, etc., should be allocated dynamically.
- Programs must be resilient to error conditions that allow them to continue running. The intention is for the programs to be able to run continuously without the need for restarting, even in cases of communication issues, temporary network unavailability, ordinary changes in network configuration, etc.
- Programs should be written in a clear and understandable manner. Valuable guidelines on this matter can be found in the following link: [Linux Kernel Coding Style](https://www.kernel.org/doc/html/v5.6/process/coding-style.html).
- In both applications, large queues, event queues, etc., should be allocated dynamically.
- Programs must be resilient to error conditions that allow them to continue running. The intention is for the programs to be able to run continuously without the need for restarting, even in cases of communication issues, temporary network unavailability, ordinary changes in network configuration, etc.
- Programs should be written in a clear and understandable manner. Valuable guidelines on this matter can be found in the following link: [Linux Kernel Coding Style](https://www.kernel.org/doc/html/v5.6/process/coding-style.html).
- When processing network binary data, use fixed-width types: [Integer types - cppreference.com](http://en.cppreference.com/w/c/types/integer).
- If invalid command-line arguments are received, programs should output an appropriate message to stderr and return an exit code of 1.

The assignment emphasizes the importance of reliability and error handling, as well as the need for clear and readable code. It provides guidelines for implementing the transmitter and receiver components of an internet radio system. Following the given instructions and considering the recommended practices should result in a functional and robust solution.