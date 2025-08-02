#include<ctype.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct node {
    int balance;
    int id;
    char password[100];
    char name[100];
    char transactions[5][100];
    int t_index;
    struct node *link;
} *root;
int count = 0;
void update_file() {
    int i;
    FILE *fp = fopen("accounts.txt", "w");
    struct node *t = root;
    while (t != NULL) {
	fprintf(fp, "%d %s %s %d %d\n", t->id, t->name, t->password, t->balance, t->t_index);
	for (i = 0; i < 5; i++) {
	    fprintf(fp, "%s\n", t->transactions[i]);
	}
	t = t->link;
    }
    fclose(fp);
}
void create() {
    struct node *nn, *temp;
    int i,bal, valid = 0;
    char na[100], pass[20], ch;
    printf("Please enter your name:\n");
    fflush(stdin);
    gets(na);
    do {
	i = 0;
	printf("Set a valid password for your account (8-10 characters without space):\n");
	while ((ch = getch()) != '\r' && i < 10) {
	    if (ch == ' ') continue;
	    pass[i++] = ch;
	    printf("*");
	}
	pass[i] = '\0';
	printf("\n");

	if (strlen(pass) >= 8 && strlen(pass) <= 10)
	    valid = 1;
	else
	    printf("Invalid password length. Try again.\n");
    } while (!valid);
    printf("Minimum deposit for creating an account is Rs.3000\n");
    printf("Enter the amount you want to deposit(Maximum: Rs.25000)\n");
    scanf("%d", &bal);
    if (bal < 3000 || bal > 25000) {
	printf("Invalid deposit amount. Please try again.\n");
	return;
    }
    nn = (struct node *)malloc(sizeof(struct node));
    nn->link = NULL;
    nn->balance = bal;
    count++;
    nn->id = count;
    strcpy(nn->name, na);
    strcpy(nn->password, pass);
    nn->t_index = 0;
    for (i = 0; i < 5; i++)
	strcpy(nn->transactions[i], "");
    sprintf(nn->transactions[nn->t_index], "Account created with deposit Rs. %d", bal);
    nn->t_index = (nn->t_index + 1) % 5;
    temp = root;
    if (root == NULL)
	root = nn;
    else {
	while (temp->link != NULL) {
	    temp = temp->link;
	}
	temp->link = nn;
    }
    printf("Your account id is %d\n", count);
    update_file();
}
void del_acc() {
    struct node *t = root, *p;
    int accid, op;
    char pass[20];
    printf("Do you want to delete your account?\n1.Yes\n2.No\n");
    scanf("%d", &op);
    if (op == 2)
	return;
    else {
	printf("Enter the account id:\n");
	scanf("%d", &accid);
	printf("Enter the password\n");
	fflush(stdin);
	scanf("%s", pass);
	while (t != NULL && t->id != accid) {
	    p = t;
	    t = t->link;
	}
	if (t == root && (strcmp(t->password, pass) == 0)) {
	    p = t->link;
	    printf("Please collect your balance amount at counter 1: Rs.%d\n", t->balance);
	    free(t);
	    root = p;
	} 
 	else if (t == NULL) {
	    printf("Incorrect id..! Please try again..!\n");
	    return;
	}
 else if (strcmp(t->password, pass) == 0) {
	    p->link = t->link;
	    printf("Name: %s", t->name);
	    printf("Please collect your balance amount at counter 1: Rs.%d\n", t->balance);
	    free(t);
	    count--;
	} 
else
	    printf("Invalid password\n");
    }
    update_file();
}

void withdraw() {
    int accid, amount;
    char pass[20];
    struct node *t = root;
    printf("Enter the acc id:\n");
    scanf("%d", &accid);
    printf("Enter the password\n");
    fflush(stdin);
    scanf("%s", pass);
    while (t != NULL && t->id != accid) {
	t = t->link;
    }
    if (t == NULL) {
	printf("Incorrect id..! Please try again..!\n");
	return;
    } else if (strcmp(t->password, pass) == 0) {
	printf("Enter the amount you want to withdraw\n");
	scanf("%d", &amount);
	if (amount > t->balance) {
	    printf("Insufficient balance\n");
	    return;
	} else {
	    t->balance -= amount;
	    printf("Please collect your money at counter 2: %d\nAccount balance: %d\n", amount, t->balance);
	    sprintf(t->transactions[t->t_index], "Withdrew Rs. %d", amount);
	    t->t_index = (t->t_index + 1) % 5;
	}
    } 
else {
	printf("Invalid password\n");
    }
    update_file();
}
void transfer() {
    int accid, amount, id2;
    char pass[20];
    struct node *t = root, *p = root;
    printf("Enter your acc id:");
    scanf("%d", &accid);
    printf("Enter the password:");
    fflush(stdin);
    scanf("%s", pass);
    while (t != NULL && t->id != accid) {
	t = t->link;
    }
    if (t == NULL) {
	printf("Incorrect id..! Please try again..!\n");
	return;
    }
 else if (strcmp(t->password, pass) == 0) {
	printf("Enter receiver acc id:\n");
	scanf("%d", &id2);
	while (p != NULL && p->id != id2) {
	    p = p->link;
	}
	if (p == NULL) {
	    printf("Invalid id\n");
	} 
else {
	    printf("Enter the amount you want to transfer:");
	    scanf("%d", &amount);
	    if (amount > t->balance) {
		printf("Insufficient balance\n");
		return;
	    } 
else {
		t->balance -= amount;
		p->balance += amount;
		printf("Amount transferred successfully..!");
		printf("Account balance: %d\n", t->balance);
		sprintf(t->transactions[t->t_index], "Transferred Rs. %d to ID %d", amount,   p->id);
		t->t_index = (t->t_index + 1) % 5;
		sprintf(p->transactions[p->t_index], "Received Rs. %d from ID %d", amount, t->id);
		p->t_index = (p->t_index + 1) % 5;
	    }
	}
    }
 else {
	printf("Invalid password\n");
    }
    update_file();
}

void cb() {
    int accid;
    char pass[20];
    struct node *t = root;
    printf("Enter the acc id:\n");
    scanf("%d", &accid);
    printf("Enter the password\n");
    fflush(stdin);
    scanf("%s", pass);
    while (t != NULL && t->id != accid) {
	t = t->link;
    }
    if (t == NULL) {
	printf("Incorrect id..! Please try again..!\n");
	return;
    } 
else if (strcmp(t->password, pass) == 0) {
	printf("Account Balance:\n%d\n", t->balance);
    }
 else {
	printf("Invalid password\n");
    }
}
void deposit() {
    int accid, amount;
    char pass[20];
    struct node *t = root;
    printf("Enter the acc id:\n");
    scanf("%d", &accid);
    printf("Enter the password\n");
    fflush(stdin);
    scanf("%s", pass);
    while (t != NULL && t->id != accid) {
	t = t->link;
    }
    if (t == NULL) {
	printf("Incorrect id..! Please try again..!\n");
	return;
    } 
else if (strcmp(t->password, pass) == 0) {
	printf("Enter the amount you want to deposit:");
	scanf("%d", &amount);
	t->balance += amount;
	printf("Deposit successful\n");
	printf("Account Balance:\n%d\n", t->balance);
	sprintf(t->transactions[t->t_index], "Deposited Rs. %d", amount);
	t->t_index = (t->t_index + 1) % 5;
    } 
else {
	printf("Invalid password\n");
    }
    update_file();
}
void changepass() {
    struct node *t = root;
    int x;
    char s[100], pass[100];
    int found = 0;
    printf("Enter the ID to change password\n");
    scanf("%d", &x);
    printf("Enter the old password\n");
    scanf("%s", s);
    while (t != NULL) {
	if (t->id == x) {
	    if (strcmp(t->password, s) == 0) {
		found = 1;
		break;
	    } 
else {
		printf("Incorrect password.\n");
		return;
	    }
	}
	t = t->link;
    }
    if (!found) {
	printf("Invalid ID.\n");
	return;
    }
    do {
	printf("\nSet a valid new password for your account (8-10 characters, no spaces):\n");
	scanf("%s", pass);
    } while (strlen(pass) < 8 || strlen(pass) > 10 || strchr(pass, ' '));
    strcpy(t->password, pass);
    printf("Password changed successfully.\n");
    update_file();
}
void load_from_file() {
    int i;
    size_t len;
    FILE *fp = fopen("accounts.txt", "r");
    struct node *nn, *last = NULL;
    if (!fp) return;
    while (1) {
	nn = (struct node *)malloc(sizeof(struct node));
	if (fscanf(fp, "%d %s %s %d %d", &nn->id, nn->name, nn->password, &nn->balance, &nn->t_index) == 5) {
	    for (i = 0; i < 5; i++) {
		fgets(nn->transactions[i], 100, fp);
		len = strlen(nn->transactions[i]);
		if (len > 0 && nn->transactions[i][len - 1] == '\n')
		    nn->transactions[i][len - 1] = '\0';
	    }
	    nn->link = NULL;
	    if (root == NULL)
		root = nn;
	    else
		last->link = nn;
	    last = nn;
	    if (nn->id > count)
		count = nn->id;
	} 
else {
	    free(nn);
	    break;
	}
    }
    fclose(fp);
}
void view_transactions() {
    int accid, i;
    char pass[20];
    struct node *t = root;
    int printed = 0;
    printf("Enter the account ID:\n");
    scanf("%d", &accid);
    printf("Enter the password:\n");
    fflush(stdin);
    scanf("%s", pass);
    while (t != NULL && t->id != accid) {
	t = t->link;
    }
    if (t == NULL) {
	printf("Incorrect account ID! Please try again.\n");
	return;
    }
    if (strcmp(t->password, pass) != 0) {
	printf("Invalid password!\n");
	return;
    }
    printf("Transaction history for %s (ID: %d):\n", t->name, t->id);
    for (i = 0; i < 5; i++) {
	int index = (t->t_index + i) % 5;
	if (strlen(t->transactions[index]) > 0) {
	    printf("%d. %s\n", printed + 1, t->transactions[index]);
	    printed++;
	}
    }
    if (printed == 0) {
	printf("No transactions found.\n");
    }
}
void main() {
    int op;             
    load_from_file();
    do {
	printf("Enter option:\n1.Create an account\n2.Deposit Money\n3.Money withdrawal\n4.Check Balance\n5.Money Transfer\n6.Delete your account\n7.Change password\n8.View Transactions\n9.Exit\n");
	scanf("%d", &op);
	switch (op) {
	    case 1: create();
		    break;
	    case 2: deposit();
		    break;
	    case 3: withdraw();
		    break;
	    case 4: cb();
		    break;
	    case 5: transfer();
		    break;
	    case 6: del_acc();
		    break;
	    case 7: changepass();
		    break;
	    case 8: view_transactions();
		    break;
	}
    } while (op != 9);
}
