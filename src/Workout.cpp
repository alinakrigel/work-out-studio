#include "Workout.h"
#include <string>
#include <utility>


Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type) : id(w_id), name(w_name),
                                                                                  price(w_price), type(w_type) {}

int Workout::getId() const {
    return id;
}

std::string Workout::getName() const {
    return name;
}

int Workout::getPrice() const {
    return price;
}

WorkoutType Workout::getType() const {
    return type;
}

// Returns a string of the kind of type
std::string Workout::getStrType() {
    if (type == ANAEROBIC)
        return "ANAEROBIC";
    else if (type == MIXED)
        return "MIXED";
    else
        return "CARDIO";

}


