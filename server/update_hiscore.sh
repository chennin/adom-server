#!/bin/bash
set +o noclobber
cd /var/lib/adom/tmp

. /var/lib/adom/etc/config

versions=('100' '111'  'etr' 'lea' 'swp')
for i in $(seq $MIN_PRE $MAX_PRE); do
	versions+=("120p$i")
done
file=()

for v in ${versions[@]}; do
    file=("${file[@]}" "/var/lib/adom/${v}/.HISCORE")
done

update()
{
    /var/lib/adom/bin/adom-$1-bin -S >/dev/null <<EOF

EOF

    if ! cmp hiscore.doc /var/lib/adom/public_html/adom_hiscore/hiscore_v$1.txt
    then
        echo "Hiscore for $1 CHANGED."
	cat hiscore.doc > /var/lib/adom/public_html/adom_hiscore/hiscore_v$1.txt
	rm hiscore.doc
    else
        echo "Hiscore for $1 not changed."
    fi
}
inotifywait -e close_write -m ${file[@]} |
while read filename eventlist eventfile
do
        rawvers=$(basename $(dirname "$filename"))
        echo "$(date): ${rawvers} updating"
        update $rawvers
done
