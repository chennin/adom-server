#!/bin/bash
. /var/lib/adom/etc/config
trap '' INT

if [[ -f $HOME/donated ]]; then
        echo "Thanks for donating!"
        exit 0
fi

echo ""
echo "To prove that you donated to the ADOM  campaign, please"
echo "enter the details from the email you received."
echo ""
echo "If you don't have them, 1.2.0p3 is free to all, so try to change"
echo "versions again and enter '3'".
echo ""
read -p "ADOM prerelease download URL: " -e GURL
read -p "Username: " -e GUSER
read -s -p "Password (input will not be shown): " -e GPASSWORD
echo ""
if [[ "${GURL}" == *"${URL}"* ]] && [[ "${USER}" == "${GUSER}" ]] && [[ "${PASSWORD}" == "${GPASSWORD}" ]]; then
        echo "Details accepted."
        touch $HOME/donated
        sleep 2
else
        echo "Details incorrect."
        sleep 2
        exit 1
fi
exit 0
