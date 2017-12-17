/*******************************************************************************
 [*] ToDo1:     Implement MetroTrain operate for N stations
 [*] 1.5:       Implement MetroTrain inStation
 [ ] 1.6:       Put first the public part in every class
 [*] ToDo2:     Passenger Constructor first implementation
 [*] 2.3:       Change Passenger to PassengerSeat and hold if the seat is empty
 [*] 2.4:       REVERT! A seat is empty when set to nullptr. Passengers should
                be created during Wagon's embarkation
 [*] 2.45:      Wagon's embarkation() first implementation
 [ ] 2.47:      Wagon's disembarkation() first implementation
 [*] 2.48:      Modify embarkation() to check in a loop every seat if it is empty
 [ ] 2.5:       Passenger Destructor: do I need one?
 [*] 2.6:       Modify Passenger constructor calls since now it accepts arguments
 [ ] 2.8:       Implement MetroTrain betweenStations
 [ ] ToDo3:     Study more examples of dynamic memory allocation, pointers to 
                pointers etc
 [ ] ToDo4:     Get the stationsCount from command line
 [ ] ToDo5:     Delete m_currentStation from MetroTrain class if not needed
 [ ] ToDo6:     Decide if stationsCount will be given in MetroTrain constructor or
                in operate 
 [ ] ToDo7:     Create an accessor and a mutator for Passenger's busted variable

*******************************************************************************/
#include <iostream>

using std::cout;


#include <cassert>

// Returns randomly true with a given probability between 0 and 1
// Assumes srand() has already been called
bool randomBoolWithProb(double probability)
{
    assert(probability>=0 && probability<=1);
    
    return rand()/(RAND_MAX+1.0) < probability;
}


// Generate a random number between min and max (inclusive)
// Assumes srand() has already been called
// Assumes max - min <= RAND_MAX
int getRandomNumber(int min, int max)
{
    // static used for efficiency, so we only calculate this value once
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);  
    // evenly distribute the random number across our range
    return min + static_cast<int>((max - min + 1) * (rand() * fraction));
}

class Passenger
{
private:
    bool m_isEmpty;         //ToDo2.4
    bool m_hasTicket;
    bool m_rightForReducedTicket;
    //When the train gets to disembarkStation station, get off the train
    //if busted betweenStations then disembarkStation = nextStation
    int m_disembarkStation; 
    //if !hasTicket when ticketInspector enters the Waggon then busted=true
    bool m_busted;        //ToDo7: I'll need an accessor and a mutator for this
    
public:
    //Temporary default constructor till constructor calls modification
    Passenger() { } 
    Passenger
    (
        const int disembarkStation,
        const double probHasTicket = 0.5,
        const double probReducedTicket = 0.5
    ):
        m_disembarkStation(disembarkStation),
        m_hasTicket(randomBoolWithProb(probHasTicket)),
        m_rightForReducedTicket(randomBoolWithProb(probReducedTicket)),
        m_busted(false)
    {
        static int passengerId(0);
        passengerId++;
        cout << "New Passenger with id: " 
            << passengerId << " and destination: " << m_disembarkStation << "\n";
        
    }
    
    ~Passenger()
    {
        
    }
    
};

class Wagon
{
private:
    Passenger** m_passengers;    //Pointer to a passengers array
    
    //Zero initialized at the constructor. Has to stay smaller than maxCapacity
    int m_passengersCount; 
    int m_maxCapacity;

public:
    //This constructor should create a wagon with empty Passenger seats 
    //An empty seat is marked with a nullptr.
    Wagon(int maxCapacity): 
        m_maxCapacity(maxCapacity),
        m_passengersCount(0)
    {
        m_passengers = new Passenger*[maxCapacity];
        for(int i(0); i<maxCapacity; i++)
        {
            // m_passengers[i] = new Passenger(7); //ToDo: New argument
            m_passengers[i] = nullptr;
        }
        //Old: m_passengers = new Passenger[maxCapacity];
        cout << "A wagon with capacity for " << maxCapacity  
            << " passengers, was created \n";
    }
    
    ~Wagon()
    {
        for(int i(0); i<m_maxCapacity; i++)
        {
            delete m_passengers[i];
        }
        delete[] m_passengers;
        
        cout << "A wagon was destroyed \n";
    }
    
    //Disembark the Passengers whose disembarkStation = currentStation.
    //Passenger objects will be deleted here after disembarkation
    void disembarkation()
    {
        /***********************************************************************
         * ToDo2.47:
         * Loop through all the Passenger seats of the current Wagon's instance
         * if the seat is not empty: m_passengers[i] != nullptr
         * && if m_passengers[i]->getDisembarkStation() == currentStation
         * Then delete m_passengers[i] and m_passengers[i] = nullptr
         * 
        /**********************************************************************/
    }
    
    //Here create and embark a random count of new Passengers
    void embarkation(const int &currentStation, const int &stationsCount)
    {
        int emptySeatsCount = (m_maxCapacity - m_passengersCount);
        int newPassengersMax = getRandomNumber(0, emptySeatsCount);
        
        cout << emptySeatsCount << " empty seats. \t" << newPassengersMax
            << " new passengers. \t" << m_passengersCount << " pre-existing passengers. \n";
        for
        (
            int seat(0), newPassengers(0); 
            seat<(m_maxCapacity) && newPassengers<newPassengersMax; 
            seat++
        )
        {
            if(m_passengers[seat] == nullptr)
            {
                int destination = getRandomNumber(currentStation+1, stationsCount);
                m_passengers[seat] = new Passenger(destination);
                m_passengersCount++;
                newPassengers++;
            }
        }
        
    }
    

    //inStation(currentStation) Probably I'll implement this in MetroTrain class
};

class MetroTrain
{
private:
    //Pointer to a dynamically allocated array of pointers to wagons
    Wagon** m_wagons;       
    int m_wagonsCount;      //Length of the array. Represents the wagons count
    
    //ToDo5: Delete if redundant
    //Holds the current station id every time the train stops. Initialized to 0
    int m_currentStation;
    //int nextStation;
    
    //Money from passengers paying a fine after being found without a ticket.
    int m_totalRevenue;
    
    
    //Here I need to know if this is the last station in order to disembark all
    void inStation
    (const int &currentStation, const int &stationsCount)
    {
        cout << "inStation: " << currentStation << "\n";
        
        const bool isFirstStation = (currentStation == 1);
        if(!isFirstStation)                     //Disembark first 
        {
            //Here loop through the wagons and call Wagon's disembarkation()
            for(int i(0); i<m_wagonsCount; i++)
            {
                m_wagons[i]->disembarkation();
            }
        }
        if(currentStation!=stationsCount)       //Then embark if not last station
        {
            //Here loop through the wagons and call Wagon's embarkation()
            for(int i(0); i<m_wagonsCount; i++)
            {
                cout << "Wagon " << i+1 << " / " << m_wagonsCount << "\n";
                m_wagons[i]->embarkation(currentStation, stationsCount);
            }
        }
        
    }
    
    void betweenStations()
    {
        //Tickets inspection process gets triggered here
    }
    
    
public:
    MetroTrain(int wagonsCount, int wagonMaxCapacity): 
        m_wagonsCount(wagonsCount), m_currentStation(0), m_totalRevenue(0)
    {
        m_wagons = new Wagon*[wagonsCount];
        for(int i=0; i<wagonsCount; i++)
        {
            m_wagons[i] = new Wagon(wagonMaxCapacity);
        }
        
        cout << "A metro train with " << wagonsCount << " wagons, was created \n";
    }
    
    ~MetroTrain()
    {
        for(int i=0; i<m_wagonsCount; i++)
        {
            delete m_wagons[i];
        }
        delete[] m_wagons;
        //we don't need to set m_wagons to null or m_wagonsCount to 0 here, 
        //since the object will be destroyed immediately after this function anyway
        
        cout << "A metro train was destroyed \n";
    }
    
    void operate(int stationsCount)
    {
        cout << "Train operates now \n";
        for(int i(1); i<=stationsCount; i++)
        {
            int &currentStation = i;    //Just an alias
            
            bool isLastStation = (currentStation == stationsCount) ;
            inStation(currentStation, stationsCount);
            if(!isLastStation)
            {
                betweenStations();
            }
        }
    }
    
    void printStatistics()
    {
        
    }
        
};

int main()
{
    srand(time(0));         //In order to use rand in many different places
    const int wagonsCount(10), wagonMaxCapacity(30), stationsCount(7);
    
    //Create a metroTrain that will operate for N stations
    MetroTrain metroTrain1(wagonsCount, wagonMaxCapacity);
    
    metroTrain1.operate(stationsCount);
    
    return 0;
}
