#!/bin/fish

clear; make; and cat process.fltr | ./solver $argv[1]; feh test_result.png

