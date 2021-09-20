#! /bin/bash
siege -R <(echo connection = keep-alive) -c10 -b http://localhost:20000
