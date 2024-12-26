#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <sys/types.h>
#include <vector>
#include <iterator>
#include <memory>
using namespace std; // only ever used in examples

enum struct vehicleType { Motorcycle, Car, Van };
class ParkingSpot;  // forward declaration

//--------------------
class Vehicle       //  base class
{
public:
    Vehicle() {};
    virtual ~Vehicle() {};

protected:
    vehicleType type;
    ushort spotsNeeded;
    std::string label;

public:
    virtual ushort getSpotsNeeded() { return spotsNeeded; }
    virtual vehicleType getType() { return type; }
    virtual std::string& getLabel() { return label; }

};//Vehicle

class Van : public Vehicle {
public:
   Van() {
    spotsNeeded = 3;
    type = vehicleType::Van;
    label = "Van";
  }
    virtual ~Van() {};
};//Van

class Car : public Vehicle {
public:
   Car() {
    spotsNeeded = 1;
    type = vehicleType::Car;
        label = "Car";
  }
   virtual ~Car() {};
};//Car

class Motorcycle : public Vehicle {
public:
   Motorcycle() {
    spotsNeeded = 1;
    type = vehicleType::Motorcycle;
    label = "Motorcycle";    
  }
   virtual ~Motorcycle() {};
};//Motorcycle


//--------------------
class ParkingSpot
{
private:
    shared_ptr<Vehicle> mpVehicle;
    ushort spotNbr;
    vehicleType type; 

public:
  ParkingSpot( std::shared_ptr<Vehicle> pv ) {
        mpVehicle = std::move( pv );
    }
  ~ParkingSpot() {};

  ushort getSpotNbr() { return spotNbr; }
  vehicleType getType() { return type; }

  void setData(  ushort spotNbr_, vehicleType vt ) {
    spotNbr = spotNbr_;
    type = vt;
  }
    // Vehicle* getVehiclePtr() { return mpVehicle; }

    vehicleType getVehicleType() { return mpVehicle->getType(); }
    std::string& getLabel() { return mpVehicle->getLabel(); }
    bool canFitInSpot() { return mpVehicle->getSpotsNeeded() == spotsNeeded( mpVehicle->getType() ); } 

    ushort spotsNeeded( vehicleType vt ) {   // code review fodder :)
        switch( vt ) {
        case vehicleType::Van:
            return 3;
        case vehicleType::Car:
        case vehicleType::Motorcycle:
            return 1;
        }
    }
}; //ParkingSpot


//--------------------
class ParkingLot
{
public:
  ParkingLot() {};
  ~ParkingLot() {};

 private:
    const ushort max_spots = 23;
    const ushort max_mc_spots = 4;
    const ushort max_car_spots = 10;
    const ushort max_van_spots = 3; // * 3
    std::vector<ParkingSpot> spots;
    ushort curMcCount, curCarCount, curVanCount = 0;

    public:
    int spotsAvailable() {
        return max_spots - (curMcCount + curCarCount + (curVanCount * 3));
    }

    ushort getTotalSpots() { return max_spots; }
    bool isEmpty() { return spots.size() == 0 ? true : false; }
    bool isFull() { return spotsAvailable() < 1 ? true : false; }

    void printSpotsMetrics() {
        cout << "Number of Motorcycles: " << curMcCount << endl;
        cout << "Number of Cars: " << curCarCount << endl;
        cout << "Number of Vans: " << curVanCount << endl;  
    }

    bool parkVehicle( std::shared_ptr<Vehicle> pv ) {  
        bool flag = false;
        if( isFull() ) {
            cout << "Parking Lot Full" << endl;
            return flag;
         }

        ParkingSpot sp( pv );
        switch( pv->getType() ) {
        case vehicleType::Van:
            if( curVanCount < max_van_spots ) {
                sp.setData( ++curVanCount, pv->getType() );
                flag = true;
            }
            break;
        case vehicleType::Car:
            if( curCarCount < max_car_spots ) {
                sp.setData( ++curCarCount, pv->getType() );
                flag = true;
            }
            break;
        case vehicleType::Motorcycle:
            if( curMcCount < max_mc_spots ) {
                sp.setData( ++curMcCount, pv->getType() );
                flag = true;
            }
            break;
        }
        
        if( flag == true ) {
            spots.push_back( sp );
        }
        return flag;
    }

    void printInfo() {
        for( auto i : spots ) {
            cout << i.getLabel() << " Spot " << i.getSpotNbr() << " has a " << i.getLabel() << endl;
        }
    }
}; //ParkingLot


int main() 
{
    ParkingLot parkingLot;
    cout << "Spots Avaiable: " << parkingLot.spotsAvailable() << endl;
    cout << "Is Parking Lot Empty? " << parkingLot.isEmpty() << endl;

    // Interesting below, eh? To Support Polymorphism. TODO: Could make a Class Factory (a Vehicle Factory, Ha!) where you pass in a vehicleType and get the correct pointer back.
    shared_ptr<Vehicle> mc1 = make_shared<Motorcycle>();
    parkingLot.parkVehicle( mc1 );

    shared_ptr<Vehicle> mc2 = make_shared<Motorcycle>();
    parkingLot.parkVehicle( mc2 );

    shared_ptr<Vehicle> car1 = make_shared<Car>();
    parkingLot.parkVehicle( car1 );

    shared_ptr<Vehicle> van1 = make_shared<Van>();
    parkingLot.parkVehicle( van1 );

    cout << "After parking some vehicles" << endl;
    parkingLot.printSpotsMetrics();
    cout << "Spots Avaiable: " << parkingLot.spotsAvailable() << endl;
    cout << "Is Parking Lot Empty? " << parkingLot.isEmpty() << endl;
    cout << "Is Parking Lot Full? " << parkingLot.isFull() << endl;

    parkingLot.printInfo();

    return 0;
}

// TODO:
// bool removeVehicleFromSpot( Vehicle& v ) {
//   bool flag = false;
//   ParkingSpot sp;

//   switch( v.getType() ) {
//   case vehicleType::Van:
//       if( curVanCount < max_van_spots ) {
//         sp.setData( ++curVanCount, v.getType() );
//         flag = true;
//       }
//       break;
//   case vehicleType::Car:
//       if( curCarCount < max_car_spots ) {
//         sp.setData( ++curCarCount, v.getType() );
//         flag = true;
//       }
//       break;
//  case vehicleType::Motorcycle:
//       if( curMcCount < max_mc_spots ) {
//         sp.setData( ++curMcCount, v.getType() );
//         flag = true;
//       }
//       break;
//   }
//   if( flag == true ) {
//     spots.push_back( sp );
//   }
//   return flag;
//}