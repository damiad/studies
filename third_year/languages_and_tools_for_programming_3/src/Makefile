all: bootp_server bootp_client

bootp_server: bootp_server.ml
	ocamlc -o bootp_server unix.cma bootp_server.ml

bootp_client: bootp_client.ml
	ocamlc -o bootp_client unix.cma bootp_client.ml

clean:
	rm -f bootp_server bootp_client *.cmo *.cmi

.PHONY: all clean
