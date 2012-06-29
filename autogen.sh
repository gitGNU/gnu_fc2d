#!/bin/sh

if test -e ./configure.ac
    then
    mkdir m4
    autoreconf --force --install && ./configure $@
    else
        echo "This directory does not seem appropriate to build GNU FC2D"
fi