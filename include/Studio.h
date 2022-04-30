#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"


class Studio {
public:

    // Constructor
    Studio(const std::string &configFilePath);

    // Destructor
    virtual ~Studio();

    // Copy Constructor
    Studio(const Studio &studio);

    // Move Constructor
    Studio(Studio &&other);

    // Copy Assigment
    Studio &operator=(const Studio &other);

    // Move Assigment
    Studio &operator=(Studio &&other);

    void start();

    int getNumOfTrainers() const;

    Trainer *getTrainer(int tid);

    const std::vector<BaseAction *> &getActionsLog() const; // Return a reference to the history of actions

    std::vector<Workout> &getWorkoutOptions(); // Return a reference to all workout options in the studio

private:
    bool open;
    std::vector<Trainer *> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction *> actionsLog;
    int customersIdCounter; // Field for remember last id that was given

    void clear();
    void copy(const bool other_open, const std::vector<Trainer *> &other_trainers,
              const std::vector<Workout> &other_workout_options,
              const std::vector<BaseAction *> &other_actionsLog, const int other_customersIdCounter); // For using in copy/move constructor
};

#endif