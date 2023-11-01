#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#define LIMIT 1000

int usercount=0;
int expenses_count=0;
int settlement_count=0; // Number of unique user pair settlements


struct Settlement
 {
    float settle_amount; //Total settlement amount between them cosidering all Expenses.
    int payer_id;
    int ower_id;
};

/*This extra "SETTLEMENT" structure stores the monetary relation between two users(If they have any). 
It contains the "TOTAL" settlement amount between the two users and the relation between them(Who is the ower and who was the payer)
This was created to make the following functions more efficient:
    void Amount_to_pay(int);
    void Amount_Owed(int);
    void delete_user(int);
    void Settle_Up(int, int);
    Using the Expense structure to run these functions would become inefficient due to too many nested loops and conditionals
Although this structures requires extra space of 48 KB (considering 4 byte int and float)
*/

struct User
{
    int userid;
    char username[100];
    float amount_spend; //the amount user has balance in the environment(Total amount he spent from his pocket and is not yet settled)

};

struct Expense
{
    int expenseID;
    float amount;   //Total Amount the payer paid
    int payer_id;   //Paying an amount doesn't mean user is also a sharer of expense(he/she might be paying for somebody else)
    int no_sharers; //number of sharers
    int sharer_id[4];
};

//Initialising all the structures to prevent garbage value
//Global declaration to prevent passing structures inside functions 
struct Settlement Settlements[4*LIMIT]={
        [0 ... 4*LIMIT-1] ={ .payer_id = 0, .ower_id = 0, .settle_amount = 0.0}
    };

struct User Users[LIMIT]={
        [0 ... 999] = { .userid = 0, .username = "NA", .amount_spend = 0.0 }
    };

struct Expense Expenses[LIMIT] = {
          [0 ... 999] = { .expenseID = 0, .amount = 0.0, .no_sharers = 0, .payer_id = 0, .sharer_id = 0}
    };

//FUNCTION DECLARATIONS

void Add_User(void);
void Add_Expense(void);
void Amount_to_pay(int);
void Amount_Owed(int);
void delete_user(int);
void Settle_Up(int, int);
void User_balances(void);
void Delete_Expense(int, int);

//FUNCTION DEFINITIONS


//**********REGARDING THE SORTING OF EXPENSE AND USER ARRAYS ACCORDING TO THE ID************
/* 
we did index linking according to the ID... "INDEX OF ARRAY = USERID or EXPENSEID"
So the array is sorted by default but there are gaps in the array
This would NOT cause extra storage requirements since storage depends on the array definition , 
Hence remains the same regardless of how we store the elements in an array.
This would eliminate the need of searching for particular User ID and Expense ID which is required in almost all functions as the 
particular User structure can be accessed directly by going to that index =ID. 
Also would make the deleting operation efficient.
*/
void Add_User(void)
{
    if(usercount<LIMIT)
    {
        int userid_to_add;
        printf("Enter User ID you want:\n");
        scanf("%d", &userid_to_add);
        if(Users[userid_to_add].userid == 0)
            {
                Users[userid_to_add].userid = userid_to_add;
                printf("Enter Username without spaces:");
                char c = getchar();
                fgets(Users[userid_to_add].username, 100, stdin);
                Users[userid_to_add].username[strcspn(Users[userid_to_add].username, "\n")] = 0;
                usercount++;
                printf("Input Taken :) \nName: %s \n",Users[userid_to_add].username);
                printf("User_id: %d \n",Users[userid_to_add].userid);
                printf("%f is amount \n\n",Users[userid_to_add].amount_spend);
            }
            else{
                printf("Sorry :( This user ID already exists. \n\n");
            }         
    }
    else
    {
        printf("Max users limit reached.\n");
    }
}


void Add_Expense(void)
{
  if(expenses_count<LIMIT)
    {
        int expenseid_to_add;
        printf("Enter the expense ID:\n");
        scanf("%d",&expenseid_to_add);
        if(Expenses[expenseid_to_add].expenseID == 0)
        {
            Expenses[expenseid_to_add].expenseID = expenseid_to_add;
            printf("Enter the amount: \n");
            scanf("%f",&Expenses[expenseid_to_add].amount);
            printf("Enter the payer ID \n");
            scanf("%d",&Expenses[expenseid_to_add].payer_id);
            printf("How many shared the expense (1-4)?\n");
            scanf("%d",&Expenses[expenseid_to_add].no_sharers);
            printf("Enter the sharer IDs\n");
            int t=0;
            while(t<Expenses[expenseid_to_add].no_sharers)
            {
                scanf("%d",&Expenses[expenseid_to_add].sharer_id[t]);
                t++;

            }
            printf("Expense ID taken: %d \n",Expenses[expenseid_to_add].expenseID);
            printf("%f is amount \n\n",Expenses[expenseid_to_add].amount);
            
            //for updating the User structure :
            Users[Expenses[expenseid_to_add].payer_id].amount_spend += Expenses[expenseid_to_add].amount;
            int amnt_shared = Expenses[expenseid_to_add].amount/Expenses[expenseid_to_add].no_sharers;
            for(int i=0;i<Expenses[expenseid_to_add].no_sharers;i++)
            {
                Users[Expenses[expenseid_to_add].sharer_id[i]].amount_spend -= amnt_shared;
            }

            //for updating an editing settlements data in settlements array, or creating a new settlement:
            if(settlement_count == 0)
            {
                for(int i=0;i<Expenses[expenseid_to_add].no_sharers;i++)
                {
                    Settlements[settlement_count].payer_id = Expenses[expenseid_to_add].payer_id;
                    Settlements[settlement_count].ower_id = Expenses[expenseid_to_add].sharer_id[i];
                    Settlements[settlement_count].settle_amount = amnt_shared;
                    settlement_count++;
                }
            }
            else
            {
                for(int j=0;j<Expenses[expenseid_to_add].no_sharers;j++)
                {
                    int flag = 0, i=0;
                    while(i<settlement_count && flag == 0)
                    {
                        if(Settlements[i].payer_id == Expenses[expenseid_to_add].payer_id && Settlements[i].ower_id == Expenses[expenseid_to_add].sharer_id[j] )
                        {

                            Settlements[i].settle_amount += amnt_shared;
                            flag = 1;
                                
                        }
                        else if(Settlements[i].ower_id == Expenses[expenseid_to_add].payer_id && Settlements[i].payer_id == Expenses[expenseid_to_add].sharer_id[j])
                        {                            
                            Settlements[i].settle_amount -= amnt_shared;    
                            flag = 1;      

                            //checking if the amount after settling is negative to swap payer and ower:
                            if(Settlements[i].settle_amount<0);
                            {
                                printf("correcting negative amount\n\n");
                                int temp;
                                temp=Settlements[i].payer_id;
                                Settlements[i].payer_id=Settlements[i].ower_id;
                                Settlements[i].ower_id=temp;
                                Settlements[i].settle_amount=(-1)*(Settlements[i].settle_amount);
                            }                
                        }
                
                        
                        i++;
                    }
                    if(flag == 0)
                        {
                            Settlements[settlement_count].payer_id = Expenses[expenseid_to_add].payer_id;
                            Settlements[settlement_count].ower_id = Expenses[expenseid_to_add].sharer_id[j];
                            Settlements[settlement_count].settle_amount = amnt_shared;
                            settlement_count++;
                        
                        }
                }
            }
        }
        else{
            printf("Sorry :( This Expense ID already exists. \n\n");
        }
    }
    else
    {
      printf("Limit Reached \n");
    }
}

void Amount_to_pay(int user_id)
{
    int i=0;
    float amount_to_pay=0.0;
    printf("Person ID : \tPerson name: \tAmount to pay:\n\n");
    for(i=0;i<settlement_count;i++)
    {
        if(Settlements[i].ower_id == user_id)
        {
            printf("%d \t\t %s \t\t %f\n", Settlements[i].payer_id, Users[Settlements[i].payer_id].username , Settlements[i].settle_amount);
            amount_to_pay += Settlements[i].settle_amount;
        }
    }
    printf("\nTotal amount you need to pay: %f\n", amount_to_pay);
}


void Amount_Owed(int user_id)
{
    float totalamount=0.0;
    int i=0;
    //This function does a linear search for the user_id in the Settlements structure payer id to print all the owers to that userid.
    //Using the Expense structure would have required multiple inefficient nested loops
    printf("Person ID : \t Person name: \t Amount Owed to you:\n\n");
    for(i=0;i<settlement_count;i++)
    {
        if(Settlements[i].payer_id == user_id)
        {
            printf("%d \t\t %s \t\t %f\n", Settlements[i].ower_id, Users[Settlements[i].ower_id].username , Settlements[i].settle_amount);
            totalamount += Settlements[i].settle_amount;
        }
    }
    printf("\nTotal amount owed to you: %f\n", totalamount);
    
}

//Deletes user only if all settlements are clear :
void delete_user(int user_id)
{
    if(Users[user_id].amount_spend == 0.0)
    {
        Users[user_id].userid = 0;
        strcpy(Users[user_id].username, "NA");
        printf("\nUser id Deleted\n");
    }
    else
    {
        //the function calls Amount_Owed and Amount_to_pay functions to show the unfinished settlements
        printf("\nSorry you have unfinished settlements :\n");
        Amount_Owed(user_id);
        Amount_to_pay(user_id);
    }
}

void Settle_Up(int userid_payer , int userid_receiver)
{
    int i=0, j=0, expense_id, settlement_index, flag_set=0;
    while(i<settlement_count && flag_set == 0)
    {
        if(Settlements[i].payer_id == userid_receiver && Settlements[i].ower_id == userid_payer)
        {
            printf("\n Total amount to pay: %f\n", Settlements[i].settle_amount);
            //stores the index of the particular settlement for updating to Settlements to avoid searching again
            //flag is used to terminate while loop
            settlement_index=i;
            flag_set=1;
        }
        i++;
    }
    printf("\nExpense IDs:\t Amount\n");
    for(i=0;i<LIMIT;i++)
    {
        if(Expenses[i].payer_id == userid_receiver)
        {
            for(j=0;j<Expenses[i].no_sharers;j++)
            {
                if(Expenses[i].sharer_id[j] == userid_payer)
                {
                    printf("%d\t\t%f\n", Expenses[i].expenseID , Expenses[i].amount);
                }
            }
        }
    }
    printf("\nEnter the ExpenseID of expense to settle:\n");
    scanf("%d", &expense_id);
    float settling_amount=Expenses[expense_id].amount/Expenses[expense_id].no_sharers;
    //updating expense amount in Expenses array:
    Expenses[expense_id].amount -= settling_amount;
    //updating amount in Settlements array :
    Settlements[settlement_index].settle_amount -= settling_amount;
    int index=0;
    //updating sharer id in Expenses array :
    for(i=0;i<Expenses[expense_id].no_sharers-1;i++)
    {
        if(Expenses[expense_id].sharer_id[i] == userid_payer)
        {
            index=1;
        }
        if(index==1)
        {
            Expenses[expense_id].sharer_id[i] = Expenses[expense_id].sharer_id[i+1];
        }
    }
    Expenses[expense_id].sharer_id[i] = 0;
    
    //updating user data for that amount settled :
    Users[userid_payer].amount_spend += settling_amount;
    Users[userid_receiver].amount_spend -= settling_amount;
    Expenses[expense_id].no_sharers--;
    printf("Settlements done :)\n");
    
}

void User_balances(void)
{
    for(int i=0;i<LIMIT;i++)
    {
        if(Users[i].userid !=0)
        {
            if(Users[i].amount_spend>0){
                printf("Username: %s \t User ID: %d \t spent Rs %f\n",Users[i].username, i, Users[i].amount_spend);
            }
            else
            {
                printf("Username: %s \t User ID: %d \t Owes Rs %f\n",Users[i].username, i, ((-1.0)*Users[i].amount_spend));
            }
            
        }
    }
}


void Delete_Expense(int del_expenseid, int userid)
{
    if(Expenses[del_expenseid].payer_id == userid)
            {
                float changed_amount=Expenses[del_expenseid].amount/Expenses[del_expenseid].no_sharers;
                //Updating the Users Structure
                Users[userid].amount_spend -= changed_amount;
                for(int i=0;i<Expenses[del_expenseid].no_sharers;i++)
                {
                    Users[Expenses[del_expenseid].sharer_id[i]].amount_spend += changed_amount;
                }
                //Updating Settlements Structure
                for(int j=0;j<Expenses[del_expenseid].no_sharers;j++)
                {
                    int flag = 0, i=0;
                    while(i<settlement_count && flag==0)
                    {
                        if(Settlements[i].payer_id == Expenses[del_expenseid].payer_id && Settlements[i].ower_id == Expenses[del_expenseid].sharer_id[j] )
                        {
                            Settlements[i].settle_amount += changed_amount;
                            flag=1;
                                
                        }
                        else if(Settlements[i].ower_id == Expenses[del_expenseid].payer_id && Settlements[i].payer_id == Expenses[del_expenseid].sharer_id[j])
                        {                            
                            Settlements[i].settle_amount -= changed_amount;    
                            flag = 1;  
                            //checking if amount is negative (meaning the payer actually owes money to ower):
                            if(Settlements[i].settle_amount<0);
                            {
                                printf("correcting negative amount\n\n");
                                int temp;
                                temp=Settlements[i].payer_id;
                                Settlements[i].payer_id=Settlements[i].ower_id;
                                Settlements[i].ower_id=temp;
                                Settlements[i].settle_amount=(-1)*(Settlements[i].settle_amount);
                            }                
                        }
                        i++;
                    }
                    
                }
                //deleting all Expenses variables :
                Expenses[del_expenseid].amount=0.0;
                Expenses[del_expenseid].expenseID=0;
                Expenses[del_expenseid].no_sharers=0;
                Expenses[del_expenseid].payer_id=0;
                for(int i=0;i<4;i++)
                {
                    Expenses[del_expenseid].sharer_id[i] = 0;

                }
                printf("Expense ID deleted\n");
            }
            else{
                printf("No such Expense ID exists\n");
            }
}

int main()
{
    //File reading for Settlements Structure
    //stored in following format in Settlements_data.txt :
    // "payerid" "owerid" "settle_amount"

    FILE *filePointer0 = fopen("Settlements_data.txt", "r");
    if (filePointer0 != NULL)
    {
        for(int i = 0; i < 4*LIMIT; i++)
        {
            fscanf(filePointer0, "%d %d %f\n", &(Settlements[i].payer_id) , &(Settlements[i].ower_id), &(Settlements[i].settle_amount));
        }
        fclose(filePointer0);
    }
        
    
    //File reading for User Structure
    //stored in following format in user_data.txt:
    // "user ID" "user name" "amount spend"
    
    FILE *filePointer = fopen("user_data.txt", "r");
    if (filePointer != NULL)
    {
        for(int i = 0; i < LIMIT; i++)
        {
            fscanf(filePointer, "%d %s %f\n", &(Users[i].userid) , Users[i].username, &(Users[i].amount_spend));
        }
        fclose(filePointer);
    }
    

    //File reading for Expense Structure
    //stored in following format in Expense_data.txt:
    // "expense ID" "amount" "no. of sharers" "payer ID"
    // "sharer ID1" "sharer ID2" "sharer ID3" "sharer ID4"
    
    FILE *filePointer2 = fopen("Expense_data.txt", "r");
    if (filePointer2 != NULL)
    {
        for(int i = 0; i < LIMIT; i++)
        {
            fscanf(filePointer2, "%d %f %d %d\n %d %d %d %d\n\n" , &(Expenses[i].expenseID) , &(Expenses[i].amount), &(Expenses[i].no_sharers), &(Expenses[i].payer_id), &(Expenses[i].sharer_id[0]), &(Expenses[i].sharer_id[1]), &(Expenses[i].sharer_id[2]), &(Expenses[i].sharer_id[3]));
        }
        fclose(filePointer2);
    }

    //since expenses start from ID=1 we can use 0th index of expense_data.txt to store global variables
    expenses_count = Expenses[0].sharer_id[0];
    usercount = Expenses[0].sharer_id[1];
    settlement_count = Expenses[0].sharer_id[2];


    //THE MAIN CODE
    int num=1, userid, userid1, userid2;
    while(num!=0)
    {
        printf("Enter:\n0.Exit\n1:Add User\n2.Delete User\n3.Add expense\n4.Delete expense\n5.Check User balance\n6.Check amount to pay\n7.Settle up\n8.Amount owed\n");
        scanf("%d", &num);
        if(num==1)
        {
            Add_User();
        }
        else if(num==2)
        {
            int userid_del;
            printf("Enter the User ID to delete\n");
            scanf("%d",&userid_del);
            delete_user(userid_del);

        }
        else if(num==3)
        {
            Add_Expense();
        }
        else if(num==4)
        {
            int del_expenseid, userid;
            printf("Enter the Expense ID\n");
            scanf("%d",&del_expenseid);
            printf("Enter the User ID\n");
            scanf("%d",&userid);
            Delete_Expense(del_expenseid, userid);
            
        }
        else if(num==5)
        {
            User_balances();
        }
        else if(num==6)
        {
            printf("Enter the User ID:\n");
            scanf("%d", &userid);
            Amount_to_pay(userid);
        }
        else if(num==7)
        {
            printf("Enter the User ID of payer:\n");
            scanf("%d", &userid1);
            printf("Enter the User ID of receiver:\n");
            scanf("%d", &userid2);
            Settle_Up(userid1 , userid2);
        }
        else if(num==8)
        {
            printf("Enter User ID:\n");
            scanf("%d", &userid);
            Amount_Owed(userid);
        }
        else if(num !=0)
        {
            printf("Invalid input\n");
        }
    }
        
    //FOR SAVING THE DATA OF USERS
    filePointer = fopen("user_data.txt", "w");
    if (filePointer == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }

    for (int i = 0; i < LIMIT; i++)
    {
        fprintf(filePointer, "%d %s %f\n", Users[i].userid , Users[i].username, Users[i].amount_spend);
    }

    fclose(filePointer);
    printf("User Data saved to the file successfully.\n");

    //USING Expense_data file for storing global variables(Currently Writing)
    Expenses[0].sharer_id[0] = expenses_count;
    Expenses[0].sharer_id[1] = usercount;
    Expenses[0].sharer_id[2] = settlement_count;
    
    //FOR SAVING THE DATA OF EXPENSES 
    filePointer2 = fopen("Expense_data.txt", "w");
    if (filePointer2 == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    for (int i = 0; i < LIMIT; i++)
    {
        fprintf(filePointer2, "%d %f %d %d\n %d %d %d %d\n\n", Expenses[i].expenseID , Expenses[i].amount, Expenses[i].no_sharers, Expenses[i].payer_id, Expenses[i].sharer_id[0], Expenses[i].sharer_id[1], Expenses[i].sharer_id[2], Expenses[i].sharer_id[3]);
    }

    fclose(filePointer2);
    printf("Expense Data saved to the file successfully.\n");

    //FOR SAVING DATA OF SETTLEMENTS
    filePointer0 = fopen("Settlements_data.txt", "w");
    if (filePointer0 == NULL)
    {
        printf("Error opening file.\n");
        exit(1);
    }

    for (int i = 0; i < LIMIT; i++)
    {
        fprintf(filePointer0, "%d %d %f\n", Settlements[i].payer_id , Settlements[i].ower_id, Settlements[i].settle_amount);
    }

    fclose(filePointer0);
    printf("Settlements Data saved to the file successfully.\n");
}