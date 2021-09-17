#! /bin/bash
siege -R <(echo connection = keep-alive) -c50 -b http://localhost:20000
