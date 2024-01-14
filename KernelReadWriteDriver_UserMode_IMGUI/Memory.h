#pragma once
#include <Windows.h>
#include "Structs.h"

namespace Memory 
{
	namespace Adress {
		ULONG LocalPlayerAdress = 0x17E0A8;
		ULONG EntityList = 0x18AC04;
		ULONG PlayerCount = 0x18AC0C;
	}

	namespace EntityOffsets { /* Alle er offsets til LocalPlayerAddress */
		ULONG HeadX = 0x04; // float
		ULONG HeadY = 0x08; // float
		ULONG HeadZ = 0x0C; // float
		ULONG X = 0x28; // float
		ULONG Y = 0x2C; // float
		ULONG Z = 0x30; // float
		ULONG ViewAngleX = 0x34; // float
		ULONG ViewAngleY = 0x38; // float
		ULONG HP = 0xEC; // int
		ULONG Armor = 0xF0; // int
		ULONG Name = 0x205; // char[16]
		ULONG Team = 0x30C; // int

		ULONG CurrentWeaponStruct = 0xC;

		namespace Weapon { /*Alle er offsets til CurrentWeaponStruct */
			USHORT Name = 0x0; // char[16]
			USHORT Recoil1 = 0x5E; // int16
			USHORT Recoil2 = 0x60; // int16
			USHORT MovementX = 0x5A; // int16
			USHORT MovementY = 0x58; // int16
			USHORT MaxAmmoInMag = 0x56; // int16 
			USHORT WeaponKickBack = 0x54; // int16
			USHORT TimeBetweenShots = 0x48; // int16
			USHORT ReloadTime = 0x46; // int16
			USHORT Damage = 0x4A; // int16
		}

		namespace AssaultRifle {
			ULONG ReserveAmmo = 0x11C; // int
			ULONG CurrentAmmo = 0x140; // int
		}

		namespace Pistol {
			ULONG ReserveAmmo = 0x108; // int
			ULONG CurrentAmmo = 0x12C; // int
		}

		namespace SMG {
			ULONG ReserveAmmo = 0x114; // int
			ULONG CurrentAmmo = 0x138; // int
		}

		namespace CarbineRifle {
			ULONG ReserveAmmo = 0x10C; // int
			ULONG CurrentAmmo = 0x130; // int
		}

		namespace Shotgun {
			ULONG ReserveAmmo = 0x110; // int
			ULONG CurrentAmmo = 0x134; // int
		}

		namespace Sniper {
			ULONG ReserveAmmo = 0x118; // int
			ULONG CurrentAmmo = 0x13C; // int
		}

		namespace Grenade {
			ULONG CurrentAmmo = 0x144; // int
		}
	}
}

/*struct Entity {
	char padding_0000[0x4];    // 00 - 04
	Vec3 headPos;          // 04 - 0C
	char padding_000C[0x24];   // 0C - 34
	Vec2 viewAngle;        // 34 - 3C
	char padding_0038[0xB0];   // 3C - EC
	int health;            // EC - F0
	int armor;             // F0 - F4
	char padding_00F4[0x111];  // F4 - 205
	char name[16];         // 205 - 221
	char padding_0219[0xF7];   // 221 - 30C
	int team;              // 30C - 310
}; <-- Til Internal */