#! /bin/bash
#testit.sh
#Arduino Telnet HTTP POST tests
header="GET /0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789 HTTP/1.1\n"

echo "open localhost 20000"
sleep 2
echo -e $header
sleep 1
