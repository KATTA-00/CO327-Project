savedcmd_/media/sf_project/part01/seconds/task02.ko := ld -r -m elf_x86_64 -z noexecstack --no-warn-rwx-segments --build-id=sha1  -T scripts/module.lds -o /media/sf_project/part01/seconds/task02.ko /media/sf_project/part01/seconds/task02.o /media/sf_project/part01/seconds/task02.mod.o