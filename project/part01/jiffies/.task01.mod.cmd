savedcmd_/media/sf_project/part01/jiffies/task01.mod := printf '%s\n'   task01.o | awk '!x[$$0]++ { print("/media/sf_project/part01/jiffies/"$$0) }' > /media/sf_project/part01/jiffies/task01.mod
