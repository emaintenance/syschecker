1 0 echo HOSTNAME ;0;Server_UP
1 0 CheckCPU HOSTNAME CPU 4,3,2 5,4,3
3 1 check_disk HOSTNAME Disques -e -x /boot -x /home -x /dev/shm 80 90
3 0 CheckSwap HOSTNAME Swap 80 90
9 1 ntpstat HOSTNAME NTP
