#!/bin/bash

seq 1 1000 | xargs -P 10 -I {} sh -c "./client_uint32_t -H SHA1 -S c"
