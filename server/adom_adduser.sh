#/bin/bash

CONF=/var/lib/adom/server/adom_adduser.conf

SOFT=393216
HARD=786432 

echo -e "\033[1mYou MUST set a password, but all information after that is OPTIONAL.\033[0m"

sudo adduser --conf $CONF --ingroup adomusers "$1" 
if [ $? != "0" ]; then echo "There was an error. Please try again."; sleep 5; exit 1; fi
sudo setquota -u "$1" "$SOFT" "$HARD" 0 0 /
if [ $? != "0" ]; then echo "There was an error. Please try again."; sleep 5; exit 1; fi

echo ""
echo "NOTE: You have a $SOFT KB disk limit"
echo ""
echo ""
echo "Your account was created. Please open a new SSH connection"
echo "and log in with your new username and password."
