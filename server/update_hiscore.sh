#!/bin/bash
set +o noclobber
cd /var/lib/adom/tmp

versions=('100' '111' '120p3' '120p4' '120p5' 'etr' 'lea' 'swp')
file=()

for v in ${versions[@]}; do
    file=("${file[@]}" "/var/lib/adom/${v}/.HISCORE")
done

update()
{
    /var/lib/adom/bin/adom-$1-bin -S >/dev/null <<EOF

EOF

    if ! cmp hiscore.doc /var/lib/adom/public_html/adom_hiscore/hiscore_$1.txt
    then
        echo "Hiscore for $1 CHANGED."
        mv hiscore.doc /var/lib/adom/public_html/adom_hiscore/hiscore_$1.txt
    else
        echo "Hiscore for $1 not changed."
    fi
}

inotifywait -e modify -m ${file[@]} |
while read filename eventlist eventfile
do
        rawvers=$(basename $(dirname "$filename"))
        echo "$(date): ${rawvers} updating"
        update $rawvers
done
