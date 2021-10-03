#! /bin/bash
siege -R <(echo connection = keep-alive) -c100 -b http://localhost:20000
