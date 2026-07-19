
#include "vehicle.h"
#include "parking.h"

using namespace std; // only ever used in examples

int main() 
{
    ParkingLot parkingLot;
    cout << "Spots Avaiable: " << parkingLot.spotsAvailable() << endl;
    cout << "Is Parking Lot Empty? " << parkingLot.isEmpty() << endl;

    // Interesting below, to support Polymorphism with shared_ptrs.
    // TODO: Could make a Class Factory (a Vehicle Factory, Ha!) where a vehicleType is passed in and the correct pointer is returned
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
    //cout << "DONE##" << endl;

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