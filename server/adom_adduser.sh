#/bin/bash

CONF=/var/lib/adom/server/adom_adduser.conf

sudo adduser --conf $CONF "$1" 

if [ $? == "0" ]; then
	echo "Your account was created. Please open a new SSH connection"
	echo "and log in with your new username and password."
else
	echo "There was an error. Please try again."
fi
