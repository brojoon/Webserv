#! /bin/bash
siege -R <(echo connection = keep-alive) -c25 -b http://localhost:20000
