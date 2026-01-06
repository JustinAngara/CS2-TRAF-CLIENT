#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <vector>
#include <cmath>
#include <cstdint>
#include <algorithm>
#include "../utils/Vector.h"
#include "../entity/Classes.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define DEG2RAD(x) ((x) * (float)M_PI / 180.0f)
#define RAD2DEG(x) ((x) * 180.0f / (float)M_PI)

constexpr float CS_GRAVITY = 800.0f;
constexpr float CS_TICK_INTERVAL = 1.0f / 64.0f;


struct QAngle
{
    float pitch, yaw, roll;

    QAngle() : pitch(0), yaw(0), roll(0) {}
    QAngle(float p, float y, float r = 0.f) : pitch(p), yaw(y), roll(r) {}

    QAngle operator+(const QAngle& o) const { return { pitch + o.pitch, yaw + o.yaw, roll + o.roll }; }
    QAngle operator-(const QAngle& o) const { return { pitch - o.pitch, yaw - o.yaw, roll - o.roll }; }
    QAngle operator*(float f) const { return { pitch * f, yaw * f, roll * f }; }

    void Normalize()
    {
        while (yaw > 180.f) yaw -= 360.f;
        while (yaw < -180.f) yaw += 360.f;

        pitch = std::clamp(pitch, -89.f, 89.f);
        roll = 0.f;
    }
};

struct Color
{
    float r, g, b, a;

    Color(int R, int G, int B, int A = 255)
        : r(R / 255.f), g(G / 255.f), b(B / 255.f), a(A / 255.f) {
    }

    static Color Red() { return { 255, 0, 0 }; }
    static Color White() { return { 255, 255, 255 }; }
    static Color Yellow() { return { 255, 255, 0 }; }
};


enum class BoneID : int {
    Head = 6,
    Neck = 5,
    Spine = 4,
    Pelvis = 0,
    LeftShoulder = 8,
    LeftArm = 9,
    LeftHand = 11,
    RightShoulder = 13,
    RightArm = 14,
    RightHand = 16,
    LeftHip = 22,
    LeftKnee = 23,
    LeftFoot = 24,
    RightHip = 25,
    RightKnee = 26,
    RightFoot = 27
};


struct BoneConnection
{
    BoneID bone1;
    BoneID bone2;
};

namespace Bones
{
    inline const std::vector<BoneConnection> connections =
    {
        {BoneID::Head, BoneID::Neck},
        {BoneID::Neck, BoneID::Spine},
        {BoneID::Spine, BoneID::Pelvis},

        {BoneID::Spine, BoneID::LeftShoulder},
        {BoneID::LeftShoulder, BoneID::LeftArm},
        {BoneID::LeftArm, BoneID::LeftHand},

        {BoneID::Spine, BoneID::RightShoulder},
        {BoneID::RightShoulder, BoneID::RightArm},
        {BoneID::RightArm, BoneID::RightHand},

        {BoneID::Pelvis, BoneID::LeftHip},
        {BoneID::LeftHip, BoneID::LeftKnee},
        {BoneID::LeftKnee, BoneID::LeftFoot},

        {BoneID::Pelvis, BoneID::RightHip},
        {BoneID::RightHip, BoneID::RightKnee},
        {BoneID::RightKnee, BoneID::RightFoot}
    };
}


namespace Utils
{
    inline bool IsValidPtr(uintptr_t addr)
    {
        return addr > 0x10000 && addr < 0x7FFFFFFFFFFF;
    }

    inline void NormalizeAngles(Vector& a)
    {
        while (a.y > 180.f) a.y -= 360.f;
        while (a.y < -180.f) a.y += 360.f;

        a.x = std::clamp(a.x, -89.f, 89.f);
        a.z = 0.f;
    }

    inline Vector CalcAngle(const Vector& src, const Vector& dst)
    {
        Vector d = dst - src;
        float hyp = std::sqrt(d.x * d.x + d.y * d.y);

        return {
            -RAD2DEG(std::atan2(d.z, hyp)),
             RAD2DEG(std::atan2(d.y, d.x)),
             0.f
        };
    }

    inline float GetFoV(const Vector& view, const Vector& aim)
    {
        Vector delta = aim - view;
        NormalizeAngles(delta);
        return delta.Length();
    }

    inline void AngleVectors(const Vector& ang, Vector& forward)
    {
        float sp = sinf(DEG2RAD(ang.x));
        float cp = cosf(DEG2RAD(ang.x));
        float sy = sinf(DEG2RAD(ang.y));
        float cy = cosf(DEG2RAD(ang.y));

        forward = { cp * cy, cp * sy, -sp };
    }

    inline bool WorldToScreen(
        const Vector& world,
        Vector& screen,
        const float* matrix,
        float width,
        float height)
    {
        float w = matrix[12] * world.x + matrix[13] * world.y + matrix[14] * world.z + matrix[15];
        if (w < 0.01f)
            return false;

        float x = matrix[0] * world.x + matrix[1] * world.y + matrix[2] * world.z + matrix[3];
        float y = matrix[4] * world.x + matrix[5] * world.y + matrix[6] * world.z + matrix[7];

        float inv = 1.f / w;
        x *= inv;
        y *= inv;

        screen.x = (width * 0.5f) * (1.f + x);
        screen.y = (height * 0.5f) * (1.f - y);
        screen.z = 0.f;

        return true;
    }

    inline Vector GetBonePos(C_CSPlayerPawn* pawn, BoneID boneID)
    {
        if (!pawn)
            return {};

        auto scene = reinterpret_cast<CGameSceneNode*>(pawn->m_pGameSceneNode());
        if (!scene)
            return {};

        uintptr_t boneArray = *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(scene) + Offsets::m_modelState + 0x80);
        if (!IsValidPtr(boneArray))
            return {};

        return *reinterpret_cast<Vector*>(boneArray + static_cast<int>(boneID) * 0x20);
    }

    inline bool SafeReadString(uintptr_t addr, char* out, size_t maxLen = 64)
    {
        if (!addr || !out || maxLen == 0)
            return false;

        __try
        {
            for (size_t i = 0; i < maxLen - 1; i++)
            {
                char c;
                memcpy(&c, reinterpret_cast<void*>(addr + i), 1);

                if (c == '\0')
                {
                    out[i] = '\0';
                    return true;
                }

                out[i] = c;
            }

            out[maxLen - 1] = '\0';
            return true;
        }
        __except (EXCEPTION_EXECUTE_HANDLER)
        {
            return false;
        }
    }
}

