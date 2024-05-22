#include <opencv2/opencv.hpp>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;
using namespace cv;

int main() {
    // Socket Create
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        cerr << "Error creating socket" << endl;
        return -1;
    }

    // Socket Address
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(4545);

    // Socket Bind
    if (bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        cerr << "Error binding socket" << endl;
        return -1;
    }

    // Socket Listen
    if (listen(server_socket, 5) < 0) {
        cerr << "Error listening on socket" << endl;
        return -1;
    }
    cout << "LISTENING AT: 192.168.10.10:4545" << endl;

    // Socket Accept
    int client_socket;
    if ((client_socket = accept(server_socket, (struct sockaddr *) &client_address, &client_address_length)) < 0) {
        cerr << "Error accepting connection" << endl;
        return -1;
    }
    cout << "GOT CONNECTION FROM: " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << endl;

    // Video Capture
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error opening video capture" << endl;
        return -1;
    }

    while (true) {
        Mat frame;
        cap >> frame;
        if (frame.empty())
            break;

        // Resize the frame to lower the resolution
        resize(frame, frame, Size(500, 500));

        // Flip the frame horizontally
        flip(frame, frame, 1);

        // Encode the frame in JPEG format
        vector<uchar> buffer;
        imencode(".jpg", frame, buffer);

        // Pack the length of the frame and the frame itself
        uint64_t size = buffer.size();
        send(client_socket, &size, sizeof(size), 0);
        send(client_socket, buffer.data(), buffer.size(), 0);

        char key = waitKey(1);
        if (key == 'q') {
            close(client_socket);
            break;
        }
    }

    cap.release();
    close(server_socket);
    return 0;
}
