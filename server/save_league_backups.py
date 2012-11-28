import os

date = (os.popen("date +%d-%m-%y-%H:%M:%S").readlines())[0].strip()
saves = os.environ["HOME"] + "/adom.data-111/savedg"

sums_raw = os.popen("md5sum " + os.environ["HOME"] + "/all_backups/*.svg").readlines()
sums = []

for sum in sums_raw:
    sums.append((sum.split()[0]).strip())

for (root,dirs,files) in os.walk(saves):
    for file in files:
        if ".svg" not in file: continue

        sum_raw = os.popen("md5sum " + root + "/" + file).readlines()[0]
        sum = (sum_raw.split()[0]).strip()

        if sum not in sums:
            os.system("cp " + root + "/" + file + " " + os.environ["HOME"] + "/all_backups/" + date + "_" + file)
