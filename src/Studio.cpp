#include "Studio.h"
#include <iostream>
#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <boost/algorithm/string/split.hpp> // Include for boost::split#include <boost/algorithm/string/classification.hpp> // Include boost::for is_any_of
#include <fstream>

// Constructor
Studio::Studio(const std::string &configFilePath) {
    open = false;
    std::ifstream file(configFilePath);
    std::string str;
    int lineCounter = 1;
    int workoutIdCounter = 0;
    while (std::getline(file, str)) {
        if (lineCounter == 5) {
            std::vector<std::string> trainersCapacity;
            boost::split(trainersCapacity, str, boost::is_any_of(","), boost::token_compress_on);
            // Loop for creating all trainers
            for (size_t id = 0; id < trainersCapacity.size(); ++id) {
                int trainerId = std::stoi(trainersCapacity[id]);
                Trainer *trainer = new Trainer(trainerId);
                trainers.push_back(trainer);
            }
        } else if (str != "" && lineCounter > 7) { // Creating all workouts
            std::vector<std::string> workout;
            boost::split(workout, str, boost::is_any_of(","), boost::token_compress_on);
            std::string workoutName = workout[0];
            std::string type = workout[1].substr(1);
            int workoutPrice = std::stoi(workout[2].substr(1));
            WorkoutType workoutType;
            if (type == "Cardio")
                workoutType = CARDIO;
            else if (type == "Mixed")
                workoutType = MIXED;
            else
                workoutType = ANAEROBIC;
            workout_options.push_back(Workout(workoutIdCounter, workoutName, workoutPrice, workoutType));
            workoutIdCounter++;
        }
        lineCounter++;
    }
}

// Destructor
Studio::~Studio() {
    clear();
}

// Copy Constructor
Studio::Studio(const Studio &other) {
    copy(other.open, other.trainers, other.workout_options, other.actionsLog, other.customersIdCounter);
}

// Move Constructor
Studio::Studio(Studio &&other) : open(other.open), trainers(std::move(other.trainers)),
                                 workout_options(std::move(other.workout_options)),
                                 actionsLog(std::move(other.actionsLog)), customersIdCounter(other.customersIdCounter) {
    other.open = false;
    other.customersIdCounter = 0;
}

// Copy Assigment
Studio &Studio::operator=(const Studio &other) {
    if (this != &other) {
        clear();
        copy(other.open, other.trainers, other.workout_options, other.actionsLog, other.customersIdCounter);
    }
    return *this;
}

// Move Assigment
Studio &Studio::operator=(Studio &&other) {
    if (this != &other) {
        clear();
        open = other.open;
        customersIdCounter = other.customersIdCounter;
        for (size_t i = 0; i < other.workout_options.size(); ++i)
            workout_options.push_back(other.workout_options[i]);
        for (size_t i = 0; i < trainers.size(); ++i)
            trainers[i] = other.trainers[i];
        for (size_t i = 0; i < actionsLog.size(); ++i)
            actionsLog[i] = other.actionsLog[i];
        other.trainers.clear();
        other.actionsLog.clear();
        other.workout_options.clear();
        other.open = false;
        other.customersIdCounter = 0;
    }
    return *this;
}

void Studio::clear() {
    for (size_t i = 0; i < trainers.size(); i++) {
        if (trainers[i] != nullptr) {
            delete trainers[i];
            trainers[i] = nullptr;
        }
    }
    trainers.clear();
    for (size_t i = 0; i < actionsLog.size(); i++) {
        if (actionsLog[i] != nullptr) {
            delete actionsLog[i];
            actionsLog[i] = nullptr;
        }
    }
    actionsLog.clear();
    workout_options.clear();
    open = false;
    customersIdCounter = 0;
}

//Deep copy
void Studio::copy(const bool other_open, const std::vector<Trainer *> &other_trainers,
                  const std::vector<Workout> &other_workout_options, const std::vector<BaseAction *> &other_actionsLog,
                  const int other_customersIdCounter) {
    open = other_open;
    customersIdCounter = other_customersIdCounter;
    for (size_t i = 0; i < other_trainers.size(); ++i) {
        Trainer *trainer = new Trainer(*other_trainers[i]); // Trainer's Copy Constructor
        trainers.push_back(trainer);
    }
    for (size_t i = 0; i < other_workout_options.size(); ++i) {
        Workout tmp = other_workout_options[i];
        Workout workout(tmp.getId(), tmp.getName(), tmp.getPrice(), tmp.getType());
        workout_options.push_back(workout);
    }
    for (size_t i = 0; i < other_actionsLog.size(); ++(i)) {
        BaseAction *action = other_actionsLog[i]->clone();
        actionsLog.push_back(action);
    }
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}

Trainer *Studio::getTrainer(int tid) {
    if (tid >= getNumOfTrainers() || tid < 0)
        return nullptr;
    else
        return trainers[tid];
}

const std::vector<BaseAction *> &Studio::getActionsLog() const {
    return actionsLog;
}

std::vector<Workout> &Studio::getWorkoutOptions() {
    return workout_options;
}

// Once "start" called then the program waits for the user to enter an action to execute
// I'm using in this function in function that split a string into a vector
void Studio::start() {
    open = true;
    customersIdCounter = 0;
    std::cout << "Studio is now open!" << std::endl;
    while (open) {
        std::string input_byUser;
        std::cout << "Please enter a command" << std::endl;
        getline(std::cin, input_byUser);
        if (input_byUser.find("open") == 0) {
            std::vector<Customer *> customers;
            std::vector<std::string> splitOfInput;
            boost::split(splitOfInput, input_byUser, boost::is_any_of(", "), boost::token_compress_on);
            if (splitOfInput.size() > 2) { // In case that someone did "open 2" for example without any customers
                int trainerId = std::stoi(splitOfInput[1]);
                int capacity = getTrainer(trainerId)->getCapacity();// Making sure we don't cross the limited capacity of costumers for each trainer
                //starting from i = 2 because firsT 2 words ubt the vector are "open" and "id"
                for (size_t i = 2; i < splitOfInput.size() && capacity != 0; i += 2) {
                    std::string customerName = splitOfInput[i];
                    std::string type = splitOfInput[i + 1];
                    if (type == "swt") {
                        SweatyCustomer *sweatyCustomer = new SweatyCustomer(customerName, customersIdCounter);
                        customers.push_back(sweatyCustomer);
                    } else if (type == "chp") {
                        CheapCustomer *cheapCustomer = new CheapCustomer(customerName, customersIdCounter);
                        customers.push_back(cheapCustomer);
                    } else if (type == "mcl") {
                        HeavyMuscleCustomer *heavyMuscleCustomer = new HeavyMuscleCustomer(customerName,
                                                                                           customersIdCounter);
                        customers.push_back(heavyMuscleCustomer);
                    } else {
                        FullBodyCustomer *fullBodyCustomer = new FullBodyCustomer(customerName, customersIdCounter);
                        customers.push_back(fullBodyCustomer);
                    }
                    capacity--;
                    customersIdCounter++;
                }
                OpenTrainer *openTrainer = new OpenTrainer(trainerId, customers);
                openTrainer->act(*this);
                actionsLog.push_back(openTrainer);
            }
        } else if (input_byUser.find("order") == 0) {
            std::vector<std::string> splitOfInput;
            boost::split(splitOfInput, input_byUser, boost::is_any_of(" "), boost::token_compress_on);
            int trainerId = std::stoi(splitOfInput[1]);
            Order *order = new Order(trainerId);
            order->act(*this);
            actionsLog.push_back(order);
        } else if (input_byUser.find("move") == 0) {
            std::vector<std::string> splitOfInput;
            boost::split(splitOfInput, input_byUser, boost::is_any_of(" "), boost::token_compress_on);
            int trainerSrc = std::stoi(splitOfInput[1]);
            int trainerDst = std::stoi(splitOfInput[2]);
            int customerId = std::stoi(splitOfInput[3]);
            MoveCustomer *moveCustomer = new MoveCustomer(trainerSrc, trainerDst, customerId);
            moveCustomer->act(*this);
            actionsLog.push_back(moveCustomer);
            if (getTrainer(trainerSrc)->getCustomers().size() ==0) { // If after the move the src trainer has no customers we close him
                Close *close = new Close(trainerSrc);
                close->act(*this);
                actionsLog.push_back(close);
            }
        } else if (input_byUser.find("closeall") == 0) {
            CloseAll *closeAll = new CloseAll();
            closeAll->act(*this);
            open = false;
            delete closeAll;
        } else if (input_byUser.find("close") == 0) {
            std::vector<std::string> splitOfInput;
            boost::split(splitOfInput, input_byUser, boost::is_any_of(" "), boost::token_compress_on);
            int trainerId = std::stoi(splitOfInput[1]);
            Close *close = new Close(trainerId);
            close->act(*this);
            actionsLog.push_back(close);
        } else if (input_byUser.find("workout_options") == 0) {
            PrintWorkoutOptions *printWorkoutOptions = new PrintWorkoutOptions();
            printWorkoutOptions->act(*this);
            actionsLog.push_back(printWorkoutOptions);
        } else if (input_byUser.find("status") == 0) {
            std::vector<std::string> splitOfInput;
            boost::split(splitOfInput, input_byUser, boost::is_any_of(" "), boost::token_compress_on);
            int trainerId = std::stoi(splitOfInput[1]);
            PrintTrainerStatus *printTrainerStatus = new PrintTrainerStatus(trainerId);
            printTrainerStatus->act(*this);
            actionsLog.push_back(printTrainerStatus);
        } else if (input_byUser.find("log") == 0) {
            PrintActionsLog *printActionsLog = new PrintActionsLog();
            printActionsLog->act(*this);
            actionsLog.push_back(printActionsLog);
        } else if (input_byUser.find("backup") == 0) {
            BackupStudio *backupStudio = new BackupStudio();
            backupStudio->act(*this);
            actionsLog.push_back(backupStudio);
        } else if (input_byUser.find("restore") == 0){
            RestoreStudio *restoreStudio = new RestoreStudio();
            restoreStudio->act(*this);
            actionsLog.push_back(restoreStudio);
        }
    }
}
