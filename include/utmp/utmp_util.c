#include <stdio.h>
#include <utmp.h>

void utmp_print(struct utmp *utmp_data){
    printf("type : %d\t", utmp_data->ut_type);
    printf("ut_pid : %d\t", utmp_data->ut_pid);
    printf("ut_line : %s\t", utmp_data->ut_line);
    printf("ut_id : %s\t", utmp_data->ut_id);
    printf("ut_user : %s\t", utmp_data->ut_user);
    printf("ut_host : %s\t", utmp_data->ut_host);
    //printf("ut_exit : %d %d\t", utmp_data->ut_exit.e_exit, utmp_data->ut_exit.e_termination);
    //printf("ut_session : %d\t", utmp_data->ut_session);
    printf("tv_sec : %d\t", utmp_data->ut_tv.tv_sec);
    printf("addr_v6 :  %u.%u.%u.%u\n", utmp_data->ut_addr_v6[0], utmp_data->ut_addr_v6[1], utmp_data->ut_addr_v6[2], utmp_data->ut_addr_v6[3]);
    return ;
}


void lastlog_print(struct lastlog *lastlog_data){
    printf("ll_host: %s\t", lastlog_data->ll_host);
    printf("ll_line: %s\t", lastlog_data->ll_line);
    printf("ll_time: %d\n", lastlog_data->ll_time);
    return ;
}

/*

for test the function

int main(int argc, char const *argv[])
{
    FILE* f_lastlog = fopen(_PATH_LASTLOG, "r");
    struct lastlog lastlog_data;
    int idx = 0;
    while(fread(&lastlog_data, sizeof(struct lastlog), 1, f_lastlog)){
        printf("idx : %d ", idx++);
        lastlog_print(&lastlog_data);  
    }
    return 0;
}

*/
