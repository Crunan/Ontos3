#ifndef GAMEPADCONTROLLER_H
#define GAMEPADCONTROLLER_H

#include <QObject>
#include <SDL2/SDL.h>

class GamepadController : public QObject
{
    Q_OBJECT
public:
    explicit GamepadController(QObject *parent = nullptr);
    ~GamepadController();

    bool Init();
    void PollForEvents();
    SDL_GameController* findController();
    void Close();

    typedef enum {
        CONTROLLER_AXIS_LEFTX = 0,
        CONTROLLER_AXIS_LEFTY = 1,
        CONTROLLER_AXIS_RIGHTX = 3,
        CONTROLLER_AXIS_RIGHTY = 4,
    } GameControllerAxis;


signals:
    void gameControllerMove(int axis, int pct);
    void joystickStateChanged(bool state);

private:
    SDL_GameController *m_pGameController;

    int m_leftJoyXDeflectionPct;
    int m_leftJoyYDeflectionPct;
    int m_rightJoyYDeflectionPct;

    int m_leftJoyXDeflectionPctLast;
    int m_leftJoyYDeflectionPctLast;
    int m_rightJoyYDeflectionPctLast;
};

#endif // GAMEPADCONTROLLER_H
