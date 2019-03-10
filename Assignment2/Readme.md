Echo engine using socket

1. First run the server program (execute ./server in terminal).
2. Open another terminal and now execute the client program (./client1 127.0.0.1) send the ip address of server as command line argument.
3. client ask for message (Enter your message(s): enter character T)
4. send a chatacter 'T'.
5. server responds with lowercase letter i.e, 't'
6. Client receives the message from server and displays the message along with response time.(Received: tTime taken to receive message is 0.000097)
Similarly, execute other clients in other terminals(./client2 127.0.0.1,   ./client3 127.0.0.1,   ./client4 127.0.0.1,  ./client5 127.0.0.1)
and repeat steps from 3 to 6.

Performance data
    clents      timetaken
one client      0.000097
two clients     0.000101
three clients   0.000090
four clients    0.000087
five clients    0.000095


Database engine using socket
1. First run the server program (execute ./server in terminal).
2. Open another terminal and now execute the client program (./client1 127.0.0.1) send the ip address of server as command line argument.
3. client ask for filename (Enter the file name: address.txt)
4. send filename 'address.txt'.
5. server responds with the contents of file.
6. Client receives the message from server and displays the message along with response time.( this file contains address
26/C, 
Hosur Rd,
 Electronics City Phase 1, 
Electronic City, 
Bengaluru,
Karnataka 560100

Time taken to receive message is 0.000118
)
Similarly, execute other clients in other terminals(./client2 127.0.0.1,   ./client3 127.0.0.1,   ./client4 127.0.0.1,  ./client5 127.0.0.1)
and repeat steps from 3 to 6.

Performance data
    clents      timetaken
one client      0.000105
two clients     0.000110
three clients   0.000138
four clients    0.000122
five clients    0.000118

