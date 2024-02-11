#include "gamepadcontroller.h"
#include <QMessageBox>
#include "logger.h"
#include <math.h>

const int JOYSTICK_DEAD_ZONE = 6000;
const float MAX_XYDEFLECTION_VALUE = 32767.0;
const float MAX_ZDEFLECTION_VALUE = 65534.0;
const int JOYSTICK_MOVE_TOLERANCE = 5;

GamepadController::GamepadController(QObject *parent)
    : QObject{parent},
    m_leftJoyXDeflectionPct(0),
    m_leftJoyYDeflectionPct(0),
    m_rightJoyYDeflectionPct(0),
    m_leftJoyXDeflectionPctLast(0),
    m_leftJoyYDeflectionPctLast(0),
    m_rightJoyYDeflectionPctLast(0),
    m_pGameController(nullptr)
{}

GamepadController::~GamepadController()
{
    SDL_Quit();
}

bool GamepadController::Init()
{
    int retVal = SDL_Init(SDL_INIT_GAMECONTROLLER);

    if (retVal == 0) {
        m_pGameController = findController();
        if (!m_pGameController) {
            Logger::logInfo("Error: No gamepad controller found");
            return false;
        }
    }
    else {
        Logger::logInfo("Error: Gamepad initialization failed");
        return false;
    }

    return true;
}

SDL_GameController* GamepadController::findController()
{
    if (SDL_NumJoysticks() > 0) {
        if (SDL_IsGameController(0)) {
            return SDL_GameControllerOpen(0);
        }
    }

    return nullptr;
}

void GamepadController::Close()
{
    SDL_GameControllerClose(m_pGameController);
    SDL_Quit();
    m_pGameController = nullptr;
}

void GamepadController::PollForEvents()
{
    // event handler
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0) {

        if (event.type == SDL_JOYAXISMOTION) {

            if (event.caxis.axis == CONTROLLER_AXIS_LEFTX) {
                if (abs(event.caxis.value) > JOYSTICK_DEAD_ZONE) { // joystick deflection outside deadzone

                    m_leftJoyXDeflectionPct = (event.jaxis.value / MAX_XYDEFLECTION_VALUE) * 100;
                }
                else {
                    m_leftJoyXDeflectionPct = 0;
                }

                if (m_leftJoyXDeflectionPct != m_leftJoyXDeflectionPctLast) {
                    if (m_leftJoyXDeflectionPct == 0 ||
                        m_leftJoyXDeflectionPct == 100 ||
                        abs(m_leftJoyXDeflectionPct - m_leftJoyXDeflectionPctLast) >= JOYSTICK_MOVE_TOLERANCE) {
                            m_leftJoyXDeflectionPctLast = m_leftJoyXDeflectionPct;
                            // emit signal to initiate move
                            gameControllerMove(CONTROLLER_AXIS_LEFTX, m_leftJoyXDeflectionPct);
                    }
                }
            }
            else if (event.caxis.axis == CONTROLLER_AXIS_LEFTY) {
                if (abs(event.caxis.value) > JOYSTICK_DEAD_ZONE) { // joystick deflection outside deadzone

                    m_leftJoyYDeflectionPct = (event.jaxis.value / MAX_XYDEFLECTION_VALUE) * 100;
                }
                else {
                    m_leftJoyYDeflectionPct = 0;
                }

                if (m_leftJoyYDeflectionPct != m_leftJoyYDeflectionPctLast) {
                    if (m_leftJoyYDeflectionPct == 0 ||
                        m_leftJoyYDeflectionPct == 100 ||
                        abs(m_leftJoyYDeflectionPct - m_leftJoyYDeflectionPctLast) >= JOYSTICK_MOVE_TOLERANCE) {
                            m_leftJoyYDeflectionPctLast = m_leftJoyYDeflectionPct;
                            // emit signal to initiate move
                            gameControllerMove(CONTROLLER_AXIS_LEFTY, m_leftJoyYDeflectionPct);
                    }
                }
            }
            else if (event.caxis.axis == CONTROLLER_AXIS_RIGHTY) {
                if (abs(event.caxis.value) > JOYSTICK_DEAD_ZONE) { // joystick deflection outside deadzone

                    m_rightJoyYDeflectionPct = (event.jaxis.value / MAX_ZDEFLECTION_VALUE) * 100;
                }
                else {
                    m_rightJoyYDeflectionPct = 0;
                }

                if (m_rightJoyYDeflectionPct != m_rightJoyYDeflectionPctLast) {
                    if (m_rightJoyYDeflectionPct == 0 ||
                        m_rightJoyYDeflectionPct == 100 ||
                        abs(m_rightJoyYDeflectionPct - m_rightJoyYDeflectionPctLast) >= JOYSTICK_MOVE_TOLERANCE) {
                            m_rightJoyYDeflectionPctLast = m_rightJoyYDeflectionPct;
                            // emit signal to initiate move
                            gameControllerMove(CONTROLLER_AXIS_RIGHTY, m_rightJoyYDeflectionPct);
                    }
                }
            }
        }
        else if(event.type == SDL_CONTROLLERDEVICEADDED) {
            Logger::logInfo("Game Controller Connected");

            m_pGameController = SDL_GameControllerOpen(event.cdevice.which);
        }
        else if(event.type == SDL_CONTROLLERDEVICEREMOVED) {
            Logger::logInfo("Game Controller Disconnected");

            emit joystickStateChanged(false);

            m_pGameController = SDL_GameControllerOpen(event.cdevice.which);

            if (m_pGameController && event.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(m_pGameController))) {
                SDL_GameControllerClose(m_pGameController);
                m_pGameController = findController();
            }
        }
    }
}


