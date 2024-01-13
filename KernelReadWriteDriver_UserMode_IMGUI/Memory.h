#pragma once
#include <Windows.h>

namespace Memory 
{
	namespace Adress {
		ULONG BaseModuleAdress;
		ULONG ProcessId;
		ULONG LocalPlayerAdress = 0x17E0A8;
		ULONG EntityList = 0x18AC04;
	}

	namespace LocalPlayerOffsets { /* Alle er offsets til LocalPlayerAddress */
		ULONG X = 0x28;
		ULONG Y = 0x2C;
		ULONG Z = 0x30;
		ULONG HP = 0xEC;
		ULONG Armor = 0xF0;
		ULONG Name = 0x205;

		ULONG CurrentWeaponStruct = 0xC;

		namespace Weapon { /*Alle er offsets til CurrentWeaponStruct */
			USHORT Name = 0x0;
			USHORT Recoil1 = 0x5E;
			USHORT Recoil2 = 0x60;
			USHORT MovementX = 0x5A;
			USHORT MovementY = 0x58;
			USHORT MaxAmmoInMag = 0x56;
			USHORT WeaponKickBack = 0x54;
			USHORT TimeBetweenShots = 0x48;
			USHORT ReloadTime = 0x46;
			USHORT Damage = 0x4A;
		}

		namespace AssaultRifle {
			ULONG ReserveAmmo = 0x11C;
			ULONG CurrentAmmo = 0x140;
		}

		namespace Pistol {
			ULONG ReserveAmmo = 0x108;
			ULONG CurrentAmmo = 0x12C;
		}

		namespace SMG {
			ULONG ReserveAmmo = 0x114;
			ULONG CurrentAmmo = 0x138;
		}

		namespace CarbineRifle {
			ULONG ReserveAmmo = 0x10C;
			ULONG CurrentAmmo = 0x130;
		}

		namespace Shotgun {
			ULONG ReserveAmmo = 0x110;
			ULONG CurrentAmmo = 0x134;
		}

		namespace Sniper {
			ULONG ReserveAmmo = 0x118;
			ULONG CurrentAmmo = 0x13C;
		}

		namespace Grenade {
			ULONG CurrentAmmo = 0x144;
		}

	}
}