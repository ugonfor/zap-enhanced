#define _PATH_UTMP "/run/utmp"
#define _PATH_WTMP "/var/log/wtmp"
#define _PATH_LASTLOG "/var/log/lastlog"

void utmp_print(struct utmp *utmp_data);
void lastlog_print(struct lastlog *lastlog_data);