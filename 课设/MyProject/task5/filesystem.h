

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
  int inode_map[INODENUM];//inode�б� 
  int block_map[BLOCKNUM];//block�б� 
  int inode_free_num;//inode������Ŀ 
  int block_free_num;//block������Ŀ 
} super_block;

typedef struct inode {
  int block_used[FILEBLKMAX];//ʹ�õ�block���� 
  int block_used_num;//ʹ��block����Ŀ 
  int size;//��С 
  int mode;//Ȩ�� 
  time_t creat_time;//����ʱ�� 
  time_t modify_time;//����޸�ʱ�� 
  int user_id;//�û�id 
} inode;

typedef struct directory {
  char name[FILENAMEMAX];//Ŀ¼���� 
  int inode_id;
} directory;

typedef struct user {
  char user_name[USERNAMEMAX];//�û��� 
  char user_pwd[USERPWDMAX];//���� 
} user;

typedef struct sys_users {
  int user_map[USERNUMMAX];//�û���Ȩ�޼��� 
  int user_num;//�û���Ŀ 
  user users[USERNUMMAX];//�û���Ϣ���� 
} sys_users;

/* Global variables */
extern FILE *disk;//������ 
extern super_block super;//������ 
extern int current_inode_id;//��ǰinode_id 
extern inode current_inode;//��ǰinode 
extern directory current_dir_content[DIRMAXNUM];//��ǰĿ¼���� 
extern int current_dir_num;//��ǰĿ¼��Ŀ 
extern int current_user_id;//��ǰ�û�id 

extern char path[128];//·���� 

/* Developer functions */
void reset_disk();//���ô��� 
void print_current_user_id(void);//��ӡ��ǰ�û�id 
void print_current_inode_id(void);//��ӡ��ǰinode��id 
void print_current_dir_num(void);//��ӡ��ǰĿ¼���� 
void show_files_info();//��ʾ�ļ���Ϣ 
void print_superblk_inode_info(int pos);//��ӡ��������inode��Ϣ 
void print_superblk_block_info(int pos);//��ӡ��������block��Ϣ 
void show_users_info(void);//��ӡ�����û���Ϣ 

/* Core function */
int inode_alloc(void);//����inode 
int inode_free(int ino);//�ͷ�inode 
int init_dir_inode(int new_ino, int ino);//��ʼ��Ŀ¼inode 
int init_file_inode(int new_ino);//��ʼ���ļ�inode 
int block_alloc(void);//����block 
int block_free(int bno);//�ͷ�block 

/* User function */
int login(void);//��¼ 
int user_pwd(void);//���� 
int user_add(char *name, char *pwd);//����û� 
int user_del(char *name);//ɾ���û� 

/* Disk function */
int load_super_block(void);//���س����� 
int format_disk(void);//��ʽ������ 
int close_disk(void);//�رմ��� 

/* File and directory function */
int dir_open(int ino);//��Ŀ¼ 
int dir_close(int ino);//�ر�Ŀ¼ 
int dir_creat(int ino, int type, char *name);//����Ŀ¼ 
int dir_rm(int ino, int type, char *name);//�Ƴ�Ŀ¼ 
int dir_cd(int ino, char *path);//ת��Ŀ¼ 
int dir_ls(void);//չʾĿ¼ 
int dir_ls_l(void);//չʾ����Ŀ¼�����ļ� 
int file_open(int ino, char *name);//���ļ� 
int file_close(int ino, char *name);//�ر��ļ� 
int file_cat(void);//��ʾ�ļ���Ϣ 
int file_mv(int ino, char *srcname, char *dstname);//�ƶ��ļ���ָ��·�� 
int file_cp(int ino, char *srcname, char *dstname);//�����ļ� 

/* Assist function */
int oct2dec(int oct_number);//8->10��ֹת�� 
int check_name(char *name);//����û��� 
int check_type(int ino, int type);//������� 
int check_mode(int mode, int operation);//���ȥ���� 
void path_change(int old_inode_id, char *name);//�ı�·�� 
int mtime_change(int ino, char *name);//�ı�ʱ�� 
void get_modestr(char *modstr, int mode);//��ȡȨ�� 
int mode_change(int mode, char *name);//�ı�Ȩ�� 
int check_if_readonly(int ino, char *name);//�ж��Ƿ���ֻ�� 

#endif 
