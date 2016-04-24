/*! \file gridactions.h

    Defines an actions for grids (sad::layouts::Grid)
 */
#pragma once
#include <QObject>
#include <QRectF>

#include <input/controls.h>
#include "abstractactions.h"
#include "../childrenprovider.h"
#include "../layouts/layoutcelledit.h"

class MainPanel;

namespace gui
{

/*! A position of scene node in grids
 */
struct GridPosition
{
    sad::layouts::Grid* Grid; //!<  A grid
    size_t Row;               //!<  A row of related cell
    size_t Col;               //!<  A column of related cell
    size_t Pos;               //!<  A position of cell grid

    /*! Makes new grid position
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
     */
    inline GridPosition(sad::layouts::Grid* g, size_t row, size_t col,  size_t pos)
    : Grid(g), Row(row), Col(col), Pos(pos)
    {
        
    }
};

namespace actions
{

/*! A group of actions, linked to grids (sad::layout::Grid)
 */
class GridActions: public QObject, public gui::actions::AbstractActions
{
Q_OBJECT
public:
    /*! Creates new label actions
        \param[in] parent a parent object
     */
    GridActions(QObject* parent = NULL);
    /*! Sets editor
        \param[in] e editor
     */
    virtual void setEditor(core::Editor* e);
    /*! This class could be inherited
     */
    virtual ~GridActions();
    /*! Returns selected grid, according to editor
        \param[in] grid a current grid
     */
    sad::layouts::Grid* selectedGrid() const;
    /*! Returns list of active grids
        \param[out] list a list of grids
     */
    static void activeGrids(sad::Vector<sad::layouts::Grid*>& grids);
    /*! Adds a grid to list of grids
        \param[in] grid a grid to be added
     */
    void addGridToGridList(sad::layouts::Grid* grid) const;
    /*! Tries to insert child to curent grid, thus updating ui
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
        \param[in] node a node
     */
    void insertChildToGrid(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, sad::SceneNode* node);
    /*! Tries to remove child from grid, affecting UI
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
     */
    void removeChildFromGridInUI(sad::layouts::Grid* g, size_t row, size_t col, size_t pos);
    /*! Removes child from a grid, affecting UI. Note, that this does not affect history.
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
     */
    void removeChildFromGrid(sad::layouts::Grid* g, size_t row, size_t col, size_t pos);
    /*! Inserts new cell editor
        \param[in] row a row
        \param[in] col a column
        \param[in] cell a cell editor
     */
    void insertCellEditor(size_t row,  size_t col, gui::layouts::LayoutCellEdit* cell);
    /*! Tries to update child name
        \param[in] g grid
        \param[in] row a row
        \param[in] col a column
        \param[in] pos a position
        \param[in] name a new name
     */
    void updateChildName(sad::layouts::Grid* g,  size_t row, size_t col, size_t pos, const QString& name);

    /*! Update current grid properties
        \param[in] immediate whether it should be invoked now, or postoponed
     */
    void updateGridPropertiesInUI(bool immediate= false);
    /*! Returns a cell editor
        \param[in] row a row
        \param[in] col a column
        \return a cell editor
     */
    gui::layouts::LayoutCellEdit* cellEditor(size_t row,  size_t col);
    /*! Seeks for related grids
        \param[in] node a node
        \return a grid position
     */
    static sad::Vector<gui::GridPosition> findRelatedGrids(sad::SceneNode* node);
public slots:
    /*! Called, when user toggles show or hide grids
        \param[in] state a new state for a checkbox
     */
    void showGridsClicked(bool state);
private:
    /*! A provider for grid action
     */
    gui::ChildrenProvider* m_provider;
    /*! A hash table from actions to local editors for provider
     */
    QHash<size_t, QHash<size_t, gui::layouts::LayoutCellEdit*> > m_cell_editors; 
};

}

}
