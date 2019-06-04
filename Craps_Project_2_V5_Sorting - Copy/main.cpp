/* 
 * File:   main.cpp
 * Author: Gonzalez, Jorge
 * Created on May 19th,2019 1:50 PM
 * Purpose: Final Craps Game Project. 
 */

//System Libraries
#include <iostream>     // Input/Output Library
#include <cstdlib>      // Random Function Library
#include <ctime>        // Time Fuction Library
#include <iomanip>      // Formating Library
#include <string>       // String Library
#include <cmath>        // Math Library
#include <fstream>      // File Input Output Library
#include <vector>       // Vector Library
using namespace std;

//User Libraries

//Global Constants, no Global Variables are allowed
    // None used
//Math/Physics/Conversions/Higher Dimensions - i.e. PI, e, etc...
const float MAXRAND=pow(2,31)-1; // Limit of the program.
const int cols=6; // Higher Dimension array
//Function Prototypes
void   outfill(int[][cols],int);                                               // Function of an outcome table.
void   outprnt(const int[][cols],int);                                         // Function to print the outcome table.
int    rulei();                                                                // Function that will Display the rules and input for monthly pay.
float  betf(float,float &,char);                                               // Function that will calculate the betting limit.
float  gpass(float &, float,char,int,int,int,int,int,string);                    // Function for rolling the die and playing the PASS BET.
float  gpass(float &,float,char,int,int,vector<int> &,int,int,string,ofstream&); // Vector function for rolling the die and playing the DON'T PASS BET.
float  adie(vector<int> &,int,int &,int,ofstream&);                            // Function for an array dice for DON'T PASS BET.
string pname(string &);                                                        // String Function for the name. 
void   frank(float [],int);                                                    // Fill ranks.
void   selsort(float [],int);                                                  // Selection sort.
void   bublsrt(float [],int,float);                                            // Bubble sort and ranking
void   lsearch (float [],int,int,float);                                       // Finding the rank.
//Execution Begins Here!
int main(int argc, char** argv) {
    //Set the random number seed
srand(static_cast<unsigned int>(time(0))); // Randomizing the dice roles and todays ranks. 
    //Declare Variables
    //Floats
float       mpay  =0,         // Monthly pay.
            blimit=0,         //Betting Limit
            bet   =0;         // Amount betted on a round.
    //Static Int's
static int  rows  =6;         // Rows for the outcome table.  
    //Int's
int         ngames=0,         // Number of games played.
            sumrll=0,         // The outcome of the roll.
            indx=0,           // Rank of the player.
            point =0,         // This is the 'point' which will be used to see if the player wins or loses. 
            die1  =0,         // First dice that will be rolled
            die2  =0,         // Second dice that will be rolled
            tsum[rows][cols]; // Sum of the table at every i(1-6)
const int   size  =2,         // Size of the die array.
            pool  =50;        // Player pool.
float       hscore[pool];
    //Vector 
vector<int> array(size);      // Vector int for the array for dice.     
    //Characters
char        answ,             // Yes or no to answer questions
            choice;           // Choose what game type will be played.
    //Strings
    string  name;             // Name of the player.
    //Files
    ofstream file;            // File where the roll will be placed (use tuna.txt to not crash into other files). 
    //Setting cent amount.
    cout.setf(ios::fixed);
    cout.setf(ios::showpoint);// setting precision to 2 values. 
    cout.precision(2); 
    // Program Start
    //Map inputs -> outputs  
    pname(name);            // Input the player's name. 
    
    outfill(tsum,rows);     // Fill the outcome table.
    outprnt(tsum,rows);     // Display the outcome table.
    //Display the outputs
    cin>>mpay;              // Input monthly pay to calculate betting limit.
    betf(mpay,blimit,answ); // Betting limit function.
    cout<<"Choose your game type. PASS BETS: '1'| DON'T PASS: '2'| ..."<<endl;
    cin>>choice;
    switch (choice){
        case '1' :{
    gpass(blimit,bet,answ,point,die1,sumrll,die2,ngames,name); // Performing the roll and calculating the results for the PASS BETS game.(Overloaded)
    break;
        }
        case '2' :{
            //
    gpass(blimit,bet,answ,point,sumrll,array,size,ngames,name,file); // Perform the roll and calculating the results for the DON't PASS BETS game.(Overloaded)
    break;
        }
    }
    frank(hscore,pool);                 // Filling the rank table.
    selsort(hscore,pool);               // Selection sort.
    lsearch(hscore,pool,indx,blimit);   // Finding rank number. 
    //bublsrt(hscore,pool,blimit);      // Bubble sort.(Broken)
    file.close ();                      // Close the file.
    //Exit stage right or left!
    return 0;
}
// Used to get the name of the player. 
string pname(string &name){ 
    cout<<"Please input your name:"<<endl;
    cin>>name;
    return name;
}
// Display the rules
int rulei(){ 
    cout<<"Disclaimer this version of craps will be simplified to these rules:"<<endl<<endl;
    cout<<"For PASS BETS:"<<endl;
    cout<<"Rule 1- Rolling a 7 or 11 means you win and will continue to roll."<<endl;
    cout<<"Rule 2- Rolling a 2,3 or 12 means you crap out or lose and your roll ends."<<endl;
    cout<<"Rule 3- The first roll that is not an outcome roll will be assigned as the 'point' which means you will win if rolled a secondtime."<<endl;
    cout<<"Rule 4- If a 7 is rolled while a point has been made then you lose."<<endl; // Explaining the point
    cout<<"Rule 5- A winning roll will double the amount placed but a losing roll will subtract your bet."<<endl<<endl; // Betting rates
    
    cout<<"For DON'T PASS BETS:"<<endl;
    cout<<"Rule 1- Rolling a 2 or 3 means you win and will continue to roll."<<endl;
    cout<<"Rule 2- Rolling a 7 or 11 means you crap out or lose and your roll ends."<<endl;
    cout<<"Rule 3- The first roll that is not an outcome roll will be assigned as the 'point which means you lose if rolled a second time"<<endl;
    cout<<"Rule 4- If a 7 is rolled while a point has been made then you win."<<endl;
    cout<<"Rule 5- Rolling a 12 will make you tie with the dealer"<<endl;
    cout<<"Rule 6- A winning roll will double the amount placed but a losing roll will subtract your bet."<<endl<<endl;
    
}
// Fill in an outcome table for each roll.
void outfill(int array[][cols],int rows){
    rulei();                // Display the rules.
    for(int i=0;i<rows;i++){
        for(int a=0;a<cols;a++){
            array[a][i]=i+a+2; // Filling an outcome table for every roll.
        }
    }
}
// Print an outcome table for each roll.
void outprnt(const int array[][cols],int rows){
    cout<<"Here are the possible outcomes."<<endl;
    cout<<"    |";
    for(int a=1;a<=cols;a++){ // The column values. 
        cout<<setw(4)<<a;
    }
    cout<<endl<<"-------------------------------";
    for(int i=0;i<rows;++i){
        cout<<endl;
        cout<<setw(3)<<i+1<<" |";  // The Rows values
        for(int a=0;a<cols;a++){
            cout<<setw(4)<<array[a][i];
        }
    }
    cout<<endl<<endl;
    // Limiting the amount some one could spend on gambling to a year. 
    cout<<"This game will use a years worth of salary for the starting betting limit based on monthly paychecks."<<endl;
    cout<<"Please enter an amount between $0 and $20,001:"<<endl;// Request for the entry of a monthly paycheck or a number that will be used for betting (input will be done out of this function). 
}
//Calculate betting limit.
float betf(float mpay,float &blimit,char answ){
    // Verifying paycheck amount for betting limit with if else statements(This will be skipped over if a correct paycheck is inputed).
    if(mpay<=0){        // If monthly paycheck is less than or equal to 0 ask for a higher monthly paycheck.
        cout<< "Paycheck is to small please enter a larger paycheck between $0 and $20001."<<endl;
        cin>>mpay;      // Request of a correct paycheck if another invalid paycheck is entered it will take care of it in the next step.
        blimit=mpay*12; // Betting limit will be calculated here by multiplying monthly pay by 12. 
        cout<<"Your betting limit is $"<<blimit<<endl;// Display the betting limit
    // This will attempt to view if the amount was to large and was a mistake. 
    }else if(mpay>=20001){ //If the amount was not a mistake then it will prompt for an answer if 1 is inputed it will count as a yes then the large amount will be used. 
            cout<<"Paycheck is higher than $20001 if you want to use this amount there is a final total starting limit of $2,147,483. Are you sure you want to use this much? $" <<mpay<<endl<<
            "Y||N"<<endl;  // This will explain that a 1 or 2 must be pressed as the answer any other answer will lead to an error. 
            cin>>answ;     // Input the answer. 
                           // This will view if Y or y was selected which will be viewed as a yes. 
            if(answ=='Y'||answ=='y'){// Pressing Y or y will continue the program even though the betting limit is high. 
                blimit=mpay*12;      // Betting limit will be calculated here by multiplying monthly pay by 12. 
                cout<<"Your betting limit is $"<<blimit<<endl; // Outputting to the user their betting limit. 
                cout<<endl;          // Line skip.
            }
            else if(answ=='N'||'n') { // Pressing N or n will prompt another cin of monthly pay. 
                cout<<"Please enter a monthly paycheck between $0 and $20001:"<<endl;
                cin>>mpay;      // Inputing a correct monthly pay.
                blimit=mpay*12; // Betting limit will be calculated here by multiplying monthly pay by 12. 
                cout<<"Your betting limit is $"<<blimit<<endl; 
                cout<<endl;     // Line skip
            }
            else{ // If neither Y or N was pressed for answer an error will appear leading to the end of the program. 
                cout<<"Error please try again from the start."<<endl;
            }
    }else{          // If a correct value is inputed then it just calculates the betting limit. 
    blimit=mpay*12; // Betting limit will be calculated here by multiplying monthly pay by 12. 
     cout<<"Your betting limit is $"<<blimit<<endl;
     cout<<endl;    //Line skip
    }    
}
// This function is overloaded and for the PASS BET.
float gpass(float &blimit,float bet,char answ,int point,int die1,int sumrll,int die2,int ngames,string name){ // Function for rolling the die.
    ofstream file; // File initializer.
        file.open ("Roll.txt"); // Open file tuna.txt
        if(blimit<=0 || blimit>MAXRAND/1000){ // Another verification of money that will verify the starting betting limit. 
        cout<<"Inputs results do not meet the requirements please try again."<<endl; // This will respond and lead to the end of the program. 
        exit(1);
    }
    else{ // Rolling begins here.
        do{
            cout<<"You are playing PASS BET."<<endl;
            cout<<"Please input your bet for this round your current limit is $"<<blimit<< " and your current 'point' is "<<point<<"."<<endl; // This will display your betting limit and the current point. 
            cin>>bet; // Enter the bet
            if(bet>blimit||bet==0){ // if current bet is more than the betting limit an error will be placed and will ask the user to input a 1 to try again. 
                cout<<"You have wagered more money then you have or have wagered $0, please press Y to try again or any other key to end."<<endl;
                cin>>answ; // Input he answer
            }else{ // If bet is less than the current betting limit. 
            ngames++; // This will increase the number of games played through every cycle.
            die1=rand()%6+1; // [1,6] Die 1
            die2=rand()%6+1; // [1,6] Die 2
            sumrll=die1+die2; // Sum of the 2 die rolled.
            file<<"("<<die1<<")"<<"("<<die2<<")"<<"="<<sumrll<<" Point="<<point<<"     "<<endl; // Right the Die outcomes and the sum roll to the file
                if(sumrll==7||sumrll==11||sumrll==point){ // Winning rolls Issue when you win with point Fix this. 
                    if(point!=0){
                        if (sumrll==7){
                          blimit-=bet;
                          cout<<"Sorry "<<name<<", You Lose! You rolled a "<<sumrll<<" while the point was declared."<<endl<<
                          "Your new Betting limit is $"<<blimit<<endl<<
                          "Will you play again?"<<endl<<"Y||N"<<endl;
                          cin>>answ;
                        }
                        else{
                          blimit+=bet; // Betting limit + the current bet will increase the betting limit
                          cout<<"Congratulations "<<name<<", You Win! You rolled a "<<sumrll<<endl<<
                          "Your new Betting limit is $" << blimit<<endl<< // Out put the new betting limit and prompt the user to input 1 to play again or 2 to leave. 
                          "Will you play again?"<<endl<<"Y||N"<<endl;   
                          cin>>answ;
                        }
                    }
                    else{
                    blimit+=bet; // Betting limit + the current bet will increase the betting limit
                    cout<<"Congratulations "<<name<<", You Win! You rolled a "<<sumrll<<endl<<
                          "Your new Betting limit is $" << blimit<<endl<< // Out put the new betting limit and prompt the user to input 1 to play again or 2 to leave. 
                          "Will you play again?"<<endl<<"Y||N"<<endl;
                    cin>>answ; // Input he answer
                    }
                }else if(sumrll==2||sumrll==3||sumrll==12){
                    blimit-=bet;
                    cout<<"Sorry "<<name<<", You Lose! You rolled a "<<sumrll<<endl<<
                          "Your new Betting limit is $"<<blimit<<endl<<
                          "Will you play again?"<<endl<<"Y||N"<<endl;
                    cin>> answ;
                }else{ // This will prompt to user to play again if a neutral roll is made. 
                    cout<<"You rolled a "<<sumrll<<" if this is your first roll, or first roll after a outcome roll, this will be marked the 'point' if rolled a second time you win."<<endl;
                    cout<<"Roll again?"<<endl;
                    cout<<"Y||N"<<endl;
                    cin>>answ;
                    }
               }
                if(point==0){ //
                 point=sumrll; // The first roll becomes point if rolled again the game ends. 
                }
                if(answ!='Y'&&answ!='y'){ // If answer equal any other value than one the program will say the amount of games played and the amount the player is leaving with. 
                cout<<endl;
                cout<<"Thank you for playing, you played "<<ngames<<" time(s) and the amount you are leaving with is $"<<blimit<<endl<<"Good-Bye"<<endl<<endl;
                } 
                if(sumrll==7||sumrll==11||sumrll==2||sumrll==3||sumrll==12){
                point=0; // This will reset the point value to zero fro the next cycle.
                }
    }while(answ=='Y'||answ=='y');// If the input from the user or program is equal to 1 the loop will be played again with new values. 
        }
        return blimit;
}
// This function is overloaded and used for the DON'T PASS BET.
float gpass(float &blimit,float bet,char answ,int point,int sumrll,vector<int> &array,int size,int ngames,string name,ofstream &file){ // Vector function for Don't pass
        if(blimit<=0 || blimit>MAXRAND/1000){ // Another verification of money that will verify the starting betting limit. 
        cout<<"Inputs results do not meet the requirements please try again."<<endl; // This will respond and lead to the end of the program. 
        exit(1);
    }
    else{ // Rolling begins here.
    do{
        cout<<"You are playing DONT PASS BET."<<endl;
        cout<<"Please input your bet for this round your current limit is $"<<blimit<< " and your current 'point' is "<<point<<"."<<endl; // This will display your betting limit and the current point. 
        cin>>bet; // Enter the bet
        if(bet>blimit||bet==0){ // if current bet is more than the betting limit an error will be placed and will ask the user to input a 1 to try again. 
            cout<<"You have wagered more money then you have or $0, please press Y to try again or any other key to end."<<endl;
            cin>>answ; // Input he answer
        }else{ // If bet is less than the current betting limit. 
        ngames++; // This will increase the number of games played through every cycle.
        adie(array,size,sumrll,point,file); // Dice Function 
                if(sumrll==2||sumrll==3){ // Winning rolls Issue when you win with point Fix this. 
                blimit+=bet; // Betting limit + the current bet will increase the betting limit
                cout<<"Congratulations "<<name<<", You Win! You rolled a "<<sumrll<<endl<<
                      "Your new Betting limit is $" << blimit<<endl<< // Out put the new betting limit and prompt the user to input 1 to play again or 2 to leave. 
                      "Will you play again?"<<endl<<"Y||N"<<endl;
                cin>>answ; // Input he answer
            }else if(sumrll==7||sumrll==11||sumrll==point){
                if(sumrll==7){
                    if(point!=0){
                    blimit+=bet; // Betting limit + the current bet will increase the betting limit
                    cout<<"Congratulations "<<name<<", You Win! You rolled a "<<sumrll<<" while the point was declared."<<endl<<
                          "Your new Betting limit is $" << blimit<<endl<< // Out put the new betting limit and prompt the user to input 1 to play again or 2 to leave. 
                          "Will you play again?"<<endl<<"Y||N"<<endl;
                    cin>>answ;
                    }
                
                    else{
                    blimit-=bet;
                    cout<<"Sorry "<<name<<", You Lose! You rolled a "<<sumrll<<" while the point was declared."<<endl<<
                          "Your new Betting limit is $"<<blimit<<endl<<
                          "Will you play again?"<<endl<<"Y||N"<<endl;
                    cin>> answ;
                    }
                }
                else{
                blimit-=bet;
                cout<<"Sorry "<<name<<", You Lose! You rolled a "<<sumrll<<" while the point was declared."<<endl<<
                      "Your new Betting limit is $"<<blimit<<endl<<
                      "Will you play again?"<<endl<<"Y||N"<<endl;
                cin>> answ;
                point=0;
                }
            }
            else{ // This will prompt to user to play again if a neutral roll is made. 
                cout<<"You rolled a "<<sumrll<<" if this is your first roll, or first roll after a outcome roll, this will be marked the 'point' if rolled a second time you lose."<<endl;
                cout<<"Roll again?"<<endl;
                cout<<"Y||N"<<endl;
                cin>>answ;
                }
           }
            if(point==0){ //
             point=sumrll; // The first roll becomes point if rolled again the game ends. 
            }
            if(answ!='Y'&&answ!='y'){ // If answer equal any other value than one the program will say the amount of games played and the amount the player is leaving with. 
            cout<<endl;
            cout<<"Thank you for playing, you played "<<ngames<<" time(s) and the amount you are leaving with is $"<<blimit<<endl<<"Good-Bye"<<endl<<endl;
            } 
            if(sumrll==7||sumrll==11||sumrll==2||sumrll==3||sumrll==12){
            point=0; // This will reset the point value to zero fro the next cycle.
            }
}while(answ=='Y'||answ=='y');// If the input from the user or program is equal to 1 the loop will be played again with new values. 
    }
        return blimit;
}
float adie(vector<int> &array,int size,int &sumrll,int point,ofstream &file){ // Function for an array dice.
    file.open ("Roll.txt",ios::app); // Open file tuna.txt with ios::app to write at the end of the file. 
    sumrll=0;
    for(int i=0;i<size;i++)
    {
        array[i]=rand()%6+1;
        file<<"("<<array[i]<<")";
        sumrll+=array[i];
    }
    file<<"="<<sumrll<<"     "<<"Point="<<point<<endl;
    cout<<"Roll ="<<sumrll<<"     "<<"Point="<<point<<endl;
    file.close();
}
void frank(float hscore[],int pool){
    cout<<"Selection Sort of Todays 50 High Scores:"<<endl;
    for(int i=0;i<pool;i++){
    float pscore=rand()%200001*(8/3);
    hscore[i]=pscore;
    }
}
void selsort(float hscore[],int pool){
for(int pos=0;pos<pool-1;pos++){
    int indx=pos;
    for(int i=pos+1;i<pool;i++){
        if(hscore[indx]<hscore[i]){
            indx=i;
        }
    }   
    int temp=hscore[indx];
    hscore[indx]=hscore[pos];
    hscore[pos]=temp;
    }
for(int i=0;i<pool;i++){
   cout<<i+1<<". "<<hscore[i]<<endl; 
}
}
void bublsrt(float hscore[],int pool,float blimit){ // Broken
    cout<<"Bubble Sort Todays 50 :"<<endl;
    bool swap;
    do{
        swap=false;
        for(int i=0;i<pool-1;i++){
            if(hscore[i]>hscore[i+1]){
                int temp=hscore[i+1];
                hscore[i]=temp;
                swap=true;
            }
        }
    }while(swap);
    for(int i=0;i<pool;i++){
        cout<<i+1<<". "<<hscore[i]<<endl;
    }
}
void lsearch(float hscore[],int pool,int indx,float blimit){
    for(int i=0;i<pool;i++){
            if(blimit>=hscore[i]){
            cout<<"You are the new rank number: "<<indx+1<<endl;
            exit(1);
        }
            if(i==49){
        if(blimit<hscore[49]){
                cout<<"You did not score high enough to place."<<endl;
        }
            indx++;
        }
    }
}