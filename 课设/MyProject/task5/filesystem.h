

#ifndef lnq_FILESYSTEM
#define lnq_FILESYSTEM

#include "./lib/sys.h"
#include "fs_error.h"
#include <time.h>
#include <wait.h>

/* Disk parameters */
#define BLOCKSIZE 1024          /* The block size */
#define INODENUM 1024           /* The Inode nums */
#define BLOCKNUM (1024 * 64)    /* The disk size 64 MB */
#define FILEBLKMAX (1024 * 4)   /* The file max size is 4 MB */
#define FILENAMEMAX 20           /* File and directory name max length */
#define USERNAMEMAX 20          /* User name max length */
#define USERPWDMAX 20           /* User password max length */
#define USERNUMMAX 10			/* The max num of user*/ 

#define INODESIZE sizeof(inode)                         /* The inode size */
#define SUPERPOS sizeof(sys_users)                      /* The #1 is super block */
#define INODEPOS (SUPERPOS + sizeof(super_block))       /* The #2 ~ #1025 are inodes */
#define BLOCKPOS (INODEPOS + INODESIZE * INODENUM)      /* The #1026 is first free block */


#define TYPE_DIR 0
#define TYPE_FILE 1

#define DIRMAXINBLK (BLOCKSIZE / sizeof(directory)) /* Directory entry num per block */
#define DIRMAXNUM (FILEBLKMAX * DIRMAXINBLK)        /* File num per directory */

#define CAN_READ 0
#define CAN_WRITE 1

#define BUFFERFILE "lnq_disk_temp_buff"

/* File system data structures */
typedef struct super_block {
  int inode_map[INODENUM];//inode列表 
  int block_map[BLOCKNUM];//block列表 
  int inode_free_num;//inode空闲数目 
  int block_free_num;//block空闲数目 
} super_block;

typedef struct inode {
  int block_used[FILEBLKMAX];//使用的block集合 
  int block_used_num;//使用block的数目 
  int size;//大小 
  int mode;//权限 
  time_t creat_time;//创建时间 
  time_t modify_time;//最后修改时间 
  int user_id;//用户id 
} inode;

typedef struct directory {
  char name[FILENAMEMAX];//目录名称 
  int inode_id;
} directory;

typedef struct user {
  char user_name[USERNAMEMAX];//用户名 
  char user_pwd[USERPWDMAX];//密码 
} user;

typedef struct sys_users {
  int user_map[USERNUMMAX];//用户名权限集合 
  int user_num;//用户数目 
  user users[USERNUMMAX];//用户信息集合 
} sys_users;

/* Global variables */
extern FILE *disk;//磁盘名 
extern super_block super;//超级块 
extern int current_inode_id;//当前inode_id 
extern inode current_inode;//当前inode 
extern directory current_dir_content[DIRMAXNUM];//当前目录内容 
extern int current_dir_num;//当前目录数目 
extern int current_user_id;//当前用户id 

extern char path[128];//路径名 

/* Developer functions */
void reset_disk();//重置磁盘 
void print_current_user_id(void);//打印当前用户id 
void print_current_inode_id(void);//打印当前inode的id 
void print_current_dir_num(void);//打印当前目录数量 
void show_files_info();//显示文件信息 
void print_superblk_inode_info(int pos);//打印超级块中inode信息 
void print_superblk_block_info(int pos);//打印超级块中block信息 
void show_users_info(void);//打印所有用户信息 

/* Core function */
int inode_alloc(void);//分配inode 
int inode_free(int ino);//释放inode 
int init_dir_inode(int new_ino, int ino);//初始化目录inode 
int init_file_inode(int new_ino);//初始化文件inode 
int block_alloc(void);//分配block 
int block_free(int bno);//释放block 

/* User function */
int login(void);//登录 
int user_pwd(void);//密码 
int user_add(char *name, char *pwd);//添加用户 
int user_del(char *name);//删除用户 

/* Disk function */
int load_super_block(void);//加载超级块 
int format_disk(void);//格式化磁盘 
int close_disk(void);//关闭磁盘 

/* File and directory function */
int dir_open(int ino);//打开目录 
int dir_close(int ino);//关闭目录 
int dir_creat(int ino, int type, char *name);//创建目录 
int dir_rm(int ino, int type, char *name);//移除目录 
int dir_cd(int ino, char *path);//转到目录 
int dir_ls(void);//展示目录 
int dir_ls_l(void);//展示所有目录所有文件 
int file_open(int ino, char *name);//打开文件 
int file_close(int ino, char *name);//关闭文件 
int file_cat(void);//显示文件信息 
int file_mv(int ino, char *srcname, char *dstname);//移动文件到指定路径 
int file_cp(int ino, char *srcname, char *dstname);//拷贝文件 

/* Assist function */
int oct2dec(int oct_number);//8->10禁止转换 
int check_name(char *name);//检查用户名 
int check_type(int ino, int type);//检查类型 
int check_mode(int mode, int operation);//检查去哪线 
void path_change(int old_inode_id, char *name);//改变路径 
int mtime_change(int ino, char *name);//改变时间 
void get_modestr(char *modstr, int mode);//获取权限 
int mode_change(int mode, char *name);//改变权限 
int check_if_readonly(int ino, char *name);//判断是否是只读 

#endif 
