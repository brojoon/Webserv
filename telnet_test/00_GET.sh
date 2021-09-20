#! /bin/bash
#testit.sh
#Arduino Telnet HTTP POST tests
header="GET /index.html HTTP/1.1\n"
echo "open localhost 20000"
sleep 2
echo -e $header
sleep 1
