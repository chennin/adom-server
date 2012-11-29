#/bin/bash

CONF=/var/lib/adom/server/adom_adduser.conf

SOFT=393216
HARD=786432 

sudo adduser --conf $CONF --ingroup adomusers "$1" 
sudo setquota -u "$1" "$SOFT" "$HARD" 0 0 /

if [ $? == "0" ]; then
        echo ""
        echo "NOTE: You have a $SOFT KB disk limit"
        echo ""
        echo ""
	echo "Your account was created. Please open a new SSH connection"
	echo "and log in with your new username and password."
else
	echo "There was an error. Please try again."
fi
