# Zap-enhanced

** I did this as the assignment of class

In the case of intrusion into the server, it is important for hackers to find and delete all log-in traces related to them without being noticed. 

In the case of Linux, wtmp and lastlog are important files that keep access records, and zap is a program that deletes login traces left in these two files. 

However, since the published zap.c (http://cd.textfiles.com/hackersencyc/HACKING/UNIX/ZAP.C) deletes all records corresponding to username, suspicion may arise.



So I make **Zap-enhanced** which doesn't delete all login logs and have a more options.



## How to build

```
sudo make
```

** It must be run as sudo.

** Since the zap change the utmp/wtmp/lastlog which need the root permission to modify, zap need the 4777 permission. And then, make must be run as sudo.



## How to use

There are three functions on zap.



#### 1. Delete all login logs of attacker

```shell
$./zap -A <username>
```



The above command deletes all logs whose username is \<username\>.

##### check wtmp/utmp

```
ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ last
ugonfor  :0           :0               Tue May  4 23:38    gone - no logout
ugonfor- :0           :0               Tue May  4 23:37 - 23:38  (00:00)
ugonfor  :0           :0               Tue May  4 23:07 - 23:36  (00:29)
reboot   system boot  5.8.0-50-generic Tue May  4 23:06   still running
ugonfor  :0           :0               Tue May  4 01:18 - crash  (21:48)
reboot   system boot  5.8.0-50-generic Tue May  4 01:18   still running
ugonfor  :0           :0               Fri Apr 23 00:10 - crash (11+01:07)
reboot   system boot  5.8.0-50-generic Fri Apr 23 00:10   still running
ugonfor  :1           :1               Wed Apr 21 21:56 - crash (1+02:13)
ugonfor- :0           :0               Wed Apr 21 21:52 - crash (1+02:17)
reboot   system boot  5.8.0-50-generic Wed Apr 21 21:51   still running
ugonfor  :0           :0               Mon Apr 19 19:32 - crash (2+02:18)
reboot   system boot  5.8.0-50-generic Mon Apr 19 19:32   still running
ugonfor- pts/1        192.168.204.132  Mon Apr 19 11:01 - crash  (08:31)
ugonfor  :0           :0               Mon Apr 19 10:43 - crash  (08:48)
reboot   system boot  5.8.0-50-generic Mon Apr 19 06:40   still running
ugonfor  :0           :0               Sat Apr 17 07:49 - down   (03:12)
reboot   system boot  5.8.0-50-generic Sat Apr 17 07:49 - 11:02  (03:12)
ugonfor  :0           :0               Fri Apr 16 21:04 - crash  (10:44)
reboot   system boot  5.8.0-49-generic Fri Apr 16 21:03 - 11:02  (13:58)
ugonfor- pts/3        192.168.204.132  Wed Apr 14 09:41 - crash (2+11:21)
ugonfor  :0           :0               Wed Apr 14 06:41 - crash (2+14:21)
reboot   system boot  5.8.0-48-generic Wed Apr 14 06:31 - 11:02 (3+04:31)
ugonfor  :0           :0               Mon Apr 12 23:00 - crash (1+07:30)
reboot   system boot  5.8.0-48-generic Mon Apr 12 22:59 - 11:02 (4+12:02)
ugonfor  :0           :0               Mon Apr 12 11:43 - crash  (11:16)
reboot   system boot  5.8.0-48-generic Mon Apr 12 11:43 - 11:02 (4+23:18)
ugonfor  :0           :0               Wed Apr  7 22:29 - crash (4+13:13)
reboot   system boot  5.8.0-48-generic Wed Apr  7 22:28 - 11:02 (9+12:33)
ugonfor  :0           :0               Wed Apr  7 22:23 - down   (00:03)
reboot   system boot  5.8.0-48-generic Wed Apr  7 22:22 - 22:27  (00:05)
ugonfor  :0           :0               Tue Apr  6 00:03 - crash (1+22:18)
reboot   system boot  5.8.0-48-generic Tue Apr  6 00:02 - 22:27 (1+22:25)

wtmp begins Tue Apr  6 00:02:11 2021

============================================================================

ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ ./zap -A ugonfor
DONE!

============================================================================

ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ last
ugonfor- :0           :0               Tue May  4 23:37 - 23:38  (00:00)
reboot   system boot  5.8.0-50-generic Tue May  4 23:06   still running
reboot   system boot  5.8.0-50-generic Tue May  4 01:18   still running
reboot   system boot  5.8.0-50-generic Fri Apr 23 00:10   still running
ugonfor- :0           :0               Wed Apr 21 21:52 - crash (1+02:17)
reboot   system boot  5.8.0-50-generic Wed Apr 21 21:51   still running
reboot   system boot  5.8.0-50-generic Mon Apr 19 19:32   still running
ugonfor- pts/1        192.168.204.132  Mon Apr 19 11:01 - crash  (08:31)
reboot   system boot  5.8.0-50-generic Mon Apr 19 06:40   still running
reboot   system boot  5.8.0-50-generic Sat Apr 17 07:49 - 11:02  (03:12)
reboot   system boot  5.8.0-49-generic Fri Apr 16 21:03 - 11:02  (13:58)
ugonfor- pts/3        192.168.204.132  Wed Apr 14 09:41 - crash (2+11:21)
reboot   system boot  5.8.0-48-generic Wed Apr 14 06:31 - 11:02 (3+04:31)
reboot   system boot  5.8.0-48-generic Mon Apr 12 22:59 - 11:02 (4+12:02)
reboot   system boot  5.8.0-48-generic Mon Apr 12 11:43 - 11:02 (4+23:18)
reboot   system boot  5.8.0-48-generic Wed Apr  7 22:28 - 11:02 (9+12:33)
reboot   system boot  5.8.0-48-generic Wed Apr  7 22:22 - 22:27  (00:05)
reboot   system boot  5.8.0-48-generic Tue Apr  6 00:02 - 22:27 (1+22:25)

wtmp begins Tue Apr  6 00:02:11 2021

```

##### check lastlog

since the user "ugonfor" Never logged in, there are no changes on lastlog file.

```
ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ lastlog

Username         Port     From             Latest
root                                       **Never logged in**
daemon                                     **Never logged in**
bin                                        **Never logged in**
sys                                        **Never logged in**
sync                                       **Never logged in**

...

ugonfor                                    **Never logged in**
systemd-coredump                           **Never logged in**
sshd                                       **Never logged in**
ugonfor-ssh      pts/1    192.168.204.132  Mon Apr 19 11:01:11 -0700 2021

```



#### 2. Delete Certain logs.

```shell
$./zap -a <username> -t <tty> -d <mmddyy>
```

** mmddyy : mondth day year... ex, 2021.05.04 -> 050421

The above command deletes the log whose username is \<username\>, tty is \<tty\>  and time is \<mmddyy\>.



##### utmp/wtmp logs

```
ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ last
ugonfor  :0           :0               Tue May  4 23:38    gone - no logout

...

reboot   system boot  5.8.0-50-generic Mon Apr 19 19:32   still running
ugonfor- pts/1        192.168.204.132  Mon Apr 19 11:01 - crash  (08:31)
ugonfor  :0           :0               Mon Apr 19 10:43 - crash  (08:48)

...

ugonfor- pts/3        192.168.204.132  Wed Apr 14 09:41 - crash (2+11:21)
ugonfor  :0           :0               Wed Apr 14 06:41 - crash (2+14:21)

...

reboot   system boot  5.8.0-48-generic Tue Apr  6 00:02 - 22:27 (1+22:25)

wtmp begins Tue Apr  6 00:02:11 2021

============================================================================

ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ ./zap -a ugonfor-ssh -t pts/1 -d 041921
DONE!

============================================================================

ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ last
ugonfor  :0           :0               Tue May  4 23:38    gone - no logout

...

reboot   system boot  5.8.0-50-generic Mon Apr 19 19:32   still running
ugonfor  :0           :0               Mon Apr 19 10:43 - crash  (08:48)

...

ugonfor- pts/3        192.168.204.132  Wed Apr 14 09:41 - crash (2+11:21)
ugonfor  :0           :0               Wed Apr 14 06:41 - crash (2+14:21)

...

reboot   system boot  5.8.0-48-generic Tue Apr  6 00:02 - 22:27 (1+22:25)

wtmp begins Tue Apr  6 00:02:11 2021

```



##### lastlog

```
ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ lastlog

Username         Port     From             Latest
root                                       **Never logged in**
daemon                                     **Never logged in**
bin                                        **Never logged in**
sys                                        **Never logged in**
sync                                       **Never logged in**

...

ugonfor                                    **Never logged in**
systemd-coredump                           **Never logged in**
sshd                                       **Never logged in**
ugonfor-ssh      pts/1    192.168.204.132  Mon Apr 19 11:01:11 -0700 2021


before
=====================================================================
after

ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ lastlog

Username         Port     From             Latest
root                                       **Never logged in**
daemon                                     **Never logged in**
bin                                        **Never logged in**
sys                                        **Never logged in**
sync                                       **Never logged in**

...

ugonfor                                    **Never logged in**
systemd-coredump                           **Never logged in**
sshd                                       **Never logged in**
ugonfor-ssh      pts/3    192.168.204.132  Wed Apr 14 09:41:35 -0700 2021

```



#### 3. modify Certain logs.

```shell
$./zap -R <username1> <username2> -t <tty1> <tty2> -d <mmddyy1> <mmddyy2>
```

** mmddyy : mondth day year... ex, 2021.05.04 -> 050421

The above command modify the log whose username is \<username1\>, tty is \<tty1\>  and time is \<mmddyy1\> to \<username2\>, \<tty2\>, \<mmddyy2\>.



##### utmp/wtmp

```
ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ last

...

reboot   system boot  5.8.0-50-generic Mon Apr 19 19:32   still running
ugonfor- pts/1        192.168.204.132  Mon Apr 19 11:01 - crash  (08:31)
ugonfor  :0           :0               Mon Apr 19 10:43 - crash  (08:48)

...

wtmp begins Tue Apr  6 00:02:11 2021

============================================================================================

ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ ./zap -R ugonfor-ssh ugonfor -t pts/1 pts/4 -d 041921 052022
DONE!

============================================================================================

ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ last

...

reboot   system boot  5.8.0-50-generic Mon Apr 19 19:32   still running
ugonfor               192.168.204.132  Fri May 20 11:01 - crash (-395+15:28)
ugonfor  :0           :0               Mon Apr 19 10:43 - crash  (08:48)

...

wtmp begins Tue Apr  6 00:02:11 2021

```



##### lastlog

```
ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ lastlog

Username         Port     From             Latest
root                                       **Never logged in**
daemon                                     **Never logged in**
bin                                        **Never logged in**
sys                                        **Never logged in**
sync                                       **Never logged in**

...

ugonfor                                    **Never logged in**
systemd-coredump                           **Never logged in**
sshd                                       **Never logged in**
ugonfor-ssh      pts/1    192.168.204.132  Mon Apr 19 11:01:11 -0700 2021


before
=====================================================================
after

ugonfor@ubuntu:~/Desktop/zap/zap-enhanced$ lastlog

Username         Port     From             Latest
root                                       **Never logged in**
daemon                                     **Never logged in**
bin                                        **Never logged in**
sys                                        **Never logged in**
sync                                       **Never logged in**

...

ugonfor                   192.168.204.132  Fri May 20 11:01:11 -0700 2022
systemd-coredump                           **Never logged in**
sshd                                       **Never logged in**
ugonfor-ssh      pts/3    192.168.204.132  Wed Apr 14 09:41:35 -0700 2021

```

