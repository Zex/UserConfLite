#!/bin/bash

RE_TEXT='^.*\(\(default\|README\|LICENSE\|MakeFile\)\|\(\.\(mk\|h\|cpp\|png\|bmp\|jpg\|php\|html\|conf\|Sample\|xls\|doc\|pdf\|css\)\)\)$'

find . -type f -iregex $RE_TEXT -exec echo {} \; -exec chmod a-x {} \;
