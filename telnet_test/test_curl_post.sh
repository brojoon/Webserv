#! /bin/bash
curl -X POST -H "Content-Type=plain/text" --data "id=webserv&password=123123" --resolve default_server:20000:127.0.0.1 http://127.0.0.1:20000/php
