#!/usr/bin/env python
import os
import ttyreclist_alphabetsort
import ttyreclist_mtimesort
import ttyreclist_sizesort

def write_list(ttyrecs, fd):
  for rec in ttyrecs:
    fd.write(rec[0] + " - " + rec[1] + "\n")

def list_recs():
  ttyrecs = []
	
  for root, dirs, files in os.walk("/var/lib/adom/users"):
    parts = root.split("/")
	  
    if len(parts) != 7 or parts[6] != "ttyrecs":
      continue
	 
    for file in files:
      if file != ".gitignore":
        ttyrecs.append((parts[5], file, os.path.getsize(root + "/" + file), 0, os.path.getmtime(root + "/" + file)))
          
  ttyrecs.sort(ttyreclist_alphabetsort.sortfunc)
  write_list(ttyrecs, open("/var/lib/adom/server/ttyrec_lists/alphasorted", "w"))

  ttyrecs.sort(ttyreclist_mtimesort.sortfunc)
  write_list(ttyrecs, open("/var/lib/adom/server/ttyrec_lists/mtimesorted", "w"))

  ttyrecs.sort(ttyreclist_sizesort.sortfunc)
  write_list(ttyrecs, open("/var/lib/adom/server/ttyrec_lists/sizesorted", "w"))

list_recs()
