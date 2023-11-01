#!/bin/bash

echo "Install target \"eclipse\"? (Y/N)"
read buf

if [[ "$buf" != "N" && "$buf" != "n" ]]; then
	cd eclipse/ && sh install.sh && cd ..
fi

echo "Install target \"eclipse-get\"? (Y/N)"
read buf

if [[ "$buf" != "N" && "$buf" != "n" ]]; then
	cd eclipse-get/ && sh install.sh && cd ..
fi
