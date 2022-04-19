/* Authors:
 * Ben Ganon - 318731007
 * Sagiv Antebi - 318159282
 */

#include "CLI.h"

CLI::CLI(DefaultIO *io) {
    this->io = io;
    commands.push_back(new UploadCSV(io));
    commands.push_back(new Settings(io));
    commands.push_back(new Detect(io));
    commands.push_back(new Results(io));
    commands.push_back(new AnalyzeAnomalies(io));
    commands.push_back(new Finish(io));
}

void CLI::start() {
    currentState sharedState;
    int validIndex = -1;
    while (validIndex != 5) {
        io->write("Welcome to the Anomaly Detection Server.\n");
        io->write("Please choose an option:\n");
        for (int i = 0; i < commands.size(); i++) {
            io->write(i + 1);
            io->write(".");
            io->write(commands[i]->description + "\n");
        }
        string choice = io->read();
        // calculate using ascii table
        validIndex = choice[0] - '0' - 1;
        // checking if the input is a valid choice
        if (validIndex > -1 && validIndex < 7)
            commands[validIndex]->execute(&sharedState);
    }
}


CLI::~CLI() {
    for (int i = 0; i < commands.size(); i++) {
        delete commands[i];
    }
}


