#pragma once

#include "script.h"
#include "componentmanager.h"
#include "movement_script.h"
#include "transform.h"
#include "controls.h"
#include "sl_time.h"
#include "console.h"
#include <algorithm>
#include "stats.h"

#define MS_HALF_PI 1.5707963268
#define MS_DEG_RAD 0.0174532925

class MovementScript : public Script {
    using Script::Script;
    public:
    float lerpTimer, yrot;
    transform* t;

    void Start() {
        lerpTimer = 0.f;
        yrot = 0.f;
        t = GetComponent<transform>();
        if (!t) Console::warn("no transform");
    }

    void Update() {
        angularRate g_rate = controls::gyroRate();
        lerpTimer -= Time::deltaTime;

        C3D_FVec rot = t->eulerAngles();

        stats::_x = rot.x;
        stats::_y = rot.y;
        stats::_z = rot.z;
        stats::_w = rot.w;

        yrot -= (abs(g_rate.z) > controls::gyroDeadZone() ? g_rate.z : 0) * Time::deltaTime * MS_DEG_RAD * controls::gyroSensitivity();
        // yrot = rot.z;

        float x = (abs(controls::circlePos().dx) > 20 ? controls::circlePos().dx : 0) * Time::deltaTime * 0.005f;
        float y = (abs(controls::circlePos().dy) > 20 ? controls::circlePos().dy : 0) * Time::deltaTime * 0.005f;

        // y = 0;
        // x = 0;

        // if (controls::getHeld(controls::key::KEY_UP)) y = Time::deltaTime;
        // if (controls::getHeld(controls::key::KEY_DOWN)) y = -Time::deltaTime;
        // if (controls::getHeld(controls::key::KEY_LEFT)) x = -Time::deltaTime;
        // if (controls::getHeld(controls::key::KEY_RIGHT)) x = Time::deltaTime;
        
        float s = sinf(-yrot), c = cosf(-yrot);

        t->position.x -= x * c - y * s;
        t->position.z += x * s + y * c;

        t->position.y -= (controls::getHeld(controls::key::KEY_L) ? 0.64 : controls::getHeld(controls::key::KEY_R) ? -0.64 : 0) * Time::deltaTime;

        if (controls::getDown(controls::key::KEY_Y)) lerpTimer = 0.5f; // time in seconds to take when recentering
        
        if (lerpTimer > 0.3) {t->rotation = transform::slerp({1, 0, 0, 0}, t->rotation, lerpTimer); yrot = 0;} // lerp towards player direction instefd
        
        t->rotateY(-(abs(g_rate.z) > controls::gyroDeadZone() ? g_rate.z : 0) * Time::deltaTime * MS_DEG_RAD * controls::gyroSensitivity(), false);
        t->rotateX((abs(g_rate.x) > controls::gyroDeadZone() ? g_rate.x : 0) * Time::deltaTime * MS_DEG_RAD * controls::gyroSensitivity());
    }

    void FixedUpdate() {};
};

COMPONENT_REGISTER(MovementScript)