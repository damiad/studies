# Internet Radio Transmitter and Receiver

This is the English translation of the first big assignment for the course "Computer Networks".

### Task Description
The task is to implement an internet radio transmitter and receiver. This task is an essential part of "Project 2," which will involve extending the functionality of the transmitter and receiver.

### Constants Used in the Assignment
The following constants are used in the assignment:

- `DEST_ADDR`: The receiver's address, set as a mandatory parameter `-a` of the transmitter.
- `DATA_PORT`: The UDP port used for data transmission, set as the `-P` parameter of both the transmitter and receiver. The default value is `20000 + (student_number % 10000)`.
- `PSIZE`: The packet size in bytes of the `audio_data` field, set as the `-p` parameter of the transmitter. The default value is 512B.
- `BSIZE`: The buffer size in bytes, set as the `-b` parameter of the receiver. The default value is 64kB (65536B).
- `NAZWA`: The name of the transmitter, set as the `-n` parameter. The default value is "Nienazwany Nadajnik" (Unnamed Transmitter).

### Part A (Transmitter)
The transmitter is responsible for sending a data stream received on standard input to the receiver. The transmitter should receive a data stream on standard input at the rate that the receiver can process the data and then send the data packed in UDP datagrams to the `DATA_PORT` on the specified `DEST_ADDR` address. The data should be transmitted in packets of size `PSIZE` bytes according to the protocol described below. The transmission speed matches the input data rate, and the transmitter does not interfere with it in any way.

After sending the entire content of the standard input, the transmitter exits with an exit code of 0. If the size of the read data is not divisible by `PSIZE`, the last (incomplete) packet is discarded and not sent.

### Transmitter Execution
For example, to send a favorite song in CD quality MP3 format, you can use the following command:
```
sox -S "05 Muzyczka.mp3" -r 44100 -b 16 -e signed-integer -c 2 -t raw - | pv -q -L \$((44100\*4)) | ./sikradio-sender -a 10.10.11.12 -n "Radio Muzyczka"
```
The first part of the command converts the MP3 file to a stream of raw data (44100 4-byte samples per second of the input file), and the second part limits the data transfer rate to the transmitter to match the receiver's processing capacity.

To send data from a microphone (in the same format as above), you can use the following command:
```
arecord -t raw -f cd | ./sikradio-sender -a 10.10.11.12 -n "Radio Podcast"
```
The `arecord` command can be found in the `alsa-utils` package.

### Part B (Receiver)
The receiver receives data sent by the transmitter and outputs it to the standard output.

The receiver has a buffer of size `BSIZE` bytes, which is used to store data from a maximum of ⌊BSIZE/PSIZE⌋ consecutive packets.

When starting playback, the receiver:
- Clears the buffer, discarding any data it contains but has not yet output.
- Connects to the transmitter on the `DATA_PORT`. The transmitter is set as the mandatory `-a` parameter of the receiver.
- Upon receiving the first audio packet, it saves the value of the `session_id` field and the number of the first received byte (let's call it `BYTE0`; see the protocol specification below).

Until it receives a byte with a number greater than or equal to `BYTE0 + ⌊BSIZE*3/4⌋`, the receiver does not output any data to the standard output. However, as soon as this condition is met, it starts outputting data to the standard output as fast as the standard output allows.

The above procedure should be applied wherever the assignment mentions starting playback.

If the receiver receives a new packet with a number greater than the previously received packet, it stores it in the buffer and reserves space for any missing packets that should be located before it. If removing old data that has not yet been output is necessary to make room for missing packets, it should be done.

The receiver outputs diagnostic information (stderr) regarding missing packets. Each time it receives a packet with number n, it checks the buffer for each i < n, where the receiver has not received packet i, even though there is space for it in the buffer. In this case, it outputs the following message:
```
MISSING: BEFORE n EXPECTED i
```
where `i` and `n` are the appropriate numbers. The messages are output in ascending order of `i`. The receiver does not output messages regarding packets containing bytes earlier than `BYTE0` or packets for which there is no space left in the buffer.

### Receiver Execution
```
./sikradio-receiver -a 10.10.11.12 | play -t raw -c 2 -r 44100 -b 16 -e signed-integer --buffer 32768 -
```
The `play` command can be found in the package that contains the `sox` program.

### Audio Data Transmission Protocol
- Data Exchange: Data exchange is done using UDP. The communication is unidirectional, with the transmitter sending audio packets and the receiver receiving them.
- Datagram Format: Binary data is transmitted in datagrams, according to the format described below.
- Byte Order: All numbers sent in the communication are in network byte order (big-endian).
- Audio Packet:
  - `uint64 session_id`
  - `uint64 first_byte_num`
  - `byte[] audio_data`

The `session_id` field remains constant throughout the transmitter's runtime. It is initialized with the number of seconds since the epoch at the beginning of the transmitter's execution.

The receiver remembers the `session_id` value from the first packet it receives after starting playback. If it receives a packet with:
- A smaller `session_id`, it ignores it.
- A larger `session_id`, it restarts the playback.

The bytes read by the transmitter from standard input are numbered starting from zero. The transmitter includes the number of the first byte among the bytes contained in `audio_data` in the `first_byte_num` field.

The transmitter sends packets in which the `audio_data` field contains exactly `PSIZE` bytes (and `first_byte_num` is divisible by `PSIZE`).

### Additional Guidelines
- Programs should support communication using IPv4. Support for IPv6 is not necessary.
- Large message queues, events, etc., should be allocated dynamically in program implementations.
- Programs should be resilient to error situations that allow them to continue running. The intention is for programs to be able to run continuously without the need for restarts, even in cases of communication issues, temporary network unavailability, or ordinary changes in network configuration, etc.
- Programs

should be written in a clear and understandable manner. Valuable guidelines on this matter can be found in the following link: [Linux Kernel Coding Style](https://www.kernel.org/doc/html/v5.6/process/coding-style.html).
- Please also refer to [this Stack Overflow post](https://stackoverflow.com/questions/4165174/when-does-a-udp-sendto-block).
- The audio playback should be smooth, without frequent or unexplained crackling noises.
- It is recommended to follow the Robustness Principle: [Robustness Principle - Wikipedia](https://en.wikipedia.org/wiki/Robustness_principle).
- When processing network binary data, use fixed-width types: [Integer types - cppreference.com](http://en.cppreference.com/w/c/types/integer).
- If invalid command-line arguments are received, programs should output an appropriate message to stderr and return an exit code of 1.

The assignment emphasizes the importance of reliability and error handling, as well as the need for clear and readable code. It provides guidelines for implementing the transmitter and receiver components of an internet radio system. Following the given instructions and considering the recommended practices should result in a functional and robust solution.