#!/bin/bash
set +o noclobber
cd /var/lib/adom/tmp

. /var/lib/adom/etc/config

function elementIn () {
  local e
  for e in "${@:2}"; do [[ "$e" == "$1" ]] && return 0; done
  return 1
}

versions=('100' '111' 'lea' 'swp')
skips=(${SKIP_LIST});
for i in $(seq $MIN_PRE $MAX_PRE); do
	if [ $i -gt 23 -a $i -lt 48 ]; then continue; fi
        if ! elementIn "$i" "${skips[@]}"; then
		if [ $i -le 23 ]; then versions+=("120p$i"); 
		else versions+=("r$i"); fi
        fi
done
file=()

name=".HISCORE"
for v in ${versions[@]}; do
    pv=${v:4:4}
    if [[ ${pv} -gt 14 ]]; then name='HISCORE'; fi
    file=("${file[@]}" "/var/lib/adom/var/${v}/${name}")
done

update()
{
    if [ ! -f /var/lib/adom/bin/adom-$1-bin ]; then return; fi
    vers=$1
    if [ "$vers" == "120p14" ]; then 
        vers="120p14-hs"
        cp -p /var/lib/adom/var/$1/.HISCORE /var/lib/adom/var/$vers/.HISCORE
        if [ ! -f /var/lib/adom/bin/adom-$vers-bin ]; then return; fi
    fi
    /var/lib/adom/bin/adom-$vers-bin -S >/dev/null <<EOF

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
