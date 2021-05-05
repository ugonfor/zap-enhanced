#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "util.h"
#include "zap.h"
#include "utmp/utmp_util.h"
#include <utmp.h>
#include <time.h>
#include <pwd.h>

int remove_check(int mode, struct utmp* utmp_data ,char* username, char* tty, char* mmddyy){
    if(mode==1){
        if(strcmp(utmp_data->ut_user, username) == 0) return 1;
        else return 0;
    }
    else{
        if(strcmp(utmp_data->ut_user, username)) return 0;
        if(strcmp(utmp_data->ut_line, tty)) return 0;
        
        //time needs a little complex method
        time_t utmp_time_sec = utmp_data->ut_tv.tv_sec;
        struct tm *utmp_time;
        utmp_time = localtime(&utmp_time_sec);

        int utmp_yy = utmp_time->tm_year - 100; // 2021 -> 21
        
        int utmp_dd = utmp_time->tm_mday;
        int utmp_mm = utmp_time->tm_mon + 1; // jan -> 1 , dec -> 12S

        int mmddyy_i = atoi(mmddyy);
        int yy = mmddyy_i % 100;
        mmddyy_i /= 100;
        int dd = mmddyy_i % 100;
        mmddyy_i /= 100;
        int mm = mmddyy_i % 100;
        mmddyy_i /= 100;
        
        //printf("%d = %d %d %d\n", utmp_data->ut_tv.tv_sec, utmp_dd, utmp_mm, utmp_yy);

        if(!(utmp_mm == mm && utmp_dd == dd && utmp_yy == yy)) return 0;
        return 1;
    }
}

int change_check(struct utmp* utmp_data ,char* username, char* tty, char* mmddyy){
    // ret = 1 : should change
    // ret = 0 : don't change
    
    if(strcmp(utmp_data->ut_user, username)) return 0;
    if(strcmp(utmp_data->ut_line, tty)) return 0;
    
    //time needs a little complex method
    time_t utmp_time_sec = utmp_data->ut_tv.tv_sec;
    struct tm *utmp_time;

    utmp_time = localtime(&utmp_time_sec);

    int utmp_yy = utmp_time->tm_year - 100; // 2021 -> 21
    
    int utmp_dd = utmp_time->tm_mday;
    int utmp_mm = utmp_time->tm_mon + 1; // jan -> 1 , dec -> 12S
    int mmddyy_i = atoi(mmddyy);

    int yy = mmddyy_i % 100;
    mmddyy_i /= 100;
    int dd = mmddyy_i % 100;
    mmddyy_i /= 100;
    int mm = mmddyy_i % 100;
    mmddyy_i /= 100;
    
    //printf("%d = %d %d %d\n", utmp_data->ut_tv.tv_sec, utmp_dd, utmp_mm, utmp_yy);
    if(!(utmp_mm == mm && utmp_dd == dd && utmp_yy == yy)) return 0;
    return 1;

}

int uwtmp_modify(int mode, char* src_path, char* dst_path,char* username, char* tty, char* mmddyy){

    FILE* f_uwtmp = fopen(src_path,"r");
    fseek(f_uwtmp, 0, SEEK_END);
    int size_uwtmp = ftell(f_uwtmp);
    fseek(f_uwtmp, 0, SEEK_SET);

    char* modified_utmp_data = (char*)malloc(size_uwtmp+1);

    struct utmp utmp_data = {0};

    int write_idx = 0;
    int read_idx = 0;
    while(read_idx != size_uwtmp){
        fread(&utmp_data, sizeof(utmp_data), 1, f_uwtmp);
        read_idx += sizeof(utmp_data);

        if (remove_check(mode, &utmp_data, username, tty, mmddyy)) continue; // ret : 0 : don't remove, 1 : should remove
        // mode : 1 -> for A option
        // mode : 0 -> for a option
        
        memcpy(modified_utmp_data+write_idx, &utmp_data, sizeof(utmp_data));
        write_idx += sizeof(utmp_data);
    }
    fclose(f_uwtmp);
    
    FILE* f_uwtmp_modified = fopen(dst_path, "w");
    fwrite(modified_utmp_data, write_idx, 1, f_uwtmp_modified);

    free(modified_utmp_data);
    fclose(f_uwtmp_modified);
    return 0;
}


int uwtmp_change(char* src_path, char* dst_path,char* username1, char* username2, char* tty1, char* tty2, char* mmddyy1, char* mmddyy2){

    FILE* f_uwtmp = fopen(src_path,"r");
    fseek(f_uwtmp, 0, SEEK_END);
    int size_uwtmp = ftell(f_uwtmp);
    fseek(f_uwtmp, 0, SEEK_SET);

    char* modified_utmp_data = (char*)malloc(size_uwtmp+1);

    struct utmp utmp_data = {0};

    int write_idx = 0;
    int read_idx = 0;
    while(read_idx != size_uwtmp){
        fread(&utmp_data, sizeof(utmp_data), 1, f_uwtmp);
        read_idx += sizeof(utmp_data);

        if (change_check(&utmp_data, username1, tty1, mmddyy1)) { // ret : 0 : don't change, 1 : should change
            printf("Before : %s %s %d\n", utmp_data.ut_user, utmp_data.ut_line, utmp_data.ut_tv.tv_sec);
            printf("checking : %s %s %s\n", username1, tty1, mmddyy1);

            printf("tobe : %s %s %s\n", username2, tty2, mmddyy2);
            memcpy(&utmp_data.ut_user, username2, strnlen(username2, sizeof(utmp_data.ut_user)));
            memset((char*)(&utmp_data.ut_user) + strnlen(username2,sizeof(utmp_data.ut_user)), 0, sizeof(utmp_data.ut_user) - strlen(username2));
            memcpy(&utmp_data.ut_line, tty2, strnlen(tty2, sizeof(utmp_data.ut_line)));
            memset((char*)(&utmp_data.ut_line) + strnlen(tty2,sizeof(utmp_data.ut_line)), 0, sizeof(utmp_data.ut_line) - strlen(tty2));
            
            // have to fix

            time_t utmp_time_sec = utmp_data.ut_tv.tv_sec;
            struct tm *utmp_time;

            utmp_time = localtime(&utmp_time_sec);
            
            int mmddyy2_i = atoi(mmddyy2);

            int yy = mmddyy2_i % 100;
            mmddyy2_i /= 100;
            int dd = mmddyy2_i % 100;
            mmddyy2_i /= 100;
            int mm = mmddyy2_i % 100;
            mmddyy2_i /= 100;

            utmp_time->tm_year = yy + 100;
            utmp_time->tm_mday = dd;
            utmp_time->tm_mon = mm - 1;

            time_t modified_time = mktime(utmp_time);

            utmp_data.ut_tv.tv_sec = modified_time;

            printf("CHANGE : %s %s %d\n", utmp_data.ut_user, utmp_data.ut_line, utmp_data.ut_tv.tv_sec);

            memcpy(modified_utmp_data+write_idx, &utmp_data, sizeof(utmp_data));
            write_idx += sizeof(utmp_data);
        }
        else{
            memcpy(modified_utmp_data+write_idx, &utmp_data, sizeof(utmp_data));
            write_idx += sizeof(utmp_data);
        }
            
    }
    fclose(f_uwtmp);
    
    FILE* f_uwtmp_modified = fopen(dst_path, "w");
    fwrite(modified_utmp_data, write_idx, 1, f_uwtmp_modified);

    free(modified_utmp_data);
    fclose(f_uwtmp_modified);
    return 0;
}

int GetLastlogData_from_wtmp(char* src_path, char* username1, struct lastlog* dst_data){
    FILE* f_wtmp = fopen("./wtmp", "r");
    struct utmp utmp_data = {0,};

    struct lastlog latest_lastlog_data = {0,};
    while (fread(&utmp_data, sizeof(struct utmp), 1, f_wtmp))
    {
        if(strcmp(utmp_data.ut_user, username1)) continue; // if username is not same, continue
        if(utmp_data.ut_line[0] == ':') continue; // if tty is like ":0" then, continue
        // since the lastlog show the lastest login from "ssh"
        // if log is formed by ssh login, tty of log is like pts/n 

        memcpy(&latest_lastlog_data.ll_host, &utmp_data.ut_host, sizeof(utmp_data.ut_host));
        memcpy(&latest_lastlog_data.ll_line, &utmp_data.ut_line, sizeof(utmp_data.ut_line));
        memcpy(&latest_lastlog_data.ll_time, &utmp_data.ut_tv.tv_sec, sizeof(utmp_data.ut_tv.tv_sec));
    }

    memcpy(dst_data, &latest_lastlog_data, sizeof(struct lastlog));

    return 0;
}

int lastlog_modify(char* src_path, char* dst_path, int uid){
    // remake the lastlog file based on wtmp file
    // remove the original file and make it as a new file
    
    FILE* f_lastlog = fopen(src_path,"r");
    fseek(f_lastlog, 0, SEEK_END);
    int size_lastlog = ftell(f_lastlog);
    fseek(f_lastlog, 0, SEEK_SET);

    char* modified_lastlog_data = (char*)malloc(size_lastlog+1);

    struct lastlog lastlog_data = {0,};
    
    int idx = 0;
    for (int i = 0; i < uid; i++)
    {
        fread(&lastlog_data, sizeof(struct lastlog), 1, f_lastlog);
        // lastlog_print(&lastlog_data); for debug

        memcpy(modified_lastlog_data + i*sizeof(struct lastlog), &lastlog_data, sizeof(struct lastlog));
        idx += sizeof(struct lastlog);
    }
    
    fread(&lastlog_data, sizeof(struct lastlog), 1, f_lastlog);

    //get the last login data from wtmp file
    GetLastlogData_from_wtmp("./wtmp", username1, &lastlog_data);
    
    memcpy(modified_lastlog_data + idx, &lastlog_data, sizeof(struct lastlog));
    idx += sizeof(struct lastlog);
    
    while(fread(&lastlog_data, sizeof(struct lastlog), 1, f_lastlog)){
        memcpy(modified_lastlog_data + idx, &lastlog_data, sizeof(struct lastlog));
        idx += sizeof(struct lastlog);
    }

    fclose(f_lastlog);

    FILE* f_lastlog_modified = fopen(dst_path, "w");
    printf("idx : %d", idx);
    fwrite(modified_lastlog_data, idx, 1, f_lastlog_modified);

    free(modified_lastlog_data);
    fclose(f_lastlog_modified);

    return 0;
}

int A_OptZap(char* username1){
    uwtmp_modify(1, _PATH_UTMP, "./utmp", username1,0,0);
    uwtmp_modify(1, _PATH_WTMP, "./wtmp", username1,0,0);

    struct passwd* passwd_data = getpwnam(username1);
    if (passwd_data == NULL) return 0; // no username for modify
    //since there are "reboot" in utmp/wtmp file, I didn't do this before the uwtmp_modify fts.

    lastlog_modify(_PATH_LASTLOG, "./lastlog", passwd_data->pw_uid);

    return 0;   
}

int a_OptZap(char* username1, char* tty1, char* mmddyy1){
    uwtmp_modify(0, _PATH_UTMP, "./utmp", username1, tty1, mmddyy1);
    uwtmp_modify(0, _PATH_WTMP, "./wtmp", username1, tty1, mmddyy1);

    struct passwd* passwd_data = getpwnam(username1);
    if (passwd_data == NULL) return 0; // no username for modify
    //since there are "reboot" in utmp/wtmp file, I didn't do this before the uwtmp_modify fts.
    
    lastlog_modify(_PATH_LASTLOG, "./lastlog", passwd_data->pw_uid);

    return 0;

}

int R_OptZap(char* username1,char* username2,char* tty1,char* tty2,char* mmddyy1,char* mmddyy2){
    uwtmp_change(_PATH_UTMP, "./utmp", username1, username2, tty1, tty2, mmddyy1, mmddyy2);
    uwtmp_change(_PATH_WTMP, "./wtmp", username1, username2, tty1, tty2, mmddyy1, mmddyy2);

    struct passwd* passwd_data = getpwnam(username1);
    if (passwd_data == NULL) return 0; // no username for modify
    //since there are "reboot" in utmp/wtmp file, I didn't do this before the uwtmp_modify fts.
    
    lastlog_modify(_PATH_LASTLOG, "./lastlog", passwd_data->pw_uid);

    struct passwd* passwd_data = getpwnam(username2);
    if (passwd_data == NULL) return 0; // no username for modify
    //since there are "reboot" in utmp/wtmp file, I didn't do this before the uwtmp_modify fts.
    
    lastlog_modify(_PATH_LASTLOG, "./lastlog", passwd_data->pw_uid);

    return 0;
}
