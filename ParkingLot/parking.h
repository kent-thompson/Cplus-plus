#ifndef PARKING_H
#define PARKING_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <vector>
#include <memory>

#include "vehicleType.h"
#include "vehicle.h"

class ParkingSpot
{
private:
    std::shared_ptr<Vehicle> mpVehicle;
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
        std::cout << "Number of Motorcycles: " << curMcCount << std::endl;
        std::cout << "Number of Cars: " << curCarCount << std::endl;
        std::cout << "Number of Vans: " << curVanCount << std::endl;  
    }

    // use spots
    bool parkVehicle( std::shared_ptr<Vehicle> pv ) {  
        bool flag = false;
        if( isFull() ) {
            std::cout << "Parking Lot Full" << std::endl;
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
            std::cout << i.getLabel() << " Spot " << i.getSpotNbr() << " has a " << i.getLabel() << std::endl;
        }
    }
}; //ParkingLot

#endif