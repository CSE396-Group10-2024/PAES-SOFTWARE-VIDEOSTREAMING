PAES-SOFTWARE-VIDEOSTREAMING
Introduction

The PAES-SOFTWARE-VIDEOSTREAMING module is a part of the Patient Assistance and Entertainment System (PAES). This module sets up a video streaming server that captures video frames from a local camera and sends them to a connected client over a network. The server reduces the resolution of the video frames to ensure smooth streaming and encodes the frames in JPEG format.
Features

    Real-time video streaming from a webcam.
    Reduced resolution for efficient streaming.
    Horizontal flip of video frames.
    Adjustable frame rate.
    JPEG encoding of video frames.

Technologies Used

    Python
    OpenCV for video capture and frame processing.
    Socket Programming for network communication.

Setup Instructions
Prerequisites

    Python 3.x
    OpenCV library

Installation

    Clone the repository:

    sh

git clone https://github.com/your-repo/PAES-SOFTWARE-VIDEOSTREAMING.git
cd PAES-SOFTWARE-VIDEOSTREAMING

Install dependencies:

sh

    pip install opencv-python

    Configure the server IP address:
        Open the server.py file.
        Change the host_ip variable to the appropriate IP address where the server will run.

Usage Instructions
Running the Video Streaming Server

    Navigate to the directory containing the server.py file.
    Run the server:

    sh

    python server.py

    The server will start listening for incoming connections on the specified IP address and port.

Connecting a Client

    Ensure the client is on the same network as the server.
    Use a compatible client application to connect to the server's IP address and port.

Server Output:
Code Overview
server.py

python

import socket
import cv2
import struct
import time

def set_frame_rate(fps):
    """ Helper function to set the time interval between frames """
    return 1 / float(fps)

# Socket Create
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host_ip = '192.168.10.10'  # Change this to the appropriate IP address
port = 4545
socket_address = (host_ip, port)

# Socket Bind
server_socket.bind(socket_address)

# Socket Listen
server_socket.listen(5)
print("LISTENING AT:", socket_address)

# Frame rate control
fps = 10  # Frames per second
frame_interval = set_frame_rate(fps)
last_frame_time = time.time()

# Socket Accept
while True:
    client_socket, addr = server_socket.accept()
    print('GOT CONNECTION FROM:', addr)
    if client_socket:
        vid = cv2.VideoCapture(0)

        while vid.isOpened():
            ret, frame = vid.read()
            if not ret:
                break

            current_time = time.time()
            if (current_time - last_frame_time) >= frame_interval:
                # Resize the frame to lower the resolution
                frame = cv2.resize(frame, (320, 240))  # Reduced resolution
                
                # Flip the frame horizontally
                flipped_frame = cv2.flip(frame, 1)
                
                # Encode the frame in JPEG format with reduced quality
                _, buffer = cv2.imencode('.jpg', flipped_frame, [int(cv2.IMWRITE_JPEG_QUALITY), 80])
                frame_data = buffer.tobytes()
    
                # Pack the length of the frame and the frame itself
                message = struct.pack("Q", len(frame_data)) + frame_data
                # Send the packed message
                client_socket.sendall(message)
    
                last_frame_time = current_time  # Update the last frame time

    vid.release()
