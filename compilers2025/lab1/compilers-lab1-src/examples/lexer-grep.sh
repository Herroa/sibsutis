#!/bin/sh

DIR="folly"

REGEXP="^int "

for f in `find $DIR -name "*.cpp"`; do
    echo "*** File $f"
    grep -E $REGEXP $f
done
