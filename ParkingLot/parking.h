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
    //vehicleType type; 

public:
    ParkingSpot( std::shared_ptr<Vehicle> pv ) {
        mpVehicle = std::move( pv );
    }
    ~ParkingSpot() {};

    inline ushort getSpotNbr() { return spotNbr; }
    inline vehicleType getVehicleType() { return mpVehicle->getType(); }
    inline std::string& getLabel() { return mpVehicle->getLabel(); }

    inline bool canFitInSpot() { return mpVehicle->getSpotsNeeded() == spotsNeeded( mpVehicle->getType() ); } 
    inline void setSpot(  ushort spotNbr_) { spotNbr = spotNbr_; }

    /* NOTE: "spots" represent the smallest sized space that a 'vehicle' occupies. Different vehicles use different numbers of spots. */
    ushort spotsNeeded( vehicleType vt ) {   // code review fodder :)
        switch( vt ) {
        case vehicleType::Van:
            return 3;
        case vehicleType::Car:
            return 2;
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
    const ushort MAX_SPOTS = 24;
    std::vector<ParkingSpot> spots;
    ushort currSpotCount = 0;
    ushort mMcCnt = 0, mCarCnt = 0, mVanCnt = 0;

    public:
    int spotsAvailable() {
        return MAX_SPOTS - calcSpotsUsed();
    }

//--------------------
    inline ushort getTotalSpots() { return MAX_SPOTS; }
    inline bool isEmpty() { return spots.size() == 0 ? true : false; }
    inline bool isFull() { return spotsAvailable() < 1 ? true : false; }


//--------------------
// RETURNS total spots used
    ushort calcSpotsMetrics() {
        mCarCnt = mMcCnt = mVanCnt = 0;
        ushort cnt = 0;

        for( auto spot : spots ) {
            switch( spot.getVehicleType() ) {
            case vehicleType::Van:
                ++mVanCnt;
                cnt += 3;
                break;
            case vehicleType::Car:
                ++mCarCnt;
                cnt += 2;
                break;
            case vehicleType::Motorcycle:
                ++mMcCnt;
                cnt += 1;
                break;
            }
        }
        return cnt;
    }


//--------------------
// Deprecated
    ushort calcSpotsUsed() {
            ushort cnt = 0;
        for( auto v : spots ) {
            switch( v.getVehicleType() ) {
            case vehicleType::Van:
                cnt += 3;
                break;
            case vehicleType::Car:
                cnt += 2;
                break;
            case vehicleType::Motorcycle:
                cnt += 1;
                break;
            }
        }
        return cnt;
    }


//--------------------
    // use up spots
    bool parkVehicle( std::shared_ptr<Vehicle> pv ) {
        // TODO: phase 2: use "first avilable" technique to get spot
        if( isFull() ) {
            std::cout << "Parking Lot Full" << std::endl;
            return false;
        }

        ushort usedSpots = calcSpotsMetrics();
        if( pv->getSpotsNeeded() + usedSpots <= MAX_SPOTS ) {
            ParkingSpot sp( pv ); 
            sp.setSpot( ++usedSpots );
            spots.push_back( sp );
            return true;
        } else {
            return false;
        }
    }


//--------------------
    bool removeVehicle( std::shared_ptr<Vehicle> v ) {
        //ParkingSpot sp;

        for( auto it = spots.begin(); it != spots.end(); ++it ) {
            if( it->getVehicleType() == v->getType() ) {
                spots.erase( it );

            }
        }
        return true;
    }


//--------------------
    void printInfo() {
         std::string s;

    ushort cnt = calcSpotsMetrics();
        std::cout << "Total Spots Used " << cnt << '\n';
         std::cout << "Spots Avaiable: " << spotsAvailable() << std::endl;
         
        std::cout << "Number of Motorcycles: " << mMcCnt << '\n';
        std::cout << "Number of Cars: " << mCarCnt << '\n';
        std::cout << "Number of Vans: " << mVanCnt << std::endl;

        std::cout << "*** Lot List ***" << '\n';
        for( auto i : spots ) {
            std::cout << "Spot " << i.getSpotNbr() << " has a " << i.getLabel() << '\n';
        }

        s = (isEmpty()) ? "True" : "False";
         std::cout << "Is Parking Lot Empty? " << s << '\n';
        s = (isFull()) ? "True" : "False";
         std::cout << "Is Parking Lot Full? " << s << '\n';
        std::cout << "|-----------------------" << '\n';
        std::cout << std::endl;
    }
}; //ParkingLot

#endif