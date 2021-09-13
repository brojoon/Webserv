#! /bin/bash
#testit.sh
#Arduino Telnet HTTP POST tests
header="DELETE /upload/test.php HTTP/1.1\n"
header+="Host: localhost 20000\n"
header+="Accept: */*\n"

echo "open localhost 20000"
sleep 2
echo -e $header
sleep 1
