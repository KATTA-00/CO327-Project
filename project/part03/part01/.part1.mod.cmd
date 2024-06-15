savedcmd_/media/sf_project/project/part03/part1.mod := printf '%s\n'   part1.o | awk '!x[$$0]++ { print("/media/sf_project/project/part03/"$$0) }' > /media/sf_project/project/part03/part1.mod
