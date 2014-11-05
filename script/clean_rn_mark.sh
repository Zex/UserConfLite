#!/bin/bash

RE_TEXT='^.*\(\(default\|README\|LICENSE\|MakeFile\)\|\(\.\(mk\|h\|cpp\|py\|php\|html\|conf\|Sample\|sql\|css\)\)\)$'

for f in `find . -type f -regex $RE_TEXT`;do echo $f; mkdir -p /tmp/`dirname $f`; sed 's/\r//g' $f > /tmp/$f; cp /tmp/$f $f;done
