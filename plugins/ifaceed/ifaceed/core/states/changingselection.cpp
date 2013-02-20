#include "../ifaceeditor.h"
#include "../../objects/abstractscreenobject.h"
#include "../../objects/screentemplate.h"
#include "../../editorcore/editorbehaviour.h"
#include "../../editorcore/editorbehaviourshareddata.h"
#include "selectedstate.h"
#include <algorithm>
#include <vector>

// Contains methods for changing selection in editor
void IFaceEditor::showObjectStats(AbstractScreenObject * o)
{
	if (o != NULL)
	{
		this->panel()->updateObjectStats(o);
	}
}

class LayerComparator 
{
public:    
	EditorLog * m_log;
	
	LayerComparator(EditorLog * log)
	{
		m_log = log;
	}

	bool operator() (AbstractScreenObject * o1, AbstractScreenObject * o2) 
	{ 
		unsigned int i1 = o1->getProperty("layer")->get(m_log)->get<unsigned int>(m_log);
		unsigned int i2 = o2->getProperty("layer")->get(m_log)->get<unsigned int>(m_log);
		return i2 > i1;
	}
};


void IFaceEditor::trySelectObject(hPointF p, bool enterSelected) 
{
	std::vector<AbstractScreenObject*> tbl;
	AbstractScreenObject * o = this->result()->templateBegin();
	
	while(o != NULL) 
	{
		if (o->isWithin(p)) 
		{
			tbl.push_back(o);
		}
		o = this->result()->templateNext();
	}
	if (tbl.empty() == false) {
		LayerComparator c(this->log());
		std::sort(tbl.begin(), tbl.end(), c);
		this->behaviourSharedData()->setSelectedObject(tbl[0]);
		this->showObjectStats(tbl[0]);
		std::vector<hst::string> chain;
		for(int i = 0; i < tbl.size(); i++) 
		{
			chain.push_back(tbl[i]->getProperty("uid")->get(m_log)->get<hst::string>(m_log));
		}
		if (enterSelected) 
		{
			this->currentBehaviour()->enterState("selected");
		}
		if (chain.size() > 1) 
		{
			SelectedState * s = static_cast<SelectedState*>(this->currentBehaviour()->getState("selected"));
			s->enterNavigation(chain);
		}
	}
}

