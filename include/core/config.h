#pragma once

// Core layer: gameplay and physics constants.

namespace Config {
constexpr float pixelsPerMeter = 50.0f;
constexpr float fixedTimeStep = 1.0f / 60.0f;
constexpr int velocityIterations = 8;
constexpr int positionIterations = 3;
constexpr float sleepVelocityThreshold = 0.08f;
constexpr float sleepAngularThreshold = 0.08f;
constexpr float sleepConfirmSeconds = 0.5f;
constexpr float maxSimulationSeconds = 5.0f;
constexpr float maxShotPower = 38.0f;
constexpr float shotImpulseScale = 0.5f;
constexpr float shotPowerScale = 9.0f;
constexpr float minDragPixels = 8.0f;
constexpr float windowWidth = 1000.0f;
constexpr float windowHeight = 620.0f;
constexpr float fieldMarginPixels = 70.0f;
constexpr float levelCardX = 46.0f;
constexpr float levelCardY = 175.0f;
constexpr float levelCardWidth = 172.0f;
constexpr float levelCardHeight = 270.0f;
constexpr float levelCardGap = 12.0f;
constexpr float modeButtonX = 46.0f;
constexpr float modeButtonY = 112.0f;
constexpr float modeButtonWidth = 190.0f;
constexpr float modeButtonHeight = 42.0f;
constexpr float modeButtonGap = 12.0f;
constexpr float demoFieldWidth = 16.0f;
constexpr float demoFieldHeight = 9.0f;
constexpr float demoPuckRadius = 0.42f;
constexpr float demoBallRadius = 0.24f;
constexpr float demoGoalWidth = 3.0f;
constexpr float demoGoalDepth = 0.6f;
constexpr int demoMaxScore = 2;
constexpr float demoPuckImpulse = 8.5f;
constexpr float demoPuckStartX = 4.0f;
constexpr float demoBallStartX = 9.0f;
constexpr float demoPuckDensity = 1.0f;
constexpr float demoBallDensity = 0.45f;
constexpr float demoLinearDamping = 0.35f;
constexpr float demoFriction = 0.28f;
constexpr float demoRestitution = 0.62f;
}
