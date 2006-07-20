/*
 * Copyright (c) 2003-2004 Lukasz Wlasinski
 *
 *    This file is part of TS XMod.
 *
 *    TS XMod is free software; you can redistribute it and/or modify it
 *    under the terms of the GNU General Public License as published by the
 *    Free Software Foundation; either version 2 of the License, or (at
 *    your option) any later version.
 *
 *    TS XMod is distributed in the hope that it will be useful, but
 *    WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with TS XMod; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *    In addition, as a special exception, the author gives permission to
 *    link the code of this program with the Half-Life Game Engine ("HL
 *    Engine") and Modified Game Libraries ("MODs") developed by Valve,
 *    L.L.C ("Valve").  You must obey the GNU General Public License in all
 *    respects for all of the code used other than the HL Engine and MODs
 *    from Valve.  If you modify this file, you may extend this exception
 *    to your version of the file, but you are not obligated to do so.  If
 *    you do not wish to do so, delete this exception statement from your
 *    version.
 *
 */

#include "amxxmodule.h"

#ifndef TSX_NATIVES
#define TSX_NATIVES

#define TSPWUP_SLOWMO 			1
#define TSPWUP_INFAMMO		 	2
#define TSPWUP_KUNGFU		 	4
#define TSPWUP_SLOWPAUSE 		8
#define TSPWUP_DFIRERATE		16
#define TSPWUP_GRENADE			32
#define TSPWUP_HEALTH			64
#define TSPWUP_ARMOR			128
#define TSPWUP_SUPERJUMP		256

static cell AMX_NATIVE_CALL get_weapon_name(AMX *amx, cell *params)
{ // from id to name 3 params id, name, len

	int id = params[1];
	if (id<0 || id>=TSMAX_WEAPONS)
	{ 
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", id);
		return 0;
	}
	return MF_SetAmxString(amx,params[2],weaponData[id].name,params[3]);
}

static cell AMX_NATIVE_CALL wpnlog_to_name(AMX *amx, cell *params)
{ // from log to name

	int iLen;
	char *log = MF_GetAmxString(amx,params[1],0,&iLen);
	int i;
	for ( i=1; i<TSMAX_WEAPONS; i++ )
	{
		if ( strcmp(log,weaponData[i].logname ) == 0 )
			return MF_SetAmxString(amx,params[2],weaponData[i].name,params[3]);

	}
	return 0;
}

static cell AMX_NATIVE_CALL wpnlog_to_id(AMX *amx, cell *params)
{ // from log to id
	int iLen;
	char *log = MF_GetAmxString(amx,params[1],0,&iLen);

	int i;
	for ( i=1; i<TSMAX_WEAPONS; i++ )
	{
		if ( strcmp(log,weaponData[i].logname) == 0 )
			return i;
	}
	return 0;
}

static cell AMX_NATIVE_CALL get_weapon_logname(AMX *amx, cell *params)
{ // from id to log
	int id = params[1];
	if (id<0 || id>=TSMAX_WEAPONS)
	{ 
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", id);
		return 0;
	}
	return MF_SetAmxString(amx,params[2],weaponData[id].logname,params[3]);
}

static cell AMX_NATIVE_CALL is_melee(AMX *amx, cell *params)
{
	int id = params[1];
	if (id<1 || id>=TSMAX_WEAPONS)
	{ 
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid weapon id %d", id);
		return 0;
	}

	if ( weaponData[id].melee )
		return 1;

	return 0;
}

static cell AMX_NATIVE_CALL ts_get_user_weapon(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	int wpn = pPlayer->current;

	cell *cpTemp = MF_GetAmxAddr(amx,params[2]);
	*cpTemp = pPlayer->weapons[wpn].clip;

	cpTemp = MF_GetAmxAddr(amx,params[3]);
	*cpTemp = pPlayer->weapons[wpn].ammo;

	cpTemp = MF_GetAmxAddr(amx,params[4]);
	*cpTemp = pPlayer->weapons[wpn].mode;

	cpTemp = MF_GetAmxAddr(amx,params[5]);
	*cpTemp = pPlayer->weapons[wpn].attach;

	return wpn;

}

static cell AMX_NATIVE_CALL get_user_weapon(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	int wpn = pPlayer->current;

	cell *cpTemp = MF_GetAmxAddr(amx,params[2]);
	*cpTemp = pPlayer->weapons[wpn].clip;

	cpTemp = MF_GetAmxAddr(amx,params[3]);
	*cpTemp = pPlayer->weapons[wpn].ammo;

	return wpn;

}

static cell AMX_NATIVE_CALL get_user_state(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	if(!pPlayer->ingame) return 0;

	return pPlayer->state;

}


static cell AMX_NATIVE_CALL set_user_cash(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	pPlayer->SetMoney(params[2]);
	return 1;

}

static cell AMX_NATIVE_CALL get_user_cash(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	return pPlayer->GetMoney();

}

static cell AMX_NATIVE_CALL set_user_slots(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	pPlayer->SetSlots(params[2]);
	return 1;
}

static cell AMX_NATIVE_CALL get_user_slots(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	return pPlayer->GetSlots();
}


static cell AMX_NATIVE_CALL get_user_pwup(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	cell *cpTemp = MF_GetAmxAddr(amx,params[2]);
	*cpTemp = pPlayer->PwUpValue;

	return pPlayer->PwUp;

}

static cell AMX_NATIVE_CALL get_user_items(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	return pPlayer->items;

}

static cell AMX_NATIVE_CALL get_killingStreak(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	return pPlayer->killingSpree;

}

static cell AMX_NATIVE_CALL get_lastFrag(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	return pPlayer->lastFrag;

}

static cell AMX_NATIVE_CALL get_killflags(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	return pPlayer->killFlags;

}

static cell AMX_NATIVE_CALL give_weapon(AMX *amx, cell *params)
{
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	if ( !pPlayer->IsAlive() )
	{
		return 0;
	}

	// can he carry this weapon check ?

	string_t item = MAKE_STRING("ts_groundweapon"); 
	edict_t	*pent = CREATE_NAMED_ENTITY( item );
	if ( FNullEnt( pent ) )
	{
		return 0;
	}

	KeyValueData pkvd;
	char szTemp[16];

	sprintf(szTemp,"%d",(int)params[2]);

	pkvd.szClassName = (char *)STRING(pent->v.classname);
	pkvd.szKeyName = "tsweaponid"; // weapon
	pkvd.szValue = szTemp;
	pkvd.fHandled = false;
	MDLL_KeyValue(pent, &pkvd);

	pkvd.szClassName = (char *)STRING(pent->v.classname);
	pkvd.szKeyName = "wduration"; // duration
	pkvd.szValue = "180";
	pkvd.fHandled = false;
	MDLL_KeyValue(pent, &pkvd);

	sprintf(szTemp,"%d",(int)params[3]); 

	pkvd.szClassName = (char *)STRING(pent->v.classname);
	pkvd.szKeyName = "wextraclip"; // clips
	pkvd.szValue = szTemp;
	pkvd.fHandled = false;
	MDLL_KeyValue(pent, &pkvd);

	sprintf(szTemp,"%d",(int)params[4]);

	pkvd.szClassName = (char *)STRING(pent->v.classname);
	pkvd.szKeyName = "spawnflags"; // attachements :flashlight,lasersight,scope..
	pkvd.szValue = szTemp;
	pkvd.fHandled = false;
	MDLL_KeyValue(pent, &pkvd);

/*
	pkvd.szClassName = "ts_groundweapon";
	pkvd.szKeyName = "message";
	pkvd.szValue = "";
	pMDLL_KeyValue(pEntity, &pkvd);
*/

	MDLL_Spawn(pent);

	pent->v.origin = pPlayer->pEdict->v.origin;

	MDLL_Use(pent, pPlayer->pEdict);

	REMOVE_ENTITY(pent);

	return 1;

}

static cell AMX_NATIVE_CALL create_pwup(AMX *amx, cell *params)
{ 

	string_t item = MAKE_STRING("ts_powerup"); 
	edict_t	*pent = CREATE_NAMED_ENTITY( item );
	if ( FNullEnt( pent ) )
	{
		return 0;
	}

	KeyValueData pkvd;
	char szTemp[16];

	sprintf(szTemp,"%d",(int)params[1]);

	pkvd.szClassName = (char *)STRING(pent->v.classname);
	pkvd.szKeyName = "pwuptype"; // type
	pkvd.szValue = szTemp;
	pkvd.fHandled = false;
	MDLL_KeyValue(pent, &pkvd);

	pkvd.szClassName = (char *)STRING(pent->v.classname);
	pkvd.szKeyName = "pwupduration"; // duration
	pkvd.szValue = "60";
	pkvd.fHandled = false;
	MDLL_KeyValue(pent, &pkvd);

/*
	pkvd.szClassName = (char *)STRING(pent->v.classname);
	pkvd.szKeyName = "message";
	pkvd.szValue = "";
	pMDLL_KeyValue(pEntity, &pkvd);
*/
	cell *vInput = MF_GetAmxAddr(amx,params[2]);

	float fNewX = *(float *)((void *)&vInput[0]);
	float fNewY = *(float *)((void *)&vInput[1]);
	float fNewZ = *(float *)((void *)&vInput[2]);

	vec3_t vNewValue = vec3_t(fNewX, fNewY, fNewZ);

	MDLL_Spawn(pent);
	pent->v.origin = vNewValue;

	return ENTINDEX(pent);
}

// create_pwup -> !wait! -> give_pwup
static cell AMX_NATIVE_CALL give_pwup(AMX *amx, cell *params)
{

	edict_t* pent = INDEXENT(params[2]);
	if ( FNullEnt( pent ) || strcmp("ts_powerup",STRING(pent->v.classname))!=0 )
	{
		return 0;
	}
	
	int id = params[1];
	CHECK_PLAYER(id);
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	if ( !pPlayer->IsAlive() )
	{
		REMOVE_ENTITY(pent);
		return 0;
	}

	pent->v.origin = pPlayer->pEdict->v.origin;

	MDLL_Touch(pent, pPlayer->pEdict);

	REMOVE_ENTITY(pent);

	return 1;

}

static cell AMX_NATIVE_CALL get_maxweapons(AMX *amx, cell *params)
{
	return TSMAX_WEAPONS;
}

static cell AMX_NATIVE_CALL is_custom(AMX *amx, cell *params)
{
	int weapon = params[1];
	if (  weapon < TSMAX_WEAPONS-TSMAX_CUSTOMWPNS )
	{
		return 0;
	}
	return 1;
}

// set_fuattack(Float:time,Float:damage)
static cell AMX_NATIVE_CALL set_fuattack(AMX *amx, cell *params)
{
	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	pPlayer->SetOffsetFloat(454,amx_ctof(params[2]) );
	pPlayer->SetOffsetFloat(455,amx_ctof(params[3]) );

	return 1;
}

static cell AMX_NATIVE_CALL has_superjump(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	int val3 = pPlayer->GetOffsetInt(433);

	if(val3 & 0x01000000) return 1;

	return 0;
}

static cell AMX_NATIVE_CALL has_fupowerup(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	int val3 = pPlayer->GetOffsetInt(433);

	if(val3 & 65536) return 1;

	return 0;
}

static cell AMX_NATIVE_CALL get_usertime(AMX *amx, cell *params)
{
	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	return pPlayer->GetCurrentTime();

}

static cell AMX_NATIVE_CALL set_usertime(AMX *amx, cell *params)
{
	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	pPlayer->SetCurrentTime(amx_ctof(params[2]));
	return 1;
}

static cell AMX_NATIVE_CALL get_consecutive_frags(AMX *amx, cell *params)
{
	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	return pPlayer->GetOffsetInt(433);
}

static cell AMX_NATIVE_CALL set_consecutive_frags(AMX *amx, cell *params)
{
	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	pPlayer->SetOffsetInt(433,params[2]);
	return 1;
}

static cell AMX_NATIVE_CALL set_user_message(AMX *amx, cell *params)
{
	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	int message = params[2];
	if(message < 1 || message > 16) 
	{
		MF_LogError(amx, AMX_ERR_NATIVE, "Invalid Message ID: %i",message );
		return 0;
	}

	int val3 = pPlayer->GetOffsetInt(433);

	pPlayer->SetOffsetInt(433, (val3 & ~15)^message);
	return 1;
}

static cell AMX_NATIVE_CALL get_user_message(AMX *amx, cell *params)
{
	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	int val3 = pPlayer->GetOffsetInt(433);

	return (val3 & 15);
}

static cell AMX_NATIVE_CALL set_bullettrail(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	long bullettrail = params[2] * 256;
	
	pPlayer->SetOffsetInt(87,bullettrail);
	return 1;
}

static cell AMX_NATIVE_CALL set_fake_slowmo(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	float time = amx_ctof(params[2]);

	pPlayer->SetOffsetInt(423,TSPWUP_SLOWMO);

	float prev = pPlayer->GetCurrentTime();

	pPlayer->SetOffsetFloat(425,prev+time);
		
	return 1;
}

static cell AMX_NATIVE_CALL set_fake_slowpause(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	float time = amx_ctof(params[2]);

	pPlayer->SetOffsetInt(423,TSPWUP_SLOWPAUSE);

	float prev = pPlayer->GetCurrentTime();

	pPlayer->SetOffsetFloat(425,prev+time);

	return 1;
}

static cell AMX_NATIVE_CALL is_in_slowmo(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	if(pPlayer->GetOffsetFloat(89) )
		return pPlayer->GetOffsetFloat(425);
	
	return 0;
}

static cell AMX_NATIVE_CALL set_speed(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	pPlayer->SetOffsetFloat(89, amx_ctof(params[2]) );
	pPlayer->SetOffsetFloat(90, amx_ctof(params[3]) );
	pPlayer->SetOffsetFloat(85, amx_ctof(params[3]) );
	pPlayer->SetOffsetFloat(425,amx_ctof(params[4]) );
	
	return 1;
}

static cell AMX_NATIVE_CALL set_physics_speed(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	pPlayer->SetOffsetFloat(86,amx_ctof(params[2]));
	
	return 1;
}

static cell AMX_NATIVE_CALL is_running_powerup(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	return pPlayer->GetOffsetInt(423);
}

static cell AMX_NATIVE_CALL force_powerup_run(AMX *amx, cell *params)
{

	int id = params[1];
	CPlayer *pPlayer = GET_PLAYER_POINTER_I(id);

	CHECK_PLAYER(id);

	pPlayer->SetOffsetInt(423,params[2]);
	return 1;
}


AMX_NATIVE_INFO base_Natives[] = {

	{ "xmod_get_wpnname", get_weapon_name },
	{ "xmod_get_wpnlogname", get_weapon_logname },
	{ "xmod_is_melee_wpn", is_melee },
	{ "xmod_get_maxweapons", get_maxweapons },
	{ "xmod_is_custom_wpn", is_custom },

	//****************************************

	{ "ts_giveweapon",give_weapon },
	{ "ts_createpwup",create_pwup },
	{ "ts_givepwup",give_pwup },

	{ "get_weaponname", get_weapon_name },
	{ "get_user_weapon", get_user_weapon },
	
	{ "ts_wpnlogtoname", wpnlog_to_name },
	{ "ts_wpnlogtoid", wpnlog_to_id },
	{ "ts_getuserwpn", ts_get_user_weapon },

	//****************************************

	{ "ts_getuserstate", get_user_state },

	{ "ts_getusercash", get_user_cash },
	{ "ts_setusercash", set_user_cash },

	{ "ts_getuserslots", get_user_slots },
	{ "ts_setuserslots", set_user_slots },

	{ "ts_getusertime",get_usertime },
	{ "ts_setusertime",set_usertime},

	{ "ts_getuserspace", get_user_slots },
	{ "ts_getuserpwup",get_user_pwup },
	{ "ts_getuseritems",get_user_items },

	{ "ts_getkillingstreak",get_killingStreak },
	{ "ts_getuserlastfrag",get_lastFrag },
	{ "ts_getuserkillflags",get_killflags },

	//****************************************
	
	{ "ts_set_fuattack", set_fuattack },
	{ "ts_set_message", set_user_message },
	{ "ts_get_message", get_user_message },

	{ "ts_has_superjump", has_superjump },
	{ "ts_has_fupowerup", has_fupowerup },

	{ "ts_get_cons_frags", get_consecutive_frags },
	{ "ts_set_cons_frags", set_consecutive_frags },

	//****************************************

	{ "ts_set_bullettrail",set_bullettrail },
	{ "ts_set_fakeslowmo",set_fake_slowmo },
	{ "ts_set_fakeslowpause",set_fake_slowpause },
	{ "ts_is_in_slowmo",is_in_slowmo },
	{ "ts_set_speed",set_speed },
	{ "ts_set_physics_speed",set_physics_speed },

	//****************************************

	{ "ts_is_running_powerup",is_running_powerup},
	{ "ts_force_run_powerup",force_powerup_run},

	//"*******************"
	{ NULL, NULL } 
};

#endif