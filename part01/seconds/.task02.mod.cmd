savedcmd_/media/sf_project/part01/seconds/task02.mod := printf '%s\n'   task02.o | awk '!x[$$0]++ { print("/media/sf_project/part01/seconds/"$$0) }' > /media/sf_project/part01/seconds/task02.mod
