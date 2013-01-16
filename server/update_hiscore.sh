#!/bin/bash
set +o noclobber
cd /var/lib/adom/tmp

. /var/lib/adom/etc/config

versions=('100' '111' 'lea' 'swp')
for i in $(seq $MIN_PRE $MAX_PRE); do
	versions+=("120p$i")
done
file=()

for v in ${versions[@]}; do
    file=("${file[@]}" "/var/lib/adom/var/${v}/.HISCORE")
done

update()
{
    if [ ! -f /var/lib/adom/bin/adom-$1-bin ]; then return; fi
    /var/lib/adom/bin/adom-$1-bin -S >/dev/null <<EOF

EOF
    if [ ! -f hiscore.doc ]; then echo " hiscore.doc not found ?"; return; fi
    if ! cmp hiscore.doc /var/lib/adom/public_html/adom_hiscore/hiscore_v$1.txt
    then
        echo "Hiscore for $1 CHANGED."
	cat hiscore.doc > /var/lib/adom/public_html/adom_hiscore/hiscore_v$1.txt
    else
        echo "Hiscore for $1 not changed."
    fi
    rm -f hiscore.doc
}
inotifywait -e close_write -m ${file[@]} |
while read filename eventlist eventfile
do
        rawvers=$(basename $(dirname "$filename"))
        echo "$(date): ${rawvers} updating"
        update $rawvers
done
