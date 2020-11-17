//#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>

void get_permissions_str(const mode_t mode, char* perm_str, const int size) {
    if(size < 9) {
        return;
    }
    memset(perm_str, '-', 9);
    if(mode & S_IRUSR) perm_str[0] = 'r';
    if(mode & S_IWUSR) perm_str[1] = 'w';
    if(mode & S_IXUSR) perm_str[2] = 'x';
    if(mode & S_IRGRP) perm_str[3] = 'r';
    if(mode & S_IWGRP) perm_str[4] = 'w';
    if(mode & S_IXGRP) perm_str[5] = 'x';
    if(mode & S_IROTH) perm_str[6] = 'r';
    if(mode & S_IWOTH) perm_str[7] = 'w';
    if(mode & S_IXOTH) perm_str[8] = 'x';
}

int main(void) {
    DIR *dir = opendir("./");
    struct dirent *de;
    if (dir != NULL){
        while (de = readdir(dir)) {
            if(!de->d_name || de->d_name[0] == '.') {
                continue;
            }
            //  type
            char type = '-';
            switch (de->d_type) {
                case DT_BLK:  type = 'b';  break;
                case DT_CHR:  type = 'c';  break;
                case DT_DIR:  type = 'd';  break;
                case DT_FIFO: type = 'p';  break;
                case DT_LNK:  type = 'l';  break;
                case DT_SOCK: type = 's';  break;
            }
            //  permissions
            struct stat st;
            stat(de->d_name, &st);
            char permissions[10] = {0};
            get_permissions_str(st.st_mode, permissions, sizeof(permissions));
            //  user name
            struct passwd *user = getpwuid(st.st_uid);
            //  group name
            struct group *grp = getgrgid(st.st_gid);
            //  time
            time_t t = time(NULL);
            struct tm *tm_info = localtime(&t);
            char buf[26];
            //strftime(buf, 26, "%Y-%m-%d %H:%M:%S", tm_info);
            //strftime(buf, 26, "%c", tm_info);
            strftime(buf, 26, "%b %d %H:%M", tm_info);
            //  print
            printf("%c%s %3ld %s %s %15ld %s %s\n", type, permissions, \
                st.st_nlink, user->pw_name, grp->gr_name, st.st_size, buf, de->d_name);
        }
    }
    closedir(dir);
    return 0;
}