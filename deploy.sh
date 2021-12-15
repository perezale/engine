#!/bin/bash
shopt -s expand_aliases
alias cssh='ssh -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null'
alias cscp='scp -o StrictHostKeyChecking=no -o UserKnownHostsFile=/dev/null'

curl -L -o engine.tar.gz $1 
tar -zxvf engine.tar.gz

cssh p12 "mkdir -p /opt/scanoss/engine/$2"
cscp scanoss p12:/opt/scanoss/engine/$2/scanoss