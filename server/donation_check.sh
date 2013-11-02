#!/bin/bash
. /var/lib/adom/etc/config
trap '' INT

if [[ -f $HOME/donated ]]; then
        echo "Thanks for donating!"
        exit 0
fi

echo ""
echo "To prove that you donated to the ADOM Resurrection campaign, please"
echo "enter the details from the email you received."
echo ""
echo "If you don't have them, 1.2.0p${MAX_FREE_PRE} is free to all, so try to change"
echo "versions again and enter '${MAX_FREE_PRE}'".
echo ""
read -p "ADOM prerelease download URL: " -e GURL
read -p "Username: " -e GUSER
read -s -p "Password (input will not be shown): " -e GPASSWORD
echo ""
if [[ "${GURL}" == *"${URL}"* ]] && [[ "${DONUSER}" == "${GUSER}" ]] && [[ "${PASSWORD}" == "${GPASSWORD}" ]]; then
        echo "Details accepted."
        touch $HOME/donated
        sleep 2
else
        echo "Details incorrect."
        sleep 2
        exit 1
fi
exit 0
