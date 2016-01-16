#include "system.h"

/*系统初始化*/
void system_init(const char *config_file_name, pUSR_ACCOUNT *pUsr_Account, pUSR_INFOR *pUsr_Infor)
{
	FILE *fp_account, *fp_infor, *fp_config;
	pUSR_ACCOUNT ptail_account;
	pUSR_INFOR ptail_infor;
	int usr_id;
	float usr_score1, usr_score2, usr_score3, usr_score4, usr_score5;
	char usr_name[USR_NAME_LEN], ch_account[40] = {0}, ch_infor[40] = {0},
		 usr_pwd[USR_PWD_LEN];
	USR_ROLE usr_role;//定义权限：0代表管理员，1代表用户
	ptail_account = NULL;
	ptail_infor = NULL;

	fp_config = fopen(config_file_name, "r+");
	if(fp_config == NULL){
		printf("config.txt open failure");
		return;
	}
	
	fscanf(fp_config, "%s", ch_account);
	fscanf(fp_config, "%s", ch_infor);
	fclose(fp_config);
	fp_account = fopen(ch_account, "r+");
	fp_infor = fopen(ch_infor, "r+");

	while(fscanf(fp_account, "%s%s%d", usr_name, usr_pwd, &usr_role) == 3){
		tail_insert(pUsr_Account, &ptail_account, usr_name, usr_pwd, usr_role);
	}
	while(fscanf(fp_infor, "%d%s%f%f%f%f%f",&usr_id,usr_name,&usr_score1,&usr_score2,&usr_score3,&usr_score4,&usr_score5)== 7){
		order_insert(pUsr_Infor, &ptail_infor,usr_id,usr_name,usr_score1,usr_score2,usr_score3,usr_score4,usr_score5);
	}
}

/*尾插法读取usr_account.txt生成账户信息链表*/
void tail_insert(pUSR_ACCOUNT *phead, pUSR_ACCOUNT *ptail, char usr_name[],char usr_pwd[], USR_ROLE role)
{
	pUSR_ACCOUNT pnew;
	pnew = (pUSR_ACCOUNT)malloc(sizeof(USR_ACCOUNT));
	strcpy(pnew->usr_name, usr_name);
	strcpy(pnew->usr_pwd, usr_pwd);
	pnew->usr_role = role;
	pnew->pNext_Usr_Account = NULL;

	if(*phead == NULL){
		*phead = pnew;
		*ptail = pnew;
	}
	else{
		(*ptail)->pNext_Usr_Account = pnew;
		*ptail = pnew;
	}
}

/*读取usr_infor.txt，按照学号升序顺序建立学生信息的有序链表*/
void order_insert(pUSR_INFOR *phead, pUSR_INFOR *ptail, int usr_id, char usr_name[],
	float score1, float score2,  float score3,  float score4,  float score5)
{
	pUSR_INFOR pnew, pcur, ppre;
	pnew = (pUSR_INFOR)malloc(sizeof(USR_INFOR));
	pnew->usr_id = usr_id;
	strcpy(pnew->usr_name, usr_name);
	pnew->usr_score1 = score1;
	pnew->usr_score2 = score2;
	pnew->usr_score3 = score3;
	pnew->usr_score4 = score4;
	pnew->usr_score5 = score5;
	pnew->pNext_Usr_Infor = NULL;
	
	pcur = *phead;
	if(*phead == NULL){//空表插表头
		*phead = pnew;
		*ptail = pnew;
	}
	else if(pcur->usr_id > pnew->usr_id){//非空表插在表头
		pnew->pNext_Usr_Infor = pcur;
		*phead = pnew;
	}
	else{
		ppre = pcur;
		pcur = pcur->pNext_Usr_Infor;
		while(pcur != NULL){//插入表中间
			if(pcur->usr_id > pnew->usr_id){
				pnew->pNext_Usr_Infor = pcur;
				ppre->pNext_Usr_Infor = pnew;
				break;
			}
			ppre = pcur;
			pcur = pcur->pNext_Usr_Infor;
		}
		if(pcur == NULL)//插入表尾
			ppre->pNext_Usr_Infor = pnew;
	}
}

/*检查用户名和密码，返回操作权限*/
USR_ROLE role_confirm(char usr_name[], char usr_pwd[], pUSR_ACCOUNT phead)
{
	int flag_name, flag_pwd;
	while(phead != NULL){
		flag_name = strcmp(usr_name, phead->usr_name);
		if(flag_name == 0){
			flag_pwd = strcmp(usr_pwd, phead->usr_pwd);
			if(flag_pwd == 0){
				if(phead->usr_role == 0)
					return Admin;
				else
					return User;
			}
			else{
				phead = phead->pNext_Usr_Account;
				continue;
			}
		}
		else
			phead = phead->pNext_Usr_Account;
	}
	if(phead == NULL)
		return Wrong;
}

/*界面显示以及返回用户选择的菜单项目*/
MENU_TYPE show_system_menu(USR_ROLE role)
{
	MENU_TYPE menu;
	if(role == Admin){
		printf("********************************************************************************\n");
		printf("*****************Student   Information   Management   System********************\n");	
		printf("********************************************************************************\n");
		printf("                   1.search   student   information\n");
		printf("                   2.add   student   informatin\n");
		printf("                   3.update   student   information\n");
		printf("                   4.delete   student   information\n");
		printf("                   5.add   user   account\n");
		printf("                   6.update   user   account\n");
		printf("                   7.delete   user   account\n");
		printf("                   8.search   user   account\n");
		printf("                   9.exit\n");
	}
	else if(role == User){
		printf("********************************************************************************\n");
		printf("*****************Student   Information   Management   System********************\n");	
		printf("********************************************************************************\n");
		printf("                   1.search   student   information\n");
		printf("                   9.exit\n");
	}
	else
		exit(-1);
	printf("select a number:");
	scanf("%d", &menu);
	return menu;
}

void search_func(MENU_TYPE funcNum, pUSR_INFOR phead_infor)
{
	if(funcNum == 0)
		search_all(phead_infor);
	if(funcNum == 1)
		search_name(phead_infor);
	if(funcNum == 2)
		search_id(phead_infor);
	if(funcNum == 3)
		return;
}

/*查找所有学生信息*/		
void search_all(pUSR_INFOR phead_infor)
{
	printf("学生信息如下：\n");
	while(phead_infor != NULL){
		printf("%d %s %.2f %.2f %.2f %.2f %.2f\n", phead_infor->usr_id,phead_infor->usr_name,phead_infor->usr_score1,phead_infor->usr_score2,
			phead_infor->usr_score3, phead_infor->usr_score4, phead_infor->usr_score5);
		phead_infor = phead_infor->pNext_Usr_Infor;
	}
	system("pause");
	system("cls");
}

/*按名字查找学生信息*/
void search_name(pUSR_INFOR phead_infor)
{
	char usr_name[USR_NAME_LEN] = {0};
	printf("请输入要查找学生的名字:");
	scanf("%s", usr_name);
	while(phead_infor != NULL){
		if(strcmp(phead_infor->usr_name, usr_name) == 0){
			printf("%d %s %.2f %.2f %.2f %.2f %.2f\n", phead_infor->usr_id,phead_infor->usr_name,phead_infor->usr_score1,
				phead_infor->usr_score2,phead_infor->usr_score3,phead_infor->usr_score4,phead_infor->usr_score5);
			system("pause");
			system("cls");
			break;
		}
		else
			phead_infor = phead_infor->pNext_Usr_Infor;
	}
	if(phead_infor == NULL){
		printf("没找到这个学生\n");
		system("pause");
		system("cls");
	}
}

/*通过学号查找学生信息*/
void search_id(pUSR_INFOR phead_infor)
{
	int usr_id;
	printf("请输入要查找学生的学号：");
	scanf("%d", &usr_id);
	while(phead_infor != NULL){
		if(phead_infor->usr_id == usr_id){
			printf("%d %s %.2f %.2f %.2f %.2f %.2f\n", phead_infor->usr_id,phead_infor->usr_name,phead_infor->usr_score1,
				phead_infor->usr_score2,phead_infor->usr_score3,phead_infor->usr_score4,phead_infor->usr_score5);
			system("pause");
			system("cls");
			break;
		}
		else
			phead_infor = phead_infor->pNext_Usr_Infor;
	}
	if(phead_infor == NULL){
		printf("没找到这个学生\n");
		system("pause");
		system("cls");
	}
}

/*增加学生信息*/
void add_infor(pUSR_INFOR *phead_infor)
{
	pUSR_INFOR phead, ptail, psearch;
	int usr_id;
	char usr_name[USR_NAME_LEN];
	float usr_score1,usr_score2, usr_score3, usr_score4, usr_score5;
	phead = *phead_infor;
	psearch = *phead_infor;
	while(phead->pNext_Usr_Infor != NULL)
		phead = phead->pNext_Usr_Infor;
	ptail = phead;
	printf("请按 <学号 姓名 分数1 分数2 分数3 分数4 分数5> 的格式输入学生信息:\n");
	scanf("%d%s%f%f%f%f%f",&usr_id,usr_name,&usr_score1,&usr_score2,&usr_score3,&usr_score4,&usr_score5);
	while(psearch != NULL){
		if(psearch->usr_id == usr_id){
			printf("警告：该生已经存在\n");
			return;
		}
		else
			psearch = psearch->pNext_Usr_Infor;
	}
	if(psearch == NULL){
		order_insert(phead_infor, &ptail,usr_id,usr_name,usr_score1,usr_score2,usr_score3,usr_score4,usr_score5);
		printf("Mission Success\n");
	}
}

/*更新已有学生信息*/
void update_infor(pUSR_INFOR *phead_infor)
{
	pUSR_INFOR psearch;
	int usr_id;
	char usr_name[USR_NAME_LEN];
	float usr_score1,usr_score2, usr_score3, usr_score4, usr_score5;
	psearch = *phead_infor;
	printf("请输入学号:");
	scanf("%d", &usr_id);
	while(psearch != NULL){
		if(psearch->usr_id == usr_id){
			printf("请按 <姓名 分数1 分数2 分数3 分数4 分数5> 的格式输入学生信息:\n");
			scanf("%s%f%f%f%f%f",usr_name,&usr_score1,&usr_score2,&usr_score3,&usr_score4,&usr_score5);
			strcpy(psearch->usr_name, usr_name);
			psearch->usr_score1 = usr_score1;
			psearch->usr_score2 = usr_score2;
			psearch->usr_score3 = usr_score3;
			psearch->usr_score4 = usr_score4;
			psearch->usr_score5 = usr_score5;
			printf("Mission Success\n");
			return;
		}
		else
			psearch = psearch->pNext_Usr_Infor;
	}
	if(psearch == NULL){
		printf("警告：没有找到该学号的学生\n");
	}
}

/*删除学生信息*/
void delete_infor(pUSR_INFOR *phead_infor)
{
	pUSR_INFOR phead, ptail, psearch;
	int usr_id;
	phead = *phead_infor;
	psearch = *phead_infor;
	while(phead->pNext_Usr_Infor != NULL)
		phead = phead->pNext_Usr_Infor;
	ptail = phead;

	printf("请输入要删除学生的学号：");
	scanf("%d", &usr_id);
	while(psearch != NULL){
		if(psearch->usr_id == usr_id){
			list_delete_std(phead_infor, &ptail, usr_id);
			printf("Mission  Success\n");
			return;
		}
		else
			psearch = psearch->pNext_Usr_Infor;
	}
	if(psearch == NULL){
		printf("找不到学号为%d的学生\n", usr_id);
	}
}

/*在链表中删除学生信息*/
void list_delete_std(pUSR_INFOR *phead, pUSR_INFOR *ptail, int usr_id)
{
	pUSR_INFOR pcur, ppre;
	pcur = *phead;
	if(pcur->usr_id == usr_id){//删除头结点
		*phead = pcur->pNext_Usr_Infor;
		free(pcur);
		if(*phead == NULL)
			*ptail = NULL;
		return;
	}
	else{
		ppre = pcur;
		pcur = pcur->pNext_Usr_Infor;
		while(pcur != NULL){
			if(pcur->usr_id == usr_id){
				ppre->pNext_Usr_Infor = pcur->pNext_Usr_Infor;
				if(ppre->pNext_Usr_Infor == NULL)
					*ptail = ppre;
				free(pcur);
				return;
			}
			ppre = pcur;
			pcur = pcur->pNext_Usr_Infor;
		}
	}
}

/*将学生信息打印到文档中*/
void print_infor(pUSR_INFOR phead_infor)
{
	FILE *fp_infor;
	fp_infor = fopen("usr_infor.txt", "w+");
	while(phead_infor != NULL){
		fprintf(fp_infor, "%d %s %.2f %.2f %.2f %.2f %.2f\n",phead_infor->usr_id, phead_infor->usr_name,phead_infor->usr_score1,
			               phead_infor->usr_score2,phead_infor->usr_score3,phead_infor->usr_score4,phead_infor->usr_score5);
		phead_infor = phead_infor->pNext_Usr_Infor;
	}
}

/*增加账号信息*/
void add_account(pUSR_ACCOUNT *phead_account)
{
	pUSR_ACCOUNT phead, ptail, psearch;
	char usr_name[USR_NAME_LEN] = {0};
	char usr_pwd[USR_PWD_LEN] = {0};
	USR_ROLE role;
	int flag;//用户名是否已经存在标志
	phead = *phead_account;
	psearch = *phead_account;
	while(phead->pNext_Usr_Account != NULL)
		phead = phead->pNext_Usr_Account;
	ptail = phead;
	printf("请按 <账户名 账户密码 权限值（0为管理员，1为用户）> 的格式输入账户信息:\n");
	scanf("%s%s%d",usr_name, usr_pwd, &role);
	while(psearch != NULL){
		flag = strcmp(psearch->usr_name, usr_name);
		if(flag == 0){
			printf("警告：该账户已经存在\n");
			return;
		}
		else
			psearch = psearch->pNext_Usr_Account;
	}
	if(psearch == NULL){
		tail_insert(phead_account, &ptail, usr_name, usr_pwd, role);
		printf("Mission Success\n");
	}
}

/*更新账户信息*/
void update_account(pUSR_ACCOUNT *phead_account)
{
	pUSR_ACCOUNT psearch;
	char usr_name[USR_NAME_LEN] = {0};
	char usr_pwd[USR_PWD_LEN] = {0};
	USR_ROLE role;
	int flag;
	psearch = *phead_account;
	printf("请输入账户名:");
	scanf("%s", usr_name);
	while(psearch != NULL){
		flag = strcmp(psearch->usr_name, usr_name);
		if(flag == 0){
			printf("请按 <账户密码 账户权限值（0为管理员，1为用户）> 的格式输入账户信息:\n");
			scanf("%s%d",usr_pwd, &role);
			strcpy(psearch->usr_pwd, usr_pwd);
			psearch->usr_role = role;
			printf("Mission Success\n");
			return;
		}
		else
			psearch = psearch->pNext_Usr_Account;
	}
	if(psearch == NULL){
		printf("警告：没有找到该账户\n");
	}
}

/*删除账户信息*/
void delete_account(pUSR_ACCOUNT *phead_account)
{
	pUSR_ACCOUNT phead, ptail, psearch;
	char usr_name[USR_NAME_LEN] = {0};
	int flag;
	phead = *phead_account;
	psearch = *phead_account;
	while(phead->pNext_Usr_Account != NULL)
		phead = phead->pNext_Usr_Account;
	ptail = phead;

	printf("请输入要删除的账户名：");
	scanf("%s", usr_name);
	if(strcmp("admin", usr_name) == 0){
		printf("该账户为管理员账户，无法删除\n");
		return;
	}
	while(psearch != NULL){
		flag = strcmp(psearch->usr_name, usr_name);
		if(flag == 0){
			list_delete_act(phead_account, &ptail, usr_name);
			printf("Mission  Success\n");
			return;
		}
		else
			psearch = psearch->pNext_Usr_Account;
	}
	if(psearch == NULL){
		printf("抱歉：系统中没有该账户\n");
	}
}

/*在链表中删除账户信息*/
void list_delete_act(pUSR_ACCOUNT *phead, pUSR_ACCOUNT *ptail, char usr_name[])
{
	pUSR_ACCOUNT pcur, ppre;
	int flag;
	pcur = *phead;
	ppre = pcur;
	pcur = pcur->pNext_Usr_Account;
	while(pcur != NULL){
		flag = strcmp(pcur->usr_name, usr_name);
		if(flag == 0){
			ppre->pNext_Usr_Account = pcur->pNext_Usr_Account;
			if(ppre->pNext_Usr_Account == NULL)
				*ptail = ppre;
			free(pcur);
			return;
		}
		ppre = pcur;
		pcur = pcur->pNext_Usr_Account;
	}
}

/*查找账户信息*/
void search_account(pUSR_ACCOUNT phead_account)
{
	printf("账户信息如下:<账户名 账户密码 账户权限（0为管理员，1为用户）>\n");
	while(phead_account != NULL){
		printf("               %s %s %d\n", phead_account->usr_name, phead_account->usr_pwd, phead_account->usr_role);
		phead_account = phead_account->pNext_Usr_Account;
	}
}

/*将账户信息打印到文档中*/
void print_account(pUSR_ACCOUNT phead_account)
{
	FILE *fp_account;
	fp_account = fopen("usr_account.txt", "w+");
	while(phead_account != NULL){
		fprintf(fp_account, "%s %s %d\n",phead_account->usr_name, phead_account->usr_pwd, phead_account->usr_role);
		phead_account = phead_account->pNext_Usr_Account;
	}
}
