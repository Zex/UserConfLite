#!/bin/bash

RE_TEXT='^.*\(\(README\)\|\(\.\(py\|php\|html\|conf\)\)\)$'

for f in `find . -type f -regex $RE_TEXT`;do echo $f; sed 's/\r//g' $f > /tmp/$f; cp /tmp/$f $f;done
