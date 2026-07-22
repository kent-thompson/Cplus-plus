
#include "vehicle.h"
#include "parking.h"

using namespace std; // only ever used in examples

int main() 
{
    ParkingLot parkingLot;
    cout << "Total Spots In Lot: " << parkingLot.spotsAvailable() << endl;
    string s = (parkingLot.isEmpty()) ? "True" : "False";
    cout << "Is Parking Lot Empty? " << s << endl;

    //NOTE below: Interesting Way to support Polymorphism with shared_ptrs.
    // TODO: Could make a Class Factory (a Vehicle Factory, Ha!) where a vehicleType is passed in and the correct pointer is returned
    shared_ptr<Vehicle> van1 = make_shared<Van>();
    parkingLot.parkVehicle( van1 );

    shared_ptr<Vehicle> mc1 = make_shared<Motorcycle>();
    parkingLot.parkVehicle( mc1 );
    shared_ptr<Vehicle> mc2 = make_shared<Motorcycle>();
    parkingLot.parkVehicle( mc2 );

    shared_ptr<Vehicle> car1 = make_shared<Car>();
    parkingLot.parkVehicle( car1 );
    shared_ptr<Vehicle> car2 = make_shared<Car>();
    parkingLot.parkVehicle( car2 );
    // NOTE: "spots" represent the smallest sized space that a 'vehicle' occupies. Different vehicles take different numbers of spots.
    cout << "After parking some vehicles..." << endl;
    parkingLot.printInfo();

    cout << "Removing a Car..." << endl;
    parkingLot.removeVehicle( car1 );
        //cout << "Spots Avaiable: " << parkingLot.spotsAvailable() << endl;
        parkingLot.printInfo();
    //cout << "DONE" << endl;


    return 0;
}
