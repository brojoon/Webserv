#! /bin/bash
siege -R <(echo connection = keep-alive) -c66 -b http://localhost:20000
