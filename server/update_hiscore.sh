#!/bin/bash

cd /var/lib/adom/server

update()
{
    /var/lib/adom/bin/adom-$1-bin -S

    if ! cmp hiscore.doc /var/lib/adom/public_html/adom_hiscore/hiscore_$2.txt
    then
        echo "Hiscore for $2 CHANGED."
        mv hiscore.doc /var/lib/adom/public_html/adom_hiscore/hiscore_$2.txt
    else
        echo "Hiscore for $2 not changed."
    fi
}

while true
do
    update 100 v100
    update 111 v111
    update etr eternium_man
    update swp swapgame
    update lea leagues

    echo "Hiscore updated."

    sleep 60
done
