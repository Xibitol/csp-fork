#!/bin/bash

# if [[ ! -d .venv/ ]]; then
# 	echo "You need to setup a Python Virtual Environment at .venv with \
# 		required packages specified in requirements.txt."
# 	exit 1
# fi

# #shellcheck source=../.venv/bin/activate
# source .venv/bin/activate

if [[ -d out/ ]]; then rm -r out/; fi
cmake -S . -B out/