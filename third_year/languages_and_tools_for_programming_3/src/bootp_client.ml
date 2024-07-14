open Unix;;

type bootp_packet = {
  op: char;
  htype: char;
  hlen: char;
  hops: char;
  xid: int32;
  secs: int;
  flags: int;
  ciaddr: inet_addr;
  yiaddr: inet_addr;
  siaddr: inet_addr;
  giaddr: inet_addr;
  chaddr: string;
  sname: string;
  file: string;
  options: string;
}

let create_bootp_request () =
  {
    op = char_of_int 1; (* Request *)
    htype = char_of_int 1; (* Ethernet *)
    hlen = char_of_int 6;
    hops = char_of_int 0;
    xid = Random.int32 (Int32.max_int);
    secs = 0;
    flags = 0;
    ciaddr = inet_addr_of_string "0.0.0.0";
    yiaddr = inet_addr_of_string "0.0.0.0";
    siaddr = inet_addr_any;
    giaddr = inet_addr_of_string "0.0.0.0";
    chaddr = "\000\000\000\000\000\000"; (* MAC Address placeholder *)
    sname = "";
    file = "";
    options = "";
  }

let byte_to_int byte = Char.code byte

let bytes_to_ip_string bytes =
  let ip_parts = ref [] in
  for i = 0 to Bytes.length bytes - 1 do
    ip_parts := string_of_int (byte_to_int (Bytes.get bytes i)) :: !ip_parts
  done;
  String.concat "." (List.rev !ip_parts)

  
let bytes_to_int32 bytes =
  let open Int32 in
  let b0 = shift_left (of_int (Char.code (Bytes.get bytes 0))) 24 in
  let b1 = shift_left (of_int (Char.code (Bytes.get bytes 1))) 16 in
  let b2 = shift_left (of_int (Char.code (Bytes.get bytes 2))) 8 in
  let b3 = of_int (Char.code (Bytes.get bytes 3)) in
  logor (logor b0 b1) (logor b2 b3)

let bytes_to_int8 bytes =
  let open Int32 in
  of_int (Char.code (Bytes.get bytes 0))

let int32_to_bytes n =
  let open Int32 in
  let b0 = to_int (logand n 0xFFl) |> Char.chr in
  let b1 = to_int (logand (shift_right_logical n 8) 0xFFl) |> Char.chr in
  let b2 = to_int (logand (shift_right_logical n 16) 0xFFl) |> Char.chr in
  let b3 = to_int (logand (shift_right_logical n 24) 0xFFl) |> Char.chr in
  Bytes.of_string (String.make 1 b3 ^ String.make 1 b2 ^ String.make 1 b1 ^ String.make 1 b0)

let inet_addr_to_bytes addr =
  if addr = Unix.inet_addr_any then
    Bytes.of_string "\000\000\000\000"
  else
    let addr_str = Unix.string_of_inet_addr addr in
    let addr_parts = String.split_on_char '.' addr_str in
    let addr_bytes = List.map int_of_string addr_parts in
    let bytes = Bytes.create 4 in
    List.iteri (fun i b -> Bytes.set bytes i (char_of_int b)) addr_bytes;
    bytes

let serialize_bootp_packet packet =
  let buffer = Bytes.create 236 in
  Bytes.set buffer 0 packet.op;
  Bytes.set buffer 1 packet.htype;
  Bytes.set buffer 2 packet.hlen;
  Bytes.set buffer 3 packet.hops;
  
  let xid_bytes = int32_to_bytes packet.xid in
  Bytes.blit xid_bytes 0 buffer 4 4;

  let secs_bytes = Bytes.of_string (String.make 2 (Char.chr packet.secs)) in
  Bytes.blit secs_bytes 0 buffer 8 2;

  let flags_bytes = Bytes.of_string (String.make 2 (Char.chr packet.flags)) in
  Bytes.blit flags_bytes 0 buffer 10 2;

  let ciaddr_bytes = inet_addr_to_bytes packet.ciaddr in
  Bytes.blit ciaddr_bytes 0 buffer 12 4;

  let yiaddr_bytes = inet_addr_to_bytes packet.yiaddr in
  Bytes.blit yiaddr_bytes 0 buffer 16 4;

  let siaddr_bytes = inet_addr_to_bytes packet.siaddr in
  Bytes.blit siaddr_bytes 0 buffer 20 4;

  let giaddr_bytes = inet_addr_to_bytes packet.giaddr in
  Bytes.blit giaddr_bytes 0 buffer 24 4;

  let chaddr_bytes = Bytes.of_string (String.make 16 (Char.chr 0)) in
  Bytes.blit (Bytes.of_string packet.chaddr) 0 chaddr_bytes 0 (String.length packet.chaddr);
  Bytes.blit chaddr_bytes 0 buffer 28 16;
  
  let buf_zero pos = Bytes.set buffer (pos + 28+16) (Char.chr 0) in
  List.init (236-28-16) buf_zero;
  (* Fill sname, file i options *)
  (* ... *)

  buffer


let deserialize_bootp_response response =
  let op = byte_to_int (Bytes.get response 0) in
  let htype = byte_to_int (Bytes.get response 1) in
  let hlen = byte_to_int (Bytes.get response 2) in
  let hops = byte_to_int (Bytes.get response 3) in
  let xid = bytes_to_int32 (Bytes.sub response 4 4) in
  let secs = Bytes.get_uint16_be response 8 in
  let flags = Bytes.get_uint16_be response 10 in
  let ciaddr = Bytes.sub response 12 4 in
  let yiaddr = Bytes.sub response 16 4 in
  let siaddr = Bytes.sub response 20 4 in
  let giaddr = Bytes.sub response 24 4 in
  let chaddr = Bytes.sub response 28 16 in
  let sname = Bytes.sub response 44 64 in
  let file = Bytes.sub response 108 128 in
  (* let options = Bytes.sub response 236 (Bytes.length response - 236) in *)

  Printf.printf "op: %d, htype: %d, hlen: %d, hops: %d, xid: %ld, secs: %d, flags: %d\n"
    op htype hlen hops xid secs flags;
  Printf.printf "ciaddr: %s, yiaddr: %s, siaddr: %s, giaddr: %s\n"
    (bytes_to_ip_string ciaddr) (bytes_to_ip_string yiaddr) (bytes_to_ip_string siaddr) (bytes_to_ip_string giaddr);
  Printf.printf "chaddr: %s, sname: %s, file: %s\n"
    (Bytes.to_string chaddr) (Bytes.to_string sname) (Bytes.to_string file)
  (* Options can be handled differently *)

let print_bytes_as_hex bytes =
  for i = 0 to Bytes.length bytes - 1 do
    Printf.printf "%02x " (Char.code (Bytes.get bytes i))
  done;
  Printf.printf "\n"

let send_bootp_request () =
  let sock = socket PF_INET SOCK_DGRAM 0 in
  setsockopt sock SO_BROADCAST true;
  bind sock (ADDR_INET (inet_addr_any, 0));  (* Bind to a random free port *)

  let packet = create_bootp_request () in
  let packet_bytes = serialize_bootp_packet packet in
  let _ = sendto sock packet_bytes 0 (Bytes.length packet_bytes) [] (ADDR_INET (inet_addr_of_string "255.255.255.255", 67)) in
  Printf.printf "Sent BOOTP request\n";

  let response = Bytes.create 236 in
  let (num_bytes, _) = recvfrom sock response 0 236 [] in
  if num_bytes > 0 then begin
    Printf.printf "Received response:\n";
    (* print_bytes_as_hex response; *)
    deserialize_bootp_response response
  end else
    Printf.printf "No response received or error occurred\n"

let () = send_bootp_request ()
