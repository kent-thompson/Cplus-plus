#ifndef VEHICLES_H
#define VEHICLES_H

#include <string>
#include <sys/types.h>
#include "vehicleType.h"

//Vehicle  base class and 3 derived classes, Van, Car, Mototcycle
//--------------------
class Vehicle   //  base class
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
    spotsNeeded = 2;
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

#endif