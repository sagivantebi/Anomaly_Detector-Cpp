/* Authors:
 * Ben Ganon - 318731007
 * Sagiv Antebi - 318159282
 */

#ifndef CLI_H_
#define CLI_H_

#include <vector>
#include <algorithm>
#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
    vector<Command*> commands;
    DefaultIO* io;
public:
    CLI(DefaultIO* io);
    void start();
    virtual ~CLI();
};

#endif /* CLI_H_ */

