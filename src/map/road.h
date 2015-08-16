/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file map/road.h Map tile accessors for road tiles. */

#ifndef MAP_ROAD_H
#define MAP_ROAD_H

#include "../stdafx.h"
#include "../tile/road.h"
#include "map.h"
#include "coord.h"
#include "../direction_type.h"
#include "../road_type.h"
#include "../company_type.h"

/**
 * Get the present road bits for a specific road type.
 * @param t  The tile to query.
 * @param rt Road type.
 * @pre IsRoadTile(t)
 * @return The present road bits for the road type.
 */
static inline RoadBits GetRoadBits(TileIndex t, RoadType rt)
{
	return tile_get_roadbits(&_mc[t], rt);
}

/**
 * Get all RoadBits set on a tile except from the given RoadType
 *
 * @param t The tile from which we want to get the RoadBits
 * @param rt The RoadType which we exclude from the querry
 * @return all set RoadBits of the tile which are not from the given RoadType
 */
static inline RoadBits GetOtherRoadBits(TileIndex t, RoadType rt)
{
	return GetRoadBits(t, rt == ROADTYPE_ROAD ? ROADTYPE_TRAM : ROADTYPE_ROAD);
}

/**
 * Get all set RoadBits on the given tile
 *
 * @param tile The tile from which we want to get the RoadBits
 * @return all set RoadBits of the tile
 */
static inline RoadBits GetAllRoadBits(TileIndex tile)
{
	return tile_get_all_roadbits(&_mc[tile]);
}

/**
 * Set the present road bits for a specific road type.
 * @param t  The tile to change.
 * @param r  The new road bits.
 * @param rt Road type.
 * @pre IsRoadTile(t)
 */
static inline void SetRoadBits(TileIndex t, RoadBits r, RoadType rt)
{
	tile_set_roadbits(&_mc[t], rt, r);
}


RoadBits GetAnyRoadBits(TileIndex tile, RoadType rt, bool tunnel_bridge_entrance = false);


/**
 * Get the present road types of a tile.
 * @param t The tile to query.
 * @return Present road types.
 */
static inline RoadTypes GetRoadTypes(TileIndex t)
{
	return tile_get_roadtypes(&_mc[t]);
}

/**
 * Set the present road types of a tile.
 * @param t  The tile to change.
 * @param rt The new road types.
 */
static inline void SetRoadTypes(TileIndex t, RoadTypes rt)
{
	tile_set_roadtypes(&_mc[t], rt);
}

/**
 * Check if a tile has a specific road type.
 * @param t  The tile to check.
 * @param rt Road type to check.
 * @return True if the tile has the specified road type.
 */
static inline bool HasTileRoadType(TileIndex t, RoadType rt)
{
	return tile_has_roadtype(&_mc[t], rt);
}


/**
 * Get the owner of a specific road type.
 * @param t  The tile to query.
 * @param rt The road type to get the owner of.
 * @return Owner of the given road type.
 */
static inline Owner GetRoadOwner(TileIndex t, RoadType rt)
{
	return tile_get_road_owner(&_mc[t], rt);
}

/**
 * Set the owner of a specific road type.
 * @param t  The tile to change.
 * @param rt The road type to change the owner of.
 * @param o  New owner of the given road type.
 */
static inline void SetRoadOwner(TileIndex t, RoadType rt, Owner o)
{
	tile_set_road_owner(&_mc[t], rt, o);
}

/**
 * Check if a specific road type is owned by an owner.
 * @param t  The tile to query.
 * @param rt The road type to compare the owner of.
 * @param o  Owner to compare with.
 * @pre HasTileRoadType(t, rt)
 * @return True if the road type is owned by the given owner.
 */
static inline bool IsRoadOwner(TileIndex t, RoadType rt, Owner o)
{
	assert(HasTileRoadType(t, rt));
	return (GetRoadOwner(t, rt) == o);
}

/**
 * Checks if given tile has town owned road
 * @param t tile to check
 * @pre IsRoadTile(t)
 * @return true iff tile has road and the road is owned by a town
 */
static inline bool HasTownOwnedRoad(TileIndex t)
{
	return HasTileRoadType(t, ROADTYPE_ROAD) && IsRoadOwner(t, ROADTYPE_ROAD, OWNER_TOWN);
}


/**
 * Get the decorations of a road.
 * @param tile The tile to query.
 * @return The road decoration of the tile.
 */
static inline Roadside GetRoadside(TileIndex tile)
{
	return tile_get_roadside(&_mc[tile]);
}

/**
 * Set the decorations of a road.
 * @param tile The tile to change.
 * @param s    The new road decoration of the tile.
 */
static inline void SetRoadside(TileIndex tile, Roadside s)
{
	tile_set_roadside(&_mc[tile], s);
}

/**
 * Check if a tile has road works.
 * @param t The tile to check.
 * @return True if the tile has road works in progress.
 */
static inline bool HasRoadWorks(TileIndex t)
{
	return tile_has_roadworks(&_mc[t]);
}

/**
 * Start road works on a tile.
 * @param t The tile to start the work on.
 * @pre IsNormalRoadTile(t)
 */
static inline void StartRoadWorks(TileIndex t)
{
	tile_init_roadworks(&_mc[t]);
}

/**
 * Decrease the road works counter.
 * @param t The tile to modify.
 * @return True if the road works are in the last stage.
 */
static inline bool DecreaseRoadWorksCounter(TileIndex t)
{
	return tile_dec_roadworks(&_mc[t]);
}


/**
 * Gets the disallowed directions
 * @param t the tile to get the directions from
 * @return the disallowed directions
 */
static inline DisallowedRoadDirections GetDisallowedRoadDirections(TileIndex t)
{
	return tile_get_disallowed_directions(&_mc[t]);
}

/**
 * Sets the disallowed directions
 * @param t   the tile to set the directions for
 * @param drd the disallowed directions
 */
static inline void SetDisallowedRoadDirections(TileIndex t, DisallowedRoadDirections drd)
{
	tile_set_disallowed_directions(&_mc[t], drd);
}


/**
 * Determines the type of road bridge on a tile
 * @param t The tile to analyze
 * @pre IsRoadBridgeTile(t)
 * @return The bridge type
 */
static inline uint GetRoadBridgeType(TileIndex t)
{
	return tile_get_road_bridge_type(&_mc[t]);
}

/**
 * Set the type of road bridge on a tile
 * @param t The tile to set
 * @param type The type to set
 */
static inline void SetRoadBridgeType(TileIndex t, uint type)
{
	tile_set_road_bridge_type(&_mc[t], type);
}

/**
 * Check if a road bridge is an extended bridge head
 * @param t The tile to check
 * @return Whether there are road bits set not in the axis of the bridge
 */
static inline bool IsExtendedRoadBridge(TileIndex t)
{
	return tile_is_road_custom_bridgehead(&_mc[t]);
}


/**
 * Make a normal road tile.
 * @param t    Tile to make a normal road.
 * @param bits Road bits to set for all present road types.
 * @param rot  New present road types.
 * @param town Town ID if the road is a town-owned road.
 * @param road New owner of road.
 * @param tram New owner of tram tracks.
 */
static inline void MakeRoadNormal(TileIndex t, RoadBits bits, RoadTypes rot, TownID town, Owner road, Owner tram)
{
	tile_make_road(&_mc[t], rot, bits, town, road, tram);
}

/**
 * Make a bridge ramp for roads.
 * @param t          the tile to make a bridge ramp
 * @param owner_road the new owner of the road on the bridge
 * @param owner_tram the new owner of the tram on the bridge
 * @param bridgetype the type of bridge this bridge ramp belongs to
 * @param d          the direction this ramp must be facing
 * @param r          the road type of the bridge
 * @param town       owner/closest town ID
 */
static inline void MakeRoadBridgeRamp(TileIndex t, Owner owner_road, Owner owner_tram, uint bridgetype, DiagDirection d, RoadTypes r, uint town)
{
	tile_make_road_bridge(&_mc[t], bridgetype, d, r, town, owner_road, owner_tram);
}

/**
 * Make a normal road tile from a road bridge ramp.
 * @param t the tile to make a normal road
 * @note roadbits will have to be adjusted when this function is called
 */
static inline void MakeNormalRoadFromBridge(TileIndex t)
{
	tile_make_road_from_bridge(&_mc[t]);
}

/**
 * Make a road bridge tile from a normal road.
 * @param t          the tile to make a road bridge
 * @param bridgetype the type of bridge this bridge ramp belongs to
 * @param d          the direction this ramp must be facing
 * @note roadbits will have to be adjusted when this function is called
 */
static inline void MakeRoadBridgeFromRoad(TileIndex t, uint bridgetype, DiagDirection d)
{
	tile_make_bridge_from_road(&_mc[t], bridgetype, d);
}


/**
 * Get the road axis of a level crossing.
 * @param t The tile to query.
 * @pre IsLevelCrossingTile(t)
 * @return The axis of the road.
 */
static inline Axis GetCrossingRoadAxis(TileIndex t)
{
	return tile_get_crossing_road_axis(&_mc[t]);
}

/**
 * Get the road bits of a level crossing.
 * @param tile The tile to query.
 * @return The present road bits.
 */
static inline RoadBits GetCrossingRoadBits(TileIndex tile)
{
	return tile_get_crossing_roadbits(&_mc[tile]);
}

/**
 * Get the rail axis of a level crossing.
 * @param t The tile to query.
 * @pre IsLevelCrossingTile(t)
 * @return The axis of the rail.
 */
static inline Axis GetCrossingRailAxis(TileIndex t)
{
	return tile_get_crossing_rail_axis(&_mc[t]);
}

/**
 * Get the rail track of a level crossing.
 * @param tile The tile to query.
 * @return The rail track.
 */
static inline Track GetCrossingRailTrack(TileIndex tile)
{
	return tile_get_crossing_rail_track(&_mc[tile]);
}

/**
 * Get the rail track bits of a level crossing.
 * @param tile The tile to query.
 * @return The rail track bits.
 */
static inline TrackBits GetCrossingRailBits(TileIndex tile)
{
	return tile_get_crossing_rail_trackbits(&_mc[tile]);
}


/**
 * Get the reservation state of the rail crossing
 * @param t the crossing tile
 * @return reservation state
 * @pre IsLevelCrossingTile(t)
 */
static inline bool HasCrossingReservation(TileIndex t)
{
	return tile_crossing_is_reserved(&_mc[t]);
}

/**
 * Set the reservation state of the rail crossing
 * @param t the crossing tile
 * @param b the reservation state
 * @pre IsLevelCrossingTile(t)
 */
static inline void SetCrossingReservation(TileIndex t, bool b)
{
	tile_crossing_set_reserved(&_mc[t], b);
}

/**
 * Get the reserved track bits for a rail crossing
 * @param t the tile
 * @pre IsLevelCrossingTile(t)
 * @return reserved track bits
 */
static inline TrackBits GetCrossingReservationTrackBits(TileIndex t)
{
	return tile_crossing_get_reserved_trackbits(&_mc[t]);
}


/**
 * Check if the level crossing is barred.
 * @param t The tile to query.
 * @pre IsLevelCrossingTile(t)
 * @return True if the level crossing is barred.
 */
static inline bool IsCrossingBarred(TileIndex t)
{
	return tile_crossing_is_barred(&_mc[t]);
}

/**
 * Set the bar state of a level crossing.
 * @param t The tile to modify.
 * @param barred True if the crossing should be barred, false otherwise.
 * @pre IsLevelCrossingTile(t)
 */
static inline void SetCrossingBarred(TileIndex t, bool barred)
{
	tile_crossing_set_barred(&_mc[t], barred);
}

/**
 * Unbar a level crossing.
 * @param t The tile to change.
 */
static inline void UnbarCrossing(TileIndex t)
{
	SetCrossingBarred(t, false);
}

/**
 * Bar a level crossing.
 * @param t The tile to change.
 */
static inline void BarCrossing(TileIndex t)
{
	SetCrossingBarred(t, true);
}


/**
 * Make a level crossing.
 * @param t       Tile to make a level crossing.
 * @param road    New owner of road.
 * @param tram    New owner of tram tracks.
 * @param rail    New owner of the rail track.
 * @param roaddir Axis of the road.
 * @param rat     New rail type.
 * @param rot     New present road types.
 * @param town    Town ID if the road is a town-owned road.
 */
static inline void MakeRoadCrossing(TileIndex t, Owner road, Owner tram, Owner rail, Axis roaddir, RailType rat, RoadTypes rot, uint town)
{
	tile_make_crossing(&_mc[t], rail, road, tram, roaddir, rat, rot, town);
}

#endif /* MAP_ROAD_H */