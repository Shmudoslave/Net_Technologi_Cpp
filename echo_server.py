#!/usr/bin/env python3
import socketserver

class EchoHandler(socketserver.StreamRequestHandler):
    def handle(self):
        print(f"Client connected from {self.client_address}")
        data = self.rfile.read()
        print(f"Received {len(data)} bytes: {data}")
        self.wfile.write(data)
        print(f"Sent back {len(data)} bytes")
        self.request.close()

if __name__ == "__main__":
    with socketserver.TCPServer(('0.0.0.0', 7777), EchoHandler) as server:
        print("Echo server started on port 7777")
        server.serve_forever()