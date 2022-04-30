#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

class Customer {
public:
    // Constructor
    Customer(std::string c_name, int c_id);

    // Destructor (default)
    virtual ~Customer() = default;

    virtual std::vector<int> order(const std::vector<Workout> &workout_options) = 0;

    virtual std::string toString() const = 0;

    virtual Customer *clone() const = 0;// Creating a deep copy of each specific costumer

    std::string getName() const;

    int getId() const;

private:
    const std::string name;
    const int id;
};


class SweatyCustomer : public Customer {
public:

    // Constructor
    SweatyCustomer(std::string name, int id);

    std::vector<int> order(const std::vector<Workout> &workout_options);

    std::string toString() const;

    virtual SweatyCustomer *clone() const;

private:
};


class CheapCustomer : public Customer {
public:

    // Constructor
    CheapCustomer(std::string name, int id);

    std::vector<int> order(const std::vector<Workout> &workout_options);

    std::string toString() const;

    virtual CheapCustomer *clone() const;

private:
    bool Hasordered; // CheapCustomer can't order more the one time
};


class HeavyMuscleCustomer : public Customer {
public:

    // Constructor
    HeavyMuscleCustomer(std::string name, int id);

    std::vector<int> order(const std::vector<Workout> &workout_options);

    std::string toString() const;

    virtual HeavyMuscleCustomer *clone() const;

private:
};


class FullBodyCustomer : public Customer {
public:

    // Constructor
    FullBodyCustomer(std::string name, int id);

    std::vector<int> order(const std::vector<Workout> &workout_options);

    std::string toString() const;

    virtual FullBodyCustomer *clone() const;

private:
};


#endif