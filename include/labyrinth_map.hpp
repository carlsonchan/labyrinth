/*
 *
 * Author: Jeffrey Leung
 * Last edited: 2016-02-01
 *
 * This C++ header file contains classes related to creating a LabyrinthMap
 * which creates, updates, and displays a map of a given Labyrinth.
 *
 */

#pragma once

#include <cstring>
#include <iostream>
#include <memory>

#include "coordinate.hpp"
#include "room_properties.hpp"
#include "labyrinth.hpp"

// This class is a template for LabyrinthMapCoordinateBorder and
// LabyrinthMapCoordinateRoom to inherit from, so that an array can be
// made where any given element is either a Border or a Room.
//
// Any methods here will throw a logic_error if the method is called on
// an incorrect object (i.e. a Room-only method on a Border or a Border-only
// method on a Room).
//
// Methods will throw the following logic_error exceptions when not overridden
// (i.e. a method for Borders will not be implemented by Room, and thus will
// throw the error when called).
class LabyrinthMapCoordinate
{
  public:

    // Destructor
    // Prevents error messages about non-virtual destructors
    virtual ~LabyrinthMapCoordinate()
    {
    }

    // Border-only methods:

    virtual bool IsWall( const Direction d ) const
    {
      // Avoiding unused parameter warning
      (void)(d);

      throw std::logic_error( "Error: A LabyrinthMapCoordinateRoom "\
        "attempted to call IsWall(), which is a Border-only method.\n"\
        "Consider using IsRoom() to check whether the Coordinate is a" \
        "Border or Room.\n" );
    }

    virtual void SetWall( const Direction d, const bool exists )
    {
      // Avoiding unused parameter warning
      (void)(d);
      (void)(exists);

      throw std::logic_error( "Error: A LabyrinthMapCoordinateRoom "\
        "attempted to call SetWall(), which is a Border-only method.\n"\
        "Consider using IsRoom() to check whether the Coordinate is a" \
        "Border or Room.\n" );
    }

    virtual bool IsExit() const
    {
      throw std::logic_error( "Error: A LabyrinthMapCoordinateRoom "\
        "attempted to call IsExit(), which is a Border-only method.\n"\
        "Consider using IsRoom() to check whether the Coordinate is a" \
        "Border or Room.\n" );
    }

    virtual void SetExit( const bool b )
    {
      // Avoiding unused parameter warning
      (void)(b);

      throw std::logic_error( "Error: A LabyrinthMapCoordinateRoom "\
        "attempted to call SetExit(), which is a Border-only method.\n"\
        "Consider using IsRoom() to check whether the Coordinate is a" \
        "Border or Room.\n" );
    }

    // Room-only methods:

    virtual Inhabitant GetInhabitant() const
    {
      throw std::logic_error( "Error: A LabyrinthMapCoordinateBorder "\
        "attempted to call HasInhabitant(), which is a Room-only method.\n"\
        "Consider using IsRoom() to check whether the Coordinate is a" \
        "Border or Room.\n" );
    }

    virtual void SetInhabitant( const Inhabitant inh )
    {
      // Avoiding unused parameter warning
      (void)(inh);

      throw std::logic_error( "Error: A LabyrinthMapCoordinateBorder "\
        "attempted to call SetInhabitant(), which is a Room-only method.\n"\
        "Consider using IsRoom() to check whether the Coordinate is a" \
        "Border or Room.\n" );
    }

    virtual Item ItemAt() const
    {
      throw std::logic_error( "Error: A LabyrinthMapCoordinateBorder "\
        "attempted to call ItemAt(), which is a Room-only method.\n"\
        "Consider using IsRoom() to check whether the Coordinate is a" \
        "Border or Room.\n" );
    }

    virtual void SetItem( const Item i )
    {
      // Avoiding unused parameter warning
      (void)(i);

      throw std::logic_error( "Error: A LabyrinthMapCoordinateBorder "\
        "attempted to call SetItem(), which is a Room-only method.\n"\
        "Consider using IsRoom() to check whether the Coordinate is a" \
        "Border or Room.\n" );
    }

};

// This class contains necessary information about a given Border coordinate,
// so that a map can be displayed.
//
// A Border is the boundary between 2 Rooms, the corner between
// 4 Rooms, or a coordinate of the outermost wall.
// Borders/Walls are true by default to avoid working explicitly with:
//   The outer wall
class LabyrinthMapCoordinateBorder : public LabyrinthMapCoordinate
{
  public:

    // This method returns whether a given Wall coordinate has a wall in the
    // given direction.
    // An exception is thrown if:
    //   d is kNone (invalid_argument)
    bool IsWall( const Direction d ) const;

    // This method sets the Wall of a given Wall coordinate to either
    // exist or not exist.
    // May be used to set a wall which already exists, or remove a Wall
    // which has already been removed.
    // An exception is thrown if:
    //   d is kNone (invalid_argument)
    void SetWall( const Direction d, const bool exists );

    // This method returns whether a given Wall coordinate has the exit.
    bool IsExit() const;

    // This method sets whether or not a given Wall coordinate has the exit.
    // May be used to set an exit where the exit already exists, or to remove
    // an exit where none exists.
    void SetExit( const bool b );

  private:
    bool wall_north_ = true;
    bool wall_east_  = true;
    bool wall_south_ = true;
    bool wall_west_  = true;

    bool exit_       = false;
};

// This class contains necessary information about a given Room coordinate,
// so that a map can be displayed.
class LabyrinthMapCoordinateRoom : public LabyrinthMapCoordinate
{
  public:

    // This method returns the inhabitant of the current Room.
    Inhabitant GetInhabitant() const;

    // This method sets the current inhabitant of the map's Room.
    // May be used to set the inhabitant to the same inhabitant, or to
    // no inhabitant.
    void SetInhabitant( const Inhabitant inh );

    // This method returns the item in a given Room.
    Item ItemAt() const;

    // This method sets the item in a given map's Room.
    // May be used to set the same item or remove a non-existent item.
    void SetItem( const Item i );

  private:
    Inhabitant i_ = Inhabitant::kNone;
    Item itm_ = Item::kNone;
};

// This class contains a map of a Labyrinth.
// Rooms are indexed first with the y-coordinate, then with the x-coordinate.
// l_ does not use a smart pointer because it is simply a pointer to the
// related Labyrinth, not a heap allocation.
class LabyrinthMap
{
  public:

    // Parameterized constructor
    // An exception is thrown if:
    //   l is null (invalid_argument)
    //   A size of 0 is given (domain_error)
    LabyrinthMap( const Labyrinth* const l,
                  const size_t x_size,
                  const size_t y_size );

    // This method displays a map of the current Labyrinth.
    void Display();

  private:

    const Labyrinth* const l_;
    const size_t x_size_;
    const size_t y_size_;

    // 2-d array of LabyrinthMapCoordinate pointers
    std::unique_ptr<
      std::unique_ptr<
        std::unique_ptr<LabyrinthMapCoordinate>
      []>
    []> map_;
    const size_t map_x_size_;
    const size_t map_y_size_;

    // This private method returns true if the Coordinate is within the bounds
    // of the Map, and false otherwise.
    bool WithinBoundsOfMap( const Coordinate c ) const;

    // This private method returns true if the Coordinate designates a Room
    // in the map, and false if it designates a Border.
    // An exception is thrown if:
    //   The Coordinate is outside of the Map (domain_error)
    bool IsRoom( const Coordinate c ) const;

    // This private method returns a reference to the Map Coordinate at
    // the given Coordinate.
    // An exception is thrown if:
    //   The Coordinate is outside of the Map (domain_error)
    LabyrinthMapCoordinate& MapCoordinateAt( const Coordinate c ) const;

    // This private method converts a Labyrinth Coordinate to the same
    // location in the Map.
    // An exception is thrown if:
    //   The Coordinate is outside of the Labyrinth (invalid_argument)
    void LabyrinthToMap( Coordinate& c ) const;

    // This private method converts a Map Room Coordinate to the same
    // Room in the Labyrinth.
    // An exception is thrown if:
    //   The Coordinate is outside of the Labyrinth (domain_error)
    //   The Coordinate designates a border (logic_error)
    void MapToLabyrinth( Coordinate& c ) const;

    // This private method cleans excess Map Borders.
    // i.e.:
    //   Borders on the exterior of the Labyrinth
    //     ┼┴┼
    //     ┼┬┼
    //   Borders directly adjacent to a Room
    //     ┌┬┐
    //     ├ ┤
    //     └┴┘
    // Borders are NOT matched to the Labyrinth's layout; use UpdateBorders()
    // for that.
    void CleanBorders();

    // This private method updates the Map Borders by checking the contents
    // of the Labyrinth.
    // Borders in the Map but not in the Labyrinth will be removed from
    // the Map; borders in the Labyrinth but not in the Map will not
    // be added to the Map.
    void UpdateBorders();

    // This private method updates the Map Rooms by checking the contents
    // of the Labyrinth.
    void UpdateRooms();

    // This private method displays the x-axis label as well as numbering
    // of the x-coordinates of Rooms.
    // Only to be used by Display().
    void LabelXAxis() const;

    // This private method displays numbering of the y-coordinates of a Room
    // as well as the y-axis label (if in the correct position), or padding
    // if the row has no Rooms.
    // Only to be used by Display().
    // Should be called every time a row of the Map is printed.
    void LabelYAxis( const size_t y ) const;

    // This private method returns characters with the contents of the
    // given Room Coordinate.
    // Legend of symbols:
    //   Inhabitants:
    //     None:
    //     Minotaur (live):  M
    //     Minotaur (dead):  m
    //     Mirror (intact):  O
    //     Mirror (cracked): 0
    //   Items:
    //     None:
    //     Bullet:   •
    //     Treasure: T
    // An exception is thrown if:
    //   The Coordinate is outside of the Labyrinth (domain_error)
    //   The Coordinate designates a Border (logic_error)
    std::string DisplayRoom( const Coordinate c ) const;

    // This private method returns a character representing the given
    // Border Coordinate.
    // An exception is thrown if:
    //   The Coordinate is outside of the Labyrinth (domain_error)
    //   The Coordinate designates a Room (logic_error)
    std::string DisplayBorder( const Coordinate c ) const;

    // This private method displays a legend for the Map symbols.
    void DisplayLegend() const;
};
