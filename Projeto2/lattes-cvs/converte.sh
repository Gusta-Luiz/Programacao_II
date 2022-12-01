#!/bin/bash

for f in ./*/*.xml
do
  iconv -f ISO-8859-15 -t UTF-8 < $f > $f.new
  mv $f.new $f
done
