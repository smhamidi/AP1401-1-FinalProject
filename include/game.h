#ifndef GAME_H
#define GAME_H

#include <QObject>
#include "extern.h"

class Game : public QObject
{
    Q_OBJECT

public:
    Game(QObject* parent = nullptr);
    ~Game();

};

#endif // GAME_H