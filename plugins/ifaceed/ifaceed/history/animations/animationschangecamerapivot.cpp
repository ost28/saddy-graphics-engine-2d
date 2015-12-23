#include "animationschangecamerapivot.h"

#include "../../core/editor.h"

#include "../../gui/uiblocks/uiblocks.h"
#include "../../gui/uiblocks/uianimationblock.h"

#include "../../blockedclosuremethodcall.h"

history::animations::ChangeCameraPivot::ChangeCameraPivot(
    sad::animations::Animation* d,
    const sad::Point3D& oldvalue,
    const sad::Point3D& newvalue

)
: history::animations::ChangeProperty<sad::Point3D>(
      d,
      "pivot",
      oldvalue,
      newvalue
)
{
}

history::animations::ChangeCameraPivot::~ChangeCameraPivot()
{

}

void history::animations::ChangeCameraPivot::updateUI(core::Editor* e, const sad::Point3D& value)
{
    gui::uiblocks::UIAnimationBlock* blk = e->uiBlocks()->uiAnimationBlock();
    
    e->emitClosure( blocked_bind(
            blk->dsbCameraRotationPivotX,
            &QDoubleSpinBox::setValue,
            value.x()
        )
    );
     e->emitClosure( blocked_bind(
            blk->dsbCameraRotationPivotY,
            &QDoubleSpinBox::setValue,
            value.y()
        )
    );
}
