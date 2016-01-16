#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define USR_NAME_LEN 20
#define USR_PWD_LEN 20
#define USR_ROLE int 
#define Admin 2
#define User 1
#define Wrong 0
#define MENU_TYPE int

typedef struct usr_account_tag{
	char usr_name[USR_NAME_LEN];
	char usr_pwd[USR_PWD_LEN];
	USR_ROLE usr_role;
	struct usr_account_tag *pNext_Usr_Account;
}USR_ACCOUNT, *pUSR_ACCOUNT;

typedef struct usr_infor_tag{
	int usr_id;
	char usr_name[USR_NAME_LEN];
	float usr_score1;
	float usr_score2;
	float usr_score3;
	float usr_score4;
	float usr_score5;
	struct usr_infor_tag *pNext_Usr_Infor;
}USR_INFOR, *pUSR_INFOR;

void system_init(const char *config_file_name, pUSR_ACCOUNT *pUsr_Account, 
	             pUSR_INFOR *pUsr_Infor);//账户和学生信息初始化

void tail_insert(pUSR_ACCOUNT *phead, pUSR_ACCOUNT *ptail, char usr_name[],
	             char usr_pwd[], USR_ROLE role);//账户信息建立链表

void order_insert(pUSR_INFOR *phead, pUSR_INFOR *ptail, int usr_id, char usr_name[],
	              float score1, float score2,  float score3,  float score4,  float score5);//学生信息建立链表

USR_ROLE role_confirm(char usr_name[], char usr_pwd[], pUSR_ACCOUNT phead);//检查用户名和密码，返回权限

MENU_TYPE show_system_menu(USR_ROLE role);//显示菜单选择界面

void search_func(MENU_TYPE funcNum, pUSR_INFOR phead_infor);//查找函数

void search_all(pUSR_INFOR phead_infor);//查找所有学生信息
void search_name(pUSR_INFOR phead_infor);//通过姓名查找学生信息
void search_id(pUSR_INFOR phead_infor);//通过学号查找学生信息
void add_infor(pUSR_INFOR *phead_infor);//增加学生信息
void delete_infor(pUSR_INFOR *phead_infor);//删除学生信息
void list_delete_std(pUSR_INFOR *phead, pUSR_INFOR *ptail, int usr_id);//在链表中删除学生信息
void update_infor(pUSR_INFOR *phead_infor);//更新学生信息
void print_infor(pUSR_INFOR phead_infor);//将学生信息打印到"usr_infor.txt"文档中

void add_account(pUSR_ACCOUNT *phead_account);//增加账号信息
void delete_account(pUSR_ACCOUNT *phead_account);//删除账户信息
void list_delete_act(pUSR_ACCOUNT *phead, pUSR_ACCOUNT *ptail, char usr_name[]);//在链表中删除账户信息
void update_account(pUSR_ACCOUNT *phead_account);//更新账户信息
void search_account(pUSR_ACCOUNT phead_account);//查找账户信息
void print_account(pUSR_ACCOUNT phead_account);//将账户信息打印到"usr_account.txt"文档中
