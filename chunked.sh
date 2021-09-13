#! /bin/bash
#testit.sh
#Arduino Telnet HTTP POST tests
header="POST /upload HTTP/1.1\n"
header+="Host: localhost 20000\n"
header+="Accept: */*\n"
header+="Transfer-Encoding: chunked\n"
thisfile="------\nContent-"
thisfile2="Disposition: form-data; name=\"file\"; filename=\"id.php\"\nContent-Type: text/php\n\n청크드 실험!!\n------\n"
thisfilelen=${#thisfile}
thisfilelen2=${#thisfile2}
printf -v hexlen '%x' $thisfilelen
printf -v hexlen2 '%x' $thisfilelen2
end="0"
echo "open localhost 20000"
sleep 2
echo -e $header
echo -e $hexlen
echo -e $thisfile
echo -e $hexlen2
echo -e $thisfile2
echo -e $end
sleep 1
header="POST /upload HTTP/1.1\n"
header+="Host: localhost 20000\n"
header+="Accept: */*\n"