#include "system.h"

int main()
{
	pUSR_ACCOUNT phead_account;
	pUSR_INFOR phead_infor;
	char usr_name[USR_NAME_LEN] = {0}, usr_pwd[USR_PWD_LEN] = {0}, ch_temp;
	int cur = 0;
	USR_ROLE authority;
	MENU_TYPE menu;
	MENU_TYPE funcNum;

	phead_account = NULL;
	phead_infor = NULL;

	system_init("config.txt", &phead_account, &phead_infor);//系统初始化
	/*while(phead_account != NULL){
		printf("%s\n", phead_account->usr_name);
		phead_account = phead_account->pNext_Usr_Account;
	}
	while(phead_infor != NULL){
		printf("%s\n", phead_infor->usr_name);
		phead_infor = phead_infor->pNext_Usr_Infor;
	}*///测试链表是否建立成功


	printf("enter usr_name:");
	gets(usr_name);
	system("cls");
	printf("enter usr_password:");
	while((ch_temp = getch()) != EOF){
		if(ch_temp == 13)//输入回车结束
			break;
		usr_pwd[cur++] = ch_temp;
		if(ch_temp == 8 && cur == 1){
			printf(" \b");
			cur = cur - 1;
			continue;
		}
		if(ch_temp == 8 && cur > 1){
			printf("\b \b");
			cur = cur - 2;
			continue;
		}
		putchar('*');
	}
	usr_pwd[cur] = '\0';
	system("cls");
	//printf("\n%s\n", usr_pwd);//测试密码是否输入正确
	authority = role_confirm(usr_name, usr_pwd, phead_account);
	//printf("\n%d\n", authority);//测试权限返回是否正确

	do{
		menu = show_system_menu(authority);
		system("cls");
		if(authority == Admin && menu == 1){//管理员查看学生信息
			printf("0.search all\n");
			printf("1.search by name\n");
			printf("2.search by id\n");
			printf("3.return\n");
			printf("select a number:");
			scanf("%d", &funcNum);
			system("cls");
			search_func(funcNum, phead_infor);
		}
		if(authority == User && menu == 1){//用户查看学生信息
			printf("1.search by name\n");
			printf("2.search by id\n");
			printf("3.return\n");
			printf("select a number:");
			scanf("%d", &funcNum);
			system("cls");
			search_func(funcNum, phead_infor);
		}
		if(authority == Admin && menu == 2){//管理员增加学生信息
			add_infor(&phead_infor);
			print_infor(phead_infor);
			system("pause");
			system("cls");
		}
		if(authority == Admin && menu == 3){//管理员更新学生信息
			update_infor(&phead_infor);
			print_infor(phead_infor);
			system("pause");
			system("cls");
		}
		if(authority == Admin && menu == 4){//管理员删除学生信息
			delete_infor(&phead_infor);
			print_infor(phead_infor);
			system("pause");
			system("cls");
		}
		if(authority == Admin && menu == 5){//管理员增加账户信息
			add_account(&phead_account);
			print_account(phead_account);
			system("pause");
			system("cls");
		}
		if(authority == Admin && menu == 6){//管理员更新账户信息
			update_account(&phead_account);
			print_account(phead_account);
			system("pause");
			system("cls");
		}
		if(authority == Admin && menu == 7){//管理员删除账户信息
			delete_account(&phead_account);
			print_account(phead_account);
			system("pause");
			system("cls");
		}
		if(authority == Admin && menu == 8){//查找账户信息
			search_account(phead_account);
			system("pause");
			system("cls");
		}
	}while(menu != 9);
}
