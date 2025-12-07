#include<stdio.h>

int login();
void displayMenu();
void checkbalance(float balance);
float withdrawMoney(float balance);
float balance = 5000.0;

int login(){
    int correctpin= 1234;
    int userpin;
    int attempts=0;

    while(attempts<3){

    printf("enter your userpin\n");
    scanf("%d",&userpin);

    if(userpin==correctpin){
        printf("access granted\n");
        return 1;
    }else{
        attempts++;
        printf("wrong pin!attempts left:%d\n",3-attempts);

    }
}
    return 0;
}

void displayMenu(){
    printf("ATM MENU\n");
    printf("1.check balance\n");
    printf("2.deposit\n");
    printf("3.withdraw\n");
    printf("4.exit\n");
}

void checkbalance(float balance){
    printf("your balance is:%.2f\n",balance);
}

float deposit(float balance){
    float amount;
    printf("enter amount to deposit\n");
    scanf("%f",&amount);

    if(amount>0){
        balance+=amount;
        printf("your balance is:%.2f\n",balance);
    }else{
        printf("invalid\n");
    }
    return balance;
}

float withdrawMoney(float balance){
    float amount;
    printf("enter amount to withdraw\n");
    scanf("%f",&amount);

    if(amount<=0){
        printf("invalid amount\n");
    }else if(amount>balance){
        printf("insufficient funds\n");
    }else{
        balance-=amount;
        printf("your balance is:%.2f\n",balance);
    }
    return balance;
}

int main(){
    float balance = 5000.0;
    int choice;

    if(login() == 0) {
        return 0;  // exit program if login failed
    }

    while(1) {
        displayMenu();
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                checkbalance(balance);
                break;


            case 2:
                balance = deposit(balance);
                break;


            case 3:
                balance = withdrawMoney(balance);
                break;


            case 4:
                printf("Thank you. Goodbye!\n");
                return 0;

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}
