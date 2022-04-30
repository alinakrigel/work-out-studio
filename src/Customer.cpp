#include <vector>
#include <string>
#include "Workout.h"
#include "Customer.h"
#include <algorithm>

// Constructor
Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id) {}

std::string Customer::getName() const {
    return name;
}

int Customer::getId() const {
    return id;
}

// Constructor
SweatyCustomer::SweatyCustomer(std::string name, int id) : Customer(name, id) {}

SweatyCustomer *SweatyCustomer::clone() const {
    return new SweatyCustomer(getName(), getId());
}

// Making an order according to specific strategy
std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> UniqueWorkout;
    for (size_t i = 0; i < workout_options.size(); i++) { //Ordering all cardio activities for this costumer
        if (workout_options[i].getType() == 2)
            UniqueWorkout.push_back(workout_options[i].getId());
    }
    return UniqueWorkout;
}

std::string SweatyCustomer::toString() const {
    return "swt";
}

// Constructor
CheapCustomer::CheapCustomer(std::string name, int id) : Customer(name, id), Hasordered(false) {}

CheapCustomer *CheapCustomer::clone() const {
    return new CheapCustomer(getName(), getId());
}

// Making an order according to specific strategy
std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options) {
    if (!Hasordered) { // Cheap customer can only order once
        std::vector<int> UniqueWorkout;
        int MinWorkoutId = workout_options[0].getId(); // Finding the cheapest workout by simple linear search
        int MinPrice = workout_options[0].getPrice();
        for (size_t i = 1; i < workout_options.size(); i++) {
            if (workout_options[i].getPrice() < MinPrice) {
                MinWorkoutId = workout_options[i].getId();
                MinPrice = workout_options[i].getPrice();
            }
        }
        UniqueWorkout.push_back(MinWorkoutId);
        Hasordered = true;
        return UniqueWorkout;
    } else
        return std::vector<int>();
}

std::string CheapCustomer::toString() const {
    return "chp";
}

// Constructor
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id) : Customer(name, id) {}

HeavyMuscleCustomer *HeavyMuscleCustomer::clone() const {
    return new HeavyMuscleCustomer(getName(), getId());
}

// Making an order according to specific strategy
std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options) {
    std::vector<int> UniqueWorkout;
    typedef std::pair<int, int> temp;// Creating a pair of the information we need in order to get the workout price from expensive to cheap
    std::vector<temp> SortWorkout = std::vector<temp>();
    for (size_t i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == 0)
            SortWorkout.push_back(std::make_pair(workout_options[i].getId(), workout_options[i].getPrice()));
    }
    while (!SortWorkout.empty()) {
        int max = -1;
        int maxId = -1;
        int Index = -1;
        for (size_t i = 0; i < SortWorkout.size(); i++) {
            temp pair = SortWorkout[i];
            if (max == -1 || max < pair.second || (max == pair.second && maxId < pair.first)) {
                max = pair.second;
                maxId = pair.first;
                Index = i;
            }
        }
        SortWorkout.erase(SortWorkout.begin() + Index);
        UniqueWorkout.push_back(maxId);
    }
    return UniqueWorkout;
}

std::string HeavyMuscleCustomer::toString() const {
    return "mcl";
}

// Constructor
FullBodyCustomer::FullBodyCustomer(std::string name, int id) : Customer(name, id) {}

FullBodyCustomer *FullBodyCustomer::clone() const {
    return new FullBodyCustomer(getName(), getId());
}

// Making an order according to specific strategy
std::vector<int> FullBodyCustomer::order(
        const std::vector<Workout> &workout_options) {// We will separate each workout type in a different loop
    std::vector<int> UniqueWorkout;
    typedef std::pair<int, int> temp;
    std::vector<temp> SortWorkoutCardio = std::vector<temp>();// Making temporary vectors for the function use
    std::vector<temp> SortWorkoutMix = std::vector<temp>();
    std::vector<temp> SortWorkoutAnerobic = std::vector<temp>();
    for (size_t i = 0; i < workout_options.size(); i++) {
        if (workout_options[i].getType() == 2)
            SortWorkoutCardio.push_back(std::make_pair(workout_options[i].getId(), workout_options[i].getPrice()));
        else if (workout_options[i].getType() == 1)
            SortWorkoutMix.push_back(std::make_pair(workout_options[i].getId(), workout_options[i].getPrice()));
        else
            SortWorkoutAnerobic.push_back(std::make_pair(workout_options[i].getId(), workout_options[i].getPrice()));
    }
    int MinPrice = SortWorkoutCardio[0].second;
    int MinIndex = SortWorkoutCardio[0].first;
    // Searching minimum price of cardio workout
    for (size_t i = 1; i < SortWorkoutCardio.size(); i++) {
        if (SortWorkoutCardio[i].second < MinPrice) {
            MinPrice = SortWorkoutCardio[i].second;
            MinIndex = SortWorkoutCardio[i].first;
        }
    }
    UniqueWorkout.push_back(MinIndex);
    int MaxPrice = SortWorkoutMix[0].second;
    int MaxIndex = SortWorkoutMix[0].first;
    // Searching minimum price of mix workout
    for (size_t i = 1; i < SortWorkoutMix.size(); i++) {
        if (SortWorkoutMix[i].second > MaxPrice) {
            MaxPrice = SortWorkoutMix[i].second;
            MaxIndex = SortWorkoutMix[i].first;
        }
    }
    UniqueWorkout.push_back(MaxIndex);
    int minPrice = SortWorkoutAnerobic[0].second;
    int minIndex = SortWorkoutAnerobic[0].first;
    // Searching minimum price of anaerobic workout
    for (size_t i = 1; i < SortWorkoutAnerobic.size(); i++) {
        if (SortWorkoutAnerobic[i].second < minPrice) {
            minPrice = SortWorkoutAnerobic[i].second;
            minIndex = SortWorkoutAnerobic[i].first;
        }
    }
    UniqueWorkout.push_back(minIndex);
    return UniqueWorkout;
}

std::string FullBodyCustomer::toString() const {
    return "fbc";
}


