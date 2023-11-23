#include "std_lib_facilities.h"

double Timer(double distance, double speed, double racer_distance, double acceleration){
    double discriminant, root1, root2;                                              //formula: Distance = Racer_Distance + Speed * Time + 1/2 * Acceleration * Time^2
    discriminant = pow(speed,2) - (2*acceleration * (racer_distance - distance));  //calculates the discriminant of the formula
    if(discriminant>0){                                                           //if discriminant>0, then we have 2 roots and we will return the positive root 
        root1 = (-speed + sqrt(discriminant))/acceleration;                      //root is the time of the racer 
        root2 = (-speed - sqrt(discriminant))/acceleration;         
        if(root1 > 0){
            return root1;
        }else if (root2 > 0){
            return root2;
        }else{
            throw logic_error("Bad input");
        }
    }else if(discriminant==0){                                                //if root is equal to zero, we have only one root 
        root1 = -speed/acceleration;  
        return root1;                                       
    }else{                                                                   //if root is negative then we used wrong inputs
        throw logic_error("Bad input");                                                  
    }                                                                                                            
}

void SortFun(int racers, vector<double>time, vector<int>position){
    int i,j;
    double tmp;
    for(i=0; i<racers; i++){                                            //sorts the times of the racers 
        for(j=i+1; j<racers; j++){
            if(time[j]<time[i]){
                tmp = time[i];
                time[i] = time[j];
                time[j] = tmp;
                tmp = position[i];                                      //keeps the position of each racer 
                position[i] = position[j];
                position[j] = tmp;
            }
        }
    }
    for(i=0; i<racers; i++){
        cout << i+1 << "." << " Racer" << position[i] << " " << "Time: " << time[i] << endl;      //prints the results of the race 
    }
}

int main(){
    int racers, i, acceleration;
    double distance, speed, racer_distance,result;
    cout << "Please enter the number of racers and the total distance: ";
    cin >> racers >> distance;                                                       //takes the number of racers and the total distance 
    vector<double>time;
    vector<int>position;
    try{
        if(racers>0){                                                               //checks if racers are not a negative number 
            for(i=0; i<racers; i++){
                cout << "\nPlease enter the speed, the travelled distance and the acceleration of the racer: ";
                cin >> speed >> racer_distance >> acceleration;                         //racer_distance is the distance that the racer has travelled                                                          
                if((acceleration>0) && (distance>=racer_distance) && (speed>0)){              //checks if the acceleration the racer_distance and the speed are correct values
                    result = Timer(distance, speed, racer_distance, acceleration);      //the Timer returns the time of each racer
                    time.push_back(result);                                             //saves the time of each racer
                    position.push_back(i+1);                                              //saves the position of each racer         
                }else{
                    throw logic_error("Bad input");
                }                   
            }
            SortFun(racers, time, position);                                           //SortFun is sorting the times of each racer
        }else{
            throw logic_error("Bad input");
        }      
    }
    catch(std::logic_error){
        cerr << "\nBad input!";
    }
} 