import os
import ttyreclist_alphabetsort
import ttyreclist_mtimesort
import ttyreclist_sizesort

def write_list(ttyrecs, fd):
  for rec in ttyrecs:
    bytes = rec[2]

    if bytes < 1024: bytestr = str(bytes) + " bytes"
    elif bytes < 1024*1024: bytestr = str(bytes/1024) + " kilobytes"
    else: bytestr = str(bytes/1024/1024) + " megabytes"

    fd.write(rec[0] + " - " + rec[1] + " (" + bytestr + ")\n")

def list_recs():
  ttyrecs = []
	
  for root, dirs, files in os.walk("/var/lib/adom/users"):
    parts = root.split("/")
	  
    if len(parts) != 7 or parts[6] != "ttyrecs":
      continue
	 
    for file in files:
      ttyrecs.append((parts[5], file, os.path.getsize(root + "/" + file), 0, os.path.getmtime(root + "/" + file)))
          
  ttyrecs.sort(ttyreclist_alphabetsort.sortfunc)
  write_list(ttyrecs, open("/var/lib/adom/server/ttyrec_lists/alphasorted", "w"))

  ttyrecs.sort(ttyreclist_mtimesort.sortfunc)
  write_list(ttyrecs, open("/var/lib/adom/server/ttyrec_lists/mtimesorted", "w"))

  ttyrecs.sort(ttyreclist_sizesort.sortfunc)
  write_list(ttyrecs, open("/var/lib/adom/server/ttyrec_lists/sizesorted", "w"))

list_recs()
