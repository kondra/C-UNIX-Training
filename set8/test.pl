#!/usr/bin/perl 

use strict;
use warnings;

system("bash -c ./gen");
system("bash -c ./hash > r1");
system("bash -c ./tree > r2");
system("bash -c \"diff r1 r2\"");
