#/bin/bash
if [ "$1" == "" ]; then
  echo "ERROR: no user"
  exit 2
fi

CONF=/var/lib/adom/server/adom_adduser.conf

SOFT=393216
HARD=786432 

echo -e "\033[1mYou MUST set a password, but your e-mail is OPTIONAL.\033[0m"
echo "If you provide an e-mail address, password recovery will be possible."

email=""
until [ ! "${email}" == "" ]; do
  read -e -p "Enter email address (optional): " email
  if [[ "${email}" == "" ]]; then email="invalid@example.com"; fi
  if [[ ! "${email}" =~ ^[0-9a-zA-Z.+-]+@[0-9a-zA-Z.-]+$ ]]; then
    echo "That email looks invalid, please try again"
    email=""
  fi
done


sudo adduser --conf $CONF --gecos ",${email},," --ingroup adomusers "$1" 
if [ $? != "0" ]; then echo "There was an error. Please try again."; sleep 5; exit 1; fi
sudo setquota -u "$1" "$SOFT" "$HARD" 0 0 /

echo ""
echo "NOTE: You have a $SOFT KB disk limit"
echo ""
echo "Your account was created. Please open a new SSH connection"
echo "and log in with your new username and password."
