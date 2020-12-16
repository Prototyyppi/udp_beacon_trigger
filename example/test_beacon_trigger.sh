#!/bin/bash

if [ ! -e ./wait_for_beacon ]; then
  echo "Put wait_for_beacon to this folder"
  exit 1
fi

echo "Now run send_beacon from a device connected to the same network"

while [ 1 ]; do
  ./wait_for_beacon
  echo "wait_for_beacon exited"
done

exit 0
