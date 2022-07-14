#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Define a struct that represents the data of the transaction
struct Data
{
    int num, price ;
    char Date[10], company[20] ;
};

// Implementation of Stacks
typedef struct node* PtrToNode;
typedef PtrToNode Stack;
struct node
{
    struct Data data ;
    PtrToNode Next;
};
int IsEmptyS( Stack S )
{
    return S->Next == NULL;
}
void Pop( Stack S )
{
    PtrToNode firstCell;
    if( IsEmptyS( S ) )
        printf( "Empty stack" );
    else
    {
        firstCell = S->Next;
        S->Next = S->Next->Next;
        free( firstCell);
    }
}
void MakeEmptyS( Stack S )
{
    if( S == NULL )
        printf( "Out of space!\n" );
    else
        while( !IsEmptyS( S ))
            Pop( S );
}
Stack CreateStack()
{
    Stack S;
    S = ( Stack )malloc( sizeof( struct node ) );
    if( S == NULL )
    {
        printf( "Out of space!\n" );
        return NULL;
    }
    S->Next = NULL;
    MakeEmptyS( S );
    return S;
}
struct Data Top( Stack S )
{
    return S->Next->data;
}
void Push( struct Data D, Stack S )
{
    Stack temp = ( Stack )malloc( sizeof( struct node ) );
    if( temp == NULL )
        printf( "Out of space!" );
    else
    {
        temp->data.num = D.num;   // The left operand of -> must be a pointer; the . operator is used if it is not a pointer.
        temp->data.price = D.price;
        strcpy(temp->data.company,D.company);
        strcpy(temp->data.Date,D.Date);
        temp->Next = S->Next;
        S->Next = temp;
    }
}
void DisposeStack( Stack S )
{
    MakeEmptyS( S );
    free( S );
}

// Implementation of Queues
typedef struct QueueRecord* Queue;
struct QueueRecord
{
    int Capacity;
    int Front;
    int Rear;
    int Size;
    struct Data *Array;
};
void MakeEmptyQ( Queue Q)
{
    Q->Size = 0;
    Q->Front = 1;
    Q->Rear = 0;
}
Queue CreateQueue(int MaxElements)
{
    Queue Q = (Queue)malloc(sizeof( struct QueueRecord));
    if( Q == NULL)
        printf("Out of space!\n");
    Q->Array = (struct Data*)malloc(sizeof(struct Data) * MaxElements);
    if( Q->Array == NULL )
        printf("Out of space!\n");
    Q->Capacity = MaxElements;
    MakeEmptyQ( Q );
    return Q;
}
int IsEmptyQ( Queue Q)
{
    return Q->Size == 0;
}
int IsFull( Queue Q)
{
    return Q->Size == Q->Capacity;
}
int Succ( int Value, Queue Q )
{
    if( ++Value == Q->Capacity )
        Value = 0;
    return Value;
}
void Enqueue( struct Data D, Queue Q )
{
    if( IsFull( Q ) )
        printf( "Full Queue" );
    else
    {
        Q->Size++;
        Q->Rear = Succ( Q->Rear, Q );
        Q->Array[ Q->Rear ].num = D.num;
        Q->Array[ Q->Rear ].price = D.price;
        strcpy(Q->Array[ Q->Rear ].company,D.company);
        strcpy(Q->Array[ Q->Rear ].Date,D.Date);
    }
}
struct Data Front( Queue Q )
{
    return Q->Array[ Q->Front ];
}
void Dequeue( Queue Q )
{
    if( IsEmptyQ( Q ) )
        printf("Empty Queue!\n");
    else
    {
        Q->Size--;
        Q->Front = Succ( Q->Front, Q );
    }
}
struct Data FrontAndDequeue( Queue Q )
{
    struct Data D ;
    Q->Size--;
    D.num = Q->Array[ Q->Front ].num;
    D.price = Q->Array[ Q->Front ].price;
    strcpy(D.company,Q->Array[ Q->Front ].company);
    strcpy(D.Date,Q->Array[ Q->Front ].Date);
    Q->Front = Succ( Q->Front, Q );
    return D;
}
void DisposeQueue( Queue Q )
{
    if( Q != NULL )
    {
        free( Q->Array );
        free( Q );
    }
}

// Define Th Functions that we will need
void printMenu();
int confirmed(char ConfirmOption[]);
void readFile(Queue Q, Stack S, struct Data* D);
char* Trim(char *S);
int CorrectInputs(struct Data D);
void DateAnalysis(int *day, int *month, int *year, char Date[]);
void WriteFile(Queue Q);
void Buy(Queue Q, Stack S, struct Data* D, int* SUM);
void ScanTransaction(struct Data* D, int flag);
int IsChecked(Queue Q, struct Data D);
void Sell_(Queue Q, Stack S, struct Data D, int* SUM, int flag);
void _LIFO(Stack S, Stack temp, struct Data* D, int* sum);
void _FIFO(Queue Q, Stack temp, struct Data* D, int* sum);
void PrintProcedure(int number, int price, struct Data D);
void PrintGainOrLoss(int X);
void SortByDate(Queue Q, Stack S, struct Data D);


int main()
{
    Stack MyStack = NULL;   // Create the Stack and the Queue that we will use in functions by passing them
    MyStack = CreateStack();
    Queue MyQueue = NULL;
    MyQueue = CreateQueue(20); // Capacity of the Queue is 20
    struct Data D ; // Create a struct Data that we will need it to store the data temporary in some operations
    int sum = 0 ; // 'sum' will show the Total Gain/Loss. after every selling/buying process, the Gain/Loss will add to 'sum'
    char Option[2], SellOption[2], ConfirmOption[2]; // To Scan the option from user
    do // do the statements below at least once
    {
        printMenu(); // call function to print menu
        scanf("%s", Option);
        if(strlen(Option) > 1) // because the options have one character
            strcpy(Option,"0") ;
        switch(atoi(Option)) // switch to know what is the option
        {
        case 1 : // if Option is 1..
            if(!IsEmptyQ(MyQueue)) // if there are a held data ..
            {
                printf("The currently held stocks transactions will be lost\n"); // print a message
                if(confirmed(ConfirmOption)) // and ask to confirm this process
                {
                    sum = 0 ; // if confirmed, reset 'sum'
                    readFile(MyQueue, MyStack, &D); // Call function to read
                }
            }
            else
            {
                // if there is no held data
                sum = 0 ; // reset 'sum'
                readFile(MyQueue, MyStack, &D); // Call function to read
            }
            break;
        case 2 : // if Option is 2..
            ScanTransaction(&D, 1); // take data from user with flag = 1 (scan the date)
            if(confirmed(ConfirmOption)) // ask to confirm this process
                Buy(MyQueue, MyStack, &D, &sum); // if confirmed, call function to buy
            break;
        case 3 : // if Option is 3..
            if(IsEmptyQ(MyQueue)) // if there is no held data..
            {
                printf("\tThere is no shares to sell!\n"); // print this message
                break; // and don't continue
            } // if there are held data..
            printf("--------------------------------------------\n\n");   //  print this message to select
            printf("Write the number of the option that you want\n");    // which type of selling we will use
            printf("1. Sell old shares first    2. Sell new shares first\n");
            scanf("%s",SellOption);
            if((atoi(SellOption) == 1 || atoi(SellOption) == 2) && strlen(SellOption) == 1) // if option is 1 or 2..
            {
                ScanTransaction(&D, 0); // take data from user with flag 0 (date doesn't matter)
                if(IsChecked(MyQueue, D)) // if the inputs are logical
                {
                    if(confirmed(ConfirmOption))  // ask to confirm the process
                    {
                        SortByDate(MyQueue, MyStack, D); // sort to know from where to sell
                        Sell_(MyQueue, MyStack, D, &sum, atoi(SellOption)-1); // call function to sell, the last parameter is a flag
                        printf("\t\tThe shares are sold!\n");
                    }
                }
            }
            else // if the scanned option is not 1 or 2 ..
                printf("     Sorry but your input is not correct!\n"); // print this message and break
            break;
        case 4 : // if Option is 4..
            PrintGainOrLoss(sum); // call function to print Total Gain/Loss for all processes
            break;
        case 5 : // if Option is 5..
            printf("If the currently held transactions are empty, it will make the file empty!\n"); // print a message to explain
            SortByDate(MyQueue, MyStack, D); // unimportant (just a view)
            WriteFile(MyQueue); // call function to write
            printf("The held shares are stored!\n");
            break;
        case 6 : // if Option is 6..
            printf("      The currently held stocks transactions are written in the file\n"); // print a message to explain
            printf("If the currently held transactions are empty, it will make the file empty!\n");
            SortByDate(MyQueue, MyStack, D);
            WriteFile(MyQueue); // call function to write
            break;
        default: // if the previous options dpn't include the scanned option
            printf("Sorry but this thing doesn't exist!\n");
        }
    }
    while(atoi(Option) != 6); // if option 6 had not chosen, do the do-while loop one more time
    DisposeStack(MyStack); // if option 6 was chosen, free the Stack and the Queue
    DisposeQueue(MyQueue);
    return 0;
}
void printMenu() // Just Print..
{
    printf("------------------- Menu -------------------\n");
    printf("Write the number of the option that you want\n\n");
    printf("         1. Read Stocks transaction.\n");
    printf("            2. Buy a new shares.\n");
    printf("         3. sell from the held shares.\n");
    printf("          4. Total capital gain/loss.\n");
    printf("        5. Update currently held shares.\n");
    printf("           6. Exit the application.\n");
}
int confirmed(char ConfirmOption[]) // print a confirm message and scan the input
{
    printf("\tAre you sure to complete this process?\n\t- Press any key except 1 to cancel -\n\t    1. Yes\n");
    scanf("%s", ConfirmOption);
    if(atoi(ConfirmOption) == 1 && strlen(ConfirmOption) == 1) // if the input is 1 return true, else return false
        return 1;
    return 0;

}
void readFile(Queue Q, Stack S, struct Data* D) // Read shares.txt File
{
    char temp[50]; // To store the scanned line
    MakeEmptyS(S) ; // to provide the repeated scanning..
    MakeEmptyQ(Q) ;
    FILE* in = fopen("shares.txt","r");
    if(in == NULL)
    {
        printf("Error in opening the file!\n");
        return;
    }
    while(fscanf(in, "\n%[^\n]s", temp)!= EOF) // if there is a line to scan..
    {
        // split by ";",remove the white spaces and store them in a suitable place..
        D->num = atoi(Trim(strtok(temp,";")));
        D->price = atoi(Trim(strtok(NULL,";")));
        strcpy(D->company, Trim(strtok(NULL,";")));
        strcpy(D->Date, Trim(strtok(NULL,";")));
        if(CorrectInputs(*D)) // To check that the inputs are correct
        {
            Push(*D, S); // Push the data into the Stack
            Enqueue(*D, Q); // Push the data into the Queue
        }
        else
        {
            // If the inputs are incorrect, print this message
            printf("       There is at least one fault in your inputs\n");
            printf("  The stocks transaction must be written like this:\n");
            printf("Number of shares ; Price per share ; Company Name ; Date\n");
            printf(" Please correct the inputs to be as the previous form!\n\n");
            MakeEmptyS(S);  // because the reading process will be cancelled
            MakeEmptyQ(Q); // and the data in the Queue and Stack are no value of them
            return; // break the while-loop reading
        }
    }
    if(IsEmptyQ(Q))
        printf("File is Empty!\n");
    else
        printf("The Input file has been read correctly\n");
    fclose(in);
}
char* Trim(char *S) // Remove the white spaces
{
    int i, j;
    for(i=0; S[i]==' '||S[i]=='\t'; i++); // to make 'i' equals the first UnSpace character's Location

    for(j=0; S[i]; i++)
    {
        S[j++]=S[i]; // to copy the string without the left white space
    }
    S[j]='\0'; // insert the null character
    for(i=0; S[i]!='\0'; i++) // while 'i' doesn't equal the null character
    {
        if(S[i]!=' '&& S[i]!='\t') // reach the last UnSpace character's location
            j=i;                  // and put it in 'j'
    }
    S[j+1]='\0'; // insert the null character after the last UnSpace character
    return S;
}
int CorrectInputs(struct Data D) // To check if the inputs of the scanning process are logical
{
    int token[3]; // to store day,month,year in them
    DateAnalysis( &token[0], &token[1], &token[2], D.Date); // for easy dealing with date
    int Months31[] = {1, 3, 5, 7, 8, 10, 12}; // define 31 & 30 months..
    int Months30[] = {4, 6, 9, 11};
    int i = 0;
    if(D.num > 0 && D.price >= 0 && token[0] > 0 && token[0] <= 31 && token[2] > 0) // check these logical conditions..
    {
        for(i = 0 ; i < 7 ; i++) // if the month is month31, the day must be between 1 and 32
            if(token[1] == Months31[i])
                return 1;
        for(i = 0 ; i < 4 ; i++) // if the month is month30, the day must be between 1 and 31
            if((token[1] ==  Months30[i]) && token[0] <= 30)
                return 1;
        if(token[1] ==  2 && token[0] <= 29)  // if the month is February, the day must be between 1 and 30..
        {
            if(token[2]%4 != 0 && token[0] == 29)  // if the year is not Leap, there is no day 29
            {
                printf("\t\t%d is not a Leap Year!\n", token[2]);
                return 0; // return false
            }
            return 1;
        }
    }
    return 0;
}
void DateAnalysis(int *day, int *month, int *year, char Date[]) // turn the date's string to integers
{
    // split the string with delimeter "-",remove the white spaces,turn it to integer then store it..
    *day = atoi(Trim(strtok(Date,"-")));
    *month = atoi(Trim(strtok(NULL,"-")));
    *year = atoi(Trim(strtok(NULL,"")));
}
void WriteFile(Queue Q) // Write shares.txt File
{
    FILE* out = fopen("shares.txt","w");
    for(int  i = Q->Size ; i>0 ; i--) // Do the next statements (number of transaction) times
    {
        fprintf(out,"%d ; %d ; %s ; %s\n", Front(Q).num, Front(Q).price, Front(Q).company, Front(Q).Date); // Print Data in the file
        Enqueue(FrontAndDequeue(Q),Q); // delete the front data, and insert it in the rear
    } // after this while-loop, Q will be the same before
    fclose(out);
}
void Buy(Queue Q, Stack S, struct Data *D, int* SUM) // Buy a new shares ( new transaction )
{
    if(CorrectInputs(*D)) // check if the inputs are correct
    {
        Push(*D, S); // Push the data in the Stack
        Enqueue(*D, Q); // Push the data in the Queue
        *SUM += D->num*(0-D->price); // calculate the Gain/Loss and add it to the total Gain/Loss of All transactions
        printf("\t\tThe shares are bought!\n\n");
    }
    else
        printf("    There is at least one wrong input of Buying Info!\n");
}
void ScanTransaction(struct Data *D, int flag) // To Scan the inputs from user (Not from File)
{
    printf("Please enter the number of the shares:\n");
    scanf("%d", &D->num);
    printf("Please enter the price per share:\n");
    scanf("%d", &D->price);
    printf("Please enter the company's name:\n");
    scanf("\n%[^\n]s", D->company);
    if(flag) // Flag selects if scanning date is important or not
    {
        // it is important in Buying process, unlike the selling process
        printf("Please enter the Date of purchase:\n");
        scanf("%s", D->Date);
    }
}
int IsChecked(Queue Q, struct Data D) // To insure that the inputs of the selling process are logical
{
    if(D.num <= 0 || D.price < 0) // number of shares cant be less than or equals to zero
    {
        // price of shares can be zero but not negative number
        printf("\tSorry but your input is not correct!\n");
        return 0;
    }
    int sum = 0 ;
    for(int i = 0 ; i < Q->Size ; i++) // for each transaction ..
    {
        if(strcasecmp(Front(Q).company,D.company) == 0) //if it is for the required company..
            sum += Front(Q).num; // add the number of shares to 'sum'
        Enqueue(FrontAndDequeue(Q), Q); // delete the front data, and insert it into the rear
    }
    if(sum == 0) // to insure that there is at least a one share in the required company
    {
        printf("You don't have any shares in this company!\n");
        return 0;
    }
    if(sum < D.num) // To insure that the user has sum of shares more than or equal to the shares to be sold in the required company
    {
        printf("You can't sell shares more than you have in this company!\n");
        return 0 ;
    }
    return 1 ; // if no condition of the previous ones are true, then the inputs are true( return true )
}
void Sell_(Queue Q, Stack S, struct Data D, int* SUM, int flag) // Preparing to selling, whatever the type
{
    Stack temp = NULL;    // Define a temporarily Stack to store the result in it
    temp = CreateStack();// and copy the contents to the Origin Stack and the Origin Queue
    int sum = 0 ; // the sum of Gain/Loss for each Selling Process will be here
    if(flag) // Flag to select the type of the selling
    {
        _FIFO(Q, temp, &D, &sum); // If Flag = 1, call FIFO
        MakeEmptyS(S); // To give the Stack the new contents
    }
    else
    {
        _LIFO(S, temp, &D, &sum); // If Flag = 0, call LIFO
        MakeEmptyQ(Q); // To give the Queue the new contents
    }

    // there is no need to ReSort the data in Q and S, because if the user want to resell, they will sorted again
    while(!IsEmptyS(temp)) // While there is a Data in the temporarily Stack
    {
        Push(Top(temp), S); // Push the data of the top of temp into the origin Stack
        Enqueue(Top(temp), Q); // Push the data of the top of temp into the origin Queue
        Pop(temp); // Delete the top of temp
    }
    *SUM += sum; // Add sum to the total Gain/Loss of All processes
    PrintGainOrLoss(sum); // Print the sum of Gain/Loss for this process
    DisposeStack(temp); // free the temporarily Stack
}
void _LIFO(Stack S, Stack temp, struct Data *D, int* sum) // Using LIFO to sell, The top of the Stack is the newest
{
    while(!IsEmptyS(S)) // while there is a transaction in Stack S..
    {
        if(strcasecmp(D->company, Top(S).company) == 0) // if it is from the required company..
        {
            if(D->num < Top(S).num) // if the number of shares to be sold less than the shares in the transaction..
            {
                if(D->num != 0) // if the rest of the number of shares to be sold do not equal to zero..
                    PrintProcedure(D->num, D->price, Top(S)); // call function to print the procedure
                S->Next->data.num -= D->num; // subtract number of shares to be sold from the shares in the transaction
                Push(Top(S), temp); // Push the data of the transaction into the temporarily Stack
                *sum += D->num *(D->price - Top(S).price); // calculate the Gain/Loss of this procedure and add it to Total
                D->num = 0; // because the rest of the number of shares to be sold are sold from this transaction
            }
            else
            {
                // if the number of shares to be sold are not less than the shares in the transaction..
                PrintProcedure(Top(S).num, D->price, Top(S)); // call function to print the procedure
                D->num -= Top(S).num ; // subtract the number of shares in this transaction from the shares to be sold
                *sum += Top(S).num * (D->price - Top(S).price); // calculate the Gain/Loss of this procedure and add it to Total
                // we will not push the data of this transaction to 'temp' because the transaction is not useful without shares
            }
        }
        else // if the transaction is not from the required company
            Push(Top(S), temp); // Push the data of the transaction into the temporarily Stack
        Pop(S); // After all that, Pop from Stack S
    }
}
void _FIFO(Queue Q, Stack temp, struct Data *D, int* sum) // Using FIFO to sell, The front of the Queue is the oldest
{
    while(!IsEmptyQ(Q)) // while there is a transaction in Queue Q..
        if(strcasecmp(Front(Q).company,D->company) == 0) // if it is from the required company..
        {
            if(D->num < Front(Q).num) // if the number of shares to be sold less than the shares in the transaction..
            {
                if(D->num != 0) // if the rest of the number of shares to be sold do not equal to zero..
                    PrintProcedure(D->num, D->price, Front(Q)); // call function to print the procedure
                Q->Array[Q->Front].num -= D->num ; // subtract number of shares to be sold from the shares in the transaction
                *sum += D->num*(D->price - Front(Q).price); // calculate the Gain/Loss of this procedure and add it to Total
                Push(FrontAndDequeue(Q), temp); // Push the data of the transaction into the temporarily Stack
                D->num = 0; // because the rest of the number of shares to be sold are sold from this transaction
            }
            else
            {
                // if the number of shares to be sold are not less than the shares in the transaction..
                PrintProcedure(Front(Q).num, D->price, Front(Q)); // call function to print the procedure
                *sum += Front(Q).num * (D->price - Front(Q).price); // calculate the Gain/Loss of this procedure and add it to Total
                D->num -= Front(Q).num; // subtract the number of shares in this transaction from the shares to be sold
                Dequeue(Q); // delete the data of the transaction from the Queue, because it is not useful without shares
            }
        }
        else // if the transaction is not from the required company..
            Push(FrontAndDequeue(Q),temp); // Push the data of the transaction into 'temp', and delete it from the Queue
}
void PrintProcedure(int number, int price, struct Data D) // print how many shares are sold for each transaction
{
    printf("%d shares are sold out of %d which are bought from %s on %s\n", number, D.num, D.company, D.Date);
    printf("%d * (%d-%d)$ = %d$\n\n", number, price, D.price, number*(price - D.price));
}
void PrintGainOrLoss(int X) // Just Print..
{
    printf("The budget of this procedure/s : %d$\n", X);
    if(X > 0)
        printf("************ profit ************\n");
    if(X < 0)
        printf("************  loss  ************\n");
    else
        printf("\n");
}
void SortByDate(Queue Q, Stack S, struct Data D) // Sort the Queue and the Stack
{
    Stack temp = NULL; // Create a temporary Stack to use it in sorting
    temp = CreateStack();
    int day[2], month[2], year[2]; // to store the two dates for the two struct data that we will compare between them
    char datetemp[10]; // to use in Strtok without editing the origin string
    while (!IsEmptyS(S)) // while there is a data in Stack S
    {
        // pop out the first element
        D = Top(S); // D will take the top of S
        Pop(S);
        DateAnalysis(&day[1], &month[1], &year[1], strcpy(datetemp,D.Date)); // take the date of transaction D as integers

        // while temporary stack is not empty and top
        // of stack is greater than temp
        while (!IsEmptyS(temp)) // while temporary Stack is not empty ..
        {
            DateAnalysis(&day[0], &month[0], &year[0], strcpy(datetemp,Top(temp).Date)); // take the date as integers
            if(year[0] > year[1] || (year[0] == year[1] && month[0] > month[1])      // if the date of the top of temporary Stack
                    || (year[0] == year[1] && month[0] == month[1] && day[0] > day[1])) // newer than the date of transaction D..
            {
                Push(Top(temp), S); // Pop from temporary Stack and Push it to the Stack S
                Pop(temp);
            }
            else
                break;
        }
        Push(D, temp); // Push transaction D to temporary Stack
    }
    MakeEmptyQ(Q); // to insert the new data
    while(!IsEmptyS(temp)) // while temporary Stack is not empty .. "To insert the contents of Stack temp into S and Q"
    {
        Push(Top(temp), S); // Push from temporary Stack to the Stack S
        Enqueue(Top(temp), Q); // Pop from temporary Stack and insert it to the Queue Q
        Pop(temp);
    }
    DisposeStack(temp); // free temp
}
